#pragma once
#include <random>
#include <chrono>
#include <mutex>

static uintptr_t g_pred_vel_target = 0;
static Vector3 g_pred_vel_smoothed = {};
static Vector3 g_pred_prev_base = {};
static ULONGLONG g_pred_prev_tick = 0;
static bool g_pred_has_prev = false;

std::map<int, BoneList> GetSelectedBone =
{
	{0, BoneList::head},
	{1, BoneList::spine3},
	{2, BoneList::spine1},
	{3, BoneList::pelvis},
	{4, BoneList::l_forearm},
	{5, BoneList::l_knee},
	{6, BoneList::r_forearm},
	{7, BoneList::r_knee},
};

inline const std::vector<BoneList> kBonesForClosest = {
	BoneList::head, BoneList::spine3, BoneList::spine1, BoneList::pelvis,
	BoneList::l_forearm, BoneList::l_knee, BoneList::r_forearm, BoneList::r_knee
};

static int g_aim_target_bone = static_cast<int>(BoneList::head);
static std::mutex g_aim_visual_mutex;
static bool g_aim_target_valid = false;
static Vector2 g_aim_target_screen = {};

Vector3 aim_head3d;
Vector2 aim_head2d;

void Target()
{
	float closest_dist = 9999.0f;
	players.aim_target = nullptr;
	g_aim_target_bone = static_cast<int>(BoneList::head);

	std::vector<RustPlayer> cachedList;
	{
		std::lock_guard<std::mutex> lock(g_global_mutex);
		cachedList = global.all_player_list;
	}

	Vector2 screen_center(1920.f / 2, 1080.f / 2);

	for (size_t i = 0; i < cachedList.size(); i++)
	{
		RustPlayer player = cachedList[i];
		if (player.is_local || player.is_dead || player.is_spectating)
			continue;

		BasePlayer* base_player = reinterpret_cast<BasePlayer*>(player.instance);

		if (cfg.aimbot.selected_bone == 8)
		{
			float best_bone_dist = 9999.0f;
			int best_bone = static_cast<int>(BoneList::head);
			Vector3 best_pos = {};

			for (BoneList bone_id : kBonesForClosest)
			{
				Vector3 bone_pos;
				if (g_bone_cache.get_bone(player.instance, static_cast<int>(bone_id), bone_pos))
					;
				else
					bone_pos = base_player->GetBone(bone_id);

				float world_dist = camera.CameraPos.Distance(bone_pos);
				if (world_dist > cfg.aimbot.max_distance) continue;

				Vector2 screen_pos;
				if (!camera.WorldToScreen(bone_pos, screen_pos)) continue;

				float screen_dist = screen_pos.Distance(screen_center);
				if (screen_dist < best_bone_dist && screen_dist < cfg.aimbot.fov)
				{
					best_bone_dist = screen_dist;
					best_bone = static_cast<int>(bone_id);
					best_pos = bone_pos;
				}
			}

			if (best_bone_dist < closest_dist)
			{
				closest_dist = best_bone_dist;
				players.aim_target = base_player;
				g_aim_target_bone = best_bone;
				aim_head3d = best_pos;
			}
		}
		else
		{
			BoneList bone = GetSelectedBone[cfg.aimbot.selected_bone];
			if (g_bone_cache.get_bone(player.instance, static_cast<int>(bone), aim_head3d))
				;
			else
				aim_head3d = base_player->GetBone(bone);

			float distancea = camera.CameraPos.Distance(aim_head3d);
			if (distancea > cfg.aimbot.max_distance) continue;

			if (camera.WorldToScreen(aim_head3d, aim_head2d))
			{
				float distance = aim_head2d.Distance(screen_center);
				if (distance < cfg.aimbot.fov && distance < closest_dist)
				{
					closest_dist = distance;
					players.aim_target = base_player;
					g_aim_target_bone = static_cast<int>(bone);
				}
			}
		}
	}
}

Vector2 SmoothAngle(Vector2 current, Vector2 target, float smoothness)
{
	float factor = 1.0f / smoothness;

	Vector2 delta = target - current;

	while (delta.x > 180.0f) delta.x -= 360.0f;
	while (delta.x < -180.0f) delta.x += 360.0f;
	while (delta.y > 180.0f) delta.y -= 360.0f;
	while (delta.y < -180.0f) delta.y += 360.0f;

	return Vector2(current.x + delta.x * factor, current.y + delta.y * factor);
}

static bool AimbotKeyActive()
{
	const int mode = cfg.aimbot.keybind_mode;
	const bool key_held = (GetAsyncKeyState(cfg.aimbot.keybind) & 0x8000) != 0;

	if (mode == 0) return key_held;
	if (mode == 2) return true;

	static bool toggled = false;
	static bool key_was_down = false;
	if (key_held && !key_was_down) toggled = !toggled;
	key_was_down = key_held;
	return toggled;
}

void Aimbot()
{
	while (true)
	{
		if (AimbotKeyActive() && cfg.aimbot.enable)
		{
			Target();

			if (!players.aim_target)
			{
				{ std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; }
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			Vector3 local_position = driver->Read<Vector3>(camera.cam + 0x454);
			Vector3 target_position;
			int bone_id = (cfg.aimbot.selected_bone == 8) ? g_aim_target_bone : static_cast<int>(GetSelectedBone[cfg.aimbot.selected_bone]);
			if (!g_bone_cache.get_bone(reinterpret_cast<uintptr_t>(players.aim_target), bone_id, target_position))
				target_position = players.aim_target->GetBone(static_cast<BoneList>(bone_id));

			if (target_position.x == 0 || target_position.y == 0) { std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; continue; }

			if (cfg.aimbot.prediction)
			{
				uintptr_t tid = reinterpret_cast<uintptr_t>(players.aim_target);
				if (tid != g_pred_vel_target) {
					g_pred_vel_smoothed = {}; g_pred_prev_base = {}; g_pred_prev_tick = 0; g_pred_has_prev = false;
					g_pred_vel_target = tid;
				}
				Vector3 base_pos = players.aim_target->GetBone(BoneList::pelvis);
				Vector3 velocity = players.aim_target->GetVelocity();
				bool got_vel = (velocity.x != 0.f || velocity.y != 0.f || velocity.z != 0.f);
				ULONGLONG now = GetTickCount64();
				if (!got_vel && g_pred_has_prev) {
					float dx = base_pos.x - g_pred_prev_base.x, dy = base_pos.y - g_pred_prev_base.y, dz = base_pos.z - g_pred_prev_base.z;
					float pos_delta = sqrtf(dx*dx + dy*dy + dz*dz);
					if (pos_delta > 0.001f) {
						ULONGLONG dt = now - g_pred_prev_tick;
						if (dt > 0 && dt < 500) { float t = (float)dt / 1000.f; velocity.x = dx/t; velocity.y = dy/t; velocity.z = dz/t; got_vel = true; }
					}
				}
				float pos_delta = sqrtf((base_pos.x-g_pred_prev_base.x)*(base_pos.x-g_pred_prev_base.x) + (base_pos.y-g_pred_prev_base.y)*(base_pos.y-g_pred_prev_base.y) + (base_pos.z-g_pred_prev_base.z)*(base_pos.z-g_pred_prev_base.z));
				if (!g_pred_has_prev || pos_delta > 0.001f) { g_pred_prev_base = base_pos; g_pred_prev_tick = now; g_pred_has_prev = true; }
				float vel_mag = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
				if (vel_mag > 25.f) { float s = 25.f/vel_mag; velocity.x*=s; velocity.y*=s; velocity.z*=s; }
				float dist = local_position.Distance(target_position);
				float lerp = 0.85f - (dist/150.f)*0.4f; if (lerp < 0.35f) lerp = 0.35f;
				if (velocity.x!=0.f||velocity.y!=0.f||velocity.z!=0.f) {
					g_pred_vel_smoothed.x += (velocity.x - g_pred_vel_smoothed.x) * lerp;
					g_pred_vel_smoothed.y += (velocity.y - g_pred_vel_smoothed.y) * lerp;
					g_pred_vel_smoothed.z += (velocity.z - g_pred_vel_smoothed.z) * lerp;
				} else if (g_pred_has_prev && (now - g_pred_prev_tick) > 150) {
					g_pred_vel_smoothed.x *= 0.92f; g_pred_vel_smoothed.y *= 0.92f; g_pred_vel_smoothed.z *= 0.92f;
				}
				float bullet_speed = players.local_player->GetHeldItem().get_bullet_speed();
				target_position = CalculatePrediction(local_position, target_position, g_pred_vel_smoothed, bullet_speed);
			}

			Vector2 target_screen;
			if (camera.WorldToScreen(target_position, target_screen))
			{
				std::lock_guard<std::mutex> lock(g_aim_visual_mutex);
				g_aim_target_valid = true;
				g_aim_target_screen = target_screen;
			}

			Vector2 target_angle = CalculateAngle(local_position, target_position);

			if (cfg.aimbot.smooth && cfg.aimbot.smoothness > 1.0f)
			{
				Vector2 current_angle = players.local_player->GetPlayerInput()->GetViewAngles();
				Vector2 smooth_angle = SmoothAngle(current_angle, target_angle, cfg.aimbot.smoothness);
				players.local_player->GetPlayerInput()->SetViewAngles(smooth_angle);
			}
			else
			{
				players.local_player->GetPlayerInput()->SetViewAngles(target_angle);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(4));
		}
		else if (AimbotKeyActive() && cfg.aimbot.silentaim)
		{
			std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> dist(0, 100);
			int chance = dist(rng);

			if (chance <= cfg.aimbot.hitchance)
			{
				Target();

				if (!players.aim_target)
				{
					{ std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; }
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				Vector3 local_position = driver->Read<Vector3>(camera.cam + 0x454);
				Vector3 target_position;
				int bone_id = (cfg.aimbot.selected_bone == 8) ? g_aim_target_bone : static_cast<int>(GetSelectedBone[cfg.aimbot.selected_bone]);
				if (!g_bone_cache.get_bone(reinterpret_cast<uintptr_t>(players.aim_target), bone_id, target_position))
					target_position = players.aim_target->GetBone(static_cast<BoneList>(bone_id));

				if (target_position.x == 0 || target_position.y == 0) { std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; continue; }

				if (cfg.aimbot.prediction)
				{
					uintptr_t tid = reinterpret_cast<uintptr_t>(players.aim_target);
					if (tid != g_pred_vel_target) {
						g_pred_vel_smoothed = {}; g_pred_prev_base = {}; g_pred_prev_tick = 0; g_pred_has_prev = false;
						g_pred_vel_target = tid;
					}
					Vector3 base_pos = players.aim_target->GetBone(BoneList::pelvis);
					Vector3 velocity = players.aim_target->GetVelocity();
					bool got_vel = (velocity.x != 0.f || velocity.y != 0.f || velocity.z != 0.f);
					ULONGLONG now = GetTickCount64();
					if (!got_vel && g_pred_has_prev) {
						float dx = base_pos.x - g_pred_prev_base.x, dy = base_pos.y - g_pred_prev_base.y, dz = base_pos.z - g_pred_prev_base.z;
						float pos_delta = sqrtf(dx*dx + dy*dy + dz*dz);
						if (pos_delta > 0.001f) {
							ULONGLONG dt = now - g_pred_prev_tick;
							if (dt > 0 && dt < 500) { float t = (float)dt / 1000.f; velocity.x = dx/t; velocity.y = dy/t; velocity.z = dz/t; got_vel = true; }
						}
					}
					float pos_delta = sqrtf((base_pos.x-g_pred_prev_base.x)*(base_pos.x-g_pred_prev_base.x) + (base_pos.y-g_pred_prev_base.y)*(base_pos.y-g_pred_prev_base.y) + (base_pos.z-g_pred_prev_base.z)*(base_pos.z-g_pred_prev_base.z));
					if (!g_pred_has_prev || pos_delta > 0.001f) { g_pred_prev_base = base_pos; g_pred_prev_tick = now; g_pred_has_prev = true; }
					float vel_mag = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z);
					if (vel_mag > 25.f) { float s = 25.f/vel_mag; velocity.x*=s; velocity.y*=s; velocity.z*=s; }
					float dist = local_position.Distance(target_position);
					float lerp = 0.85f - (dist/150.f)*0.4f; if (lerp < 0.35f) lerp = 0.35f;
					if (velocity.x!=0.f||velocity.y!=0.f||velocity.z!=0.f) {
						g_pred_vel_smoothed.x += (velocity.x - g_pred_vel_smoothed.x) * lerp;
						g_pred_vel_smoothed.y += (velocity.y - g_pred_vel_smoothed.y) * lerp;
						g_pred_vel_smoothed.z += (velocity.z - g_pred_vel_smoothed.z) * lerp;
					} else if (g_pred_has_prev && (now - g_pred_prev_tick) > 150) {
						g_pred_vel_smoothed.x *= 0.92f; g_pred_vel_smoothed.y *= 0.92f; g_pred_vel_smoothed.z *= 0.92f;
					}
					float bullet_speed = players.local_player->GetHeldItem().get_bullet_speed();
					target_position = CalculatePrediction(local_position, target_position, g_pred_vel_smoothed, bullet_speed);
				}

				Vector2 target_screen;
				if (camera.WorldToScreen(target_position, target_screen))
				{
					std::lock_guard<std::mutex> lock(g_aim_visual_mutex);
					g_aim_target_valid = true;
					g_aim_target_screen = target_screen;
				}

				Vector2 angle = CalculateAngle(local_position, target_position);

				players.local_player->GetPlayerEyes()->SetBodyRotation(ToQuat(Vector3(angle.x, angle.y, 0.0f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
			else
			{
				{ std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; }
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
			}
		}
		else
		{
			{ std::lock_guard<std::mutex> lock(g_aim_visual_mutex); g_aim_target_valid = false; }
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}
