#pragma once
#include <algorithm>
#include <chrono>

void UpdateCamera()
{
	while (true)
	{
		camera.cam = camera.GetCamera();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}

void UpdateLists()
{
	using clock = std::chrono::steady_clock;
	constexpr auto kHeartbeatInterval = std::chrono::milliseconds(2);
	constexpr auto kWorldRefreshInterval = std::chrono::milliseconds(2);
	constexpr auto kItemRefreshInterval = std::chrono::milliseconds(2);

	auto next_world_refresh = clock::now();
	auto next_item_refresh = clock::now();

	while (1)
	{
		std::vector<RustPlayer> all_players = world.GetAllPlayerList();
		std::vector<RustPlayer> filtered_players = world.FilterPlayerList(all_players);
		std::vector<RustPlayer> player_copy = all_players;

		const bool any_world = cfg.world_esp.elite_crate || cfg.world_esp.military_crate || cfg.world_esp.crate ||
			cfg.world_esp.airdrop || cfg.world_esp.locked_crate || cfg.world_esp.auto_turret ||
			cfg.world_esp.flame_turret || cfg.world_esp.shotgun_trap || cfg.world_esp.corpse ||
			cfg.world_esp.caves ||
			cfg.world_esp.stone || cfg.world_esp.metal || cfg.world_esp.sulfur || cfg.world_esp.hemp ||
			cfg.world_esp.animals;

		const bool track_items = cfg.world_esp.dropped_items;
		const auto now = clock::now();

		const bool refresh_world = any_world && now >= next_world_refresh;
		const bool refresh_items = track_items && now >= next_item_refresh;

		std::vector<WorldEntity> world_entities;
		std::vector<RustItem> dropped_items;

		if (refresh_world)
		{
			world_entities = world.GetWorldEntityList();
			next_world_refresh = now + kWorldRefreshInterval;
		}
		else if (!any_world)
		{
			next_world_refresh = now;
		}

		if (refresh_items)
		{
			dropped_items = world.GetDroppedItemList();
			next_item_refresh = now + kItemRefreshInterval;
		}
		else if (!track_items)
		{
			next_item_refresh = now;
		}

		{
			std::lock_guard<std::mutex> lock(g_global_mutex);
			global.all_player_list = std::move(all_players);
			global.player_list = std::move(filtered_players);

			if (!any_world)
				global.world_entity_list.clear();
			else if (refresh_world)
				global.world_entity_list = std::move(world_entities);

			if (!track_items)
				global.item_list.clear();
			else if (refresh_items)
				global.item_list = std::move(dropped_items);
		}

		g_bone_cache.tick();
		std::vector<uintptr_t> valid_instances;
		valid_instances.reserve(player_copy.size());
		for (const auto& p : player_copy) valid_instances.push_back(p.instance);
		g_bone_cache.remove_stale(valid_instances);

		const float max_cache_dist = cfg.player_visuals.max_distance + 50.0f;

		struct PlayerDist { const RustPlayer* p; float dist; bool fresh; Vector3 head_pos; };
		std::vector<PlayerDist> to_update;
		to_update.reserve(player_copy.size());

		for (const auto& player : player_copy)
		{
			if (player.is_local) continue;

			Vector3 head_pos;
			float distance;
			bool has_head = g_bone_cache.get_bone(player.instance, static_cast<int>(BoneList::head), head_pos);
			if (has_head)
				distance = camera.CameraPos.Distance(head_pos);
			else
			{
				head_pos = reinterpret_cast<BasePlayer*>(player.instance)->GetBone(BoneList::head);
				distance = camera.CameraPos.Distance(head_pos);
			}

			if (distance > max_cache_dist) continue;

			bool fresh = !has_head;
			if (!fresh && !g_bone_cache.should_update(player.instance, distance)) continue;

			to_update.push_back({ &player, distance, fresh, head_pos });
		}

		std::stable_sort(to_update.begin(), to_update.end(), [](const PlayerDist& a, const PlayerDist& b) {
			if (a.fresh != b.fresh) return a.fresh > b.fresh;
			return a.dist < b.dist;
		});

		constexpr int kMaxCachedUpdatesPerTick = 300;
		int cached_updated = 0;
		for (const auto& pd : to_update)
		{
			if (!pd.fresh && cached_updated >= kMaxCachedUpdatesPerTick)
				break;
			const auto& player = *pd.p;

			auto* bp = reinterpret_cast<BasePlayer*>(player.instance);
			PlayerBones bones;
			bool any_set = false;
			for (BoneList bone_id : kBonesToCache)
			{
				Vector3 pos = bp->GetBone(bone_id);
				if (pos.x != 0.f || pos.y != 0.f || pos.z != 0.f)
				{
					bones.set(static_cast<int>(bone_id), pos);
					any_set = true;
				}
			}
			if (any_set)
			{
				g_bone_cache.update(player.instance, bones);
				if (!pd.fresh)
					cached_updated++;
			}
			else if (pd.fresh && (pd.head_pos.x != 0.f || pd.head_pos.y != 0.f || pd.head_pos.z != 0.f))
			{
				bones.set(static_cast<int>(BoneList::head), pd.head_pos);
				const Vector3 feet_approx(pd.head_pos.x, pd.head_pos.y - 1.8f, pd.head_pos.z);
				bones.set(static_cast<int>(BoneList::l_foot), feet_approx);
				bones.set(static_cast<int>(BoneList::r_foot), feet_approx);
				g_bone_cache.update(player.instance, bones);
			}
		}

		std::this_thread::sleep_for(kHeartbeatInterval);
	}
}
