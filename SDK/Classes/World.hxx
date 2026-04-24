#pragma once

enum class EntityCategory : int
{
	Player,
	Scientist,
	Dweller
};

struct EntityDefinition
{
	const char* prefab;
	const char* display_name;
	EntityCategory category;
};

inline constexpr EntityDefinition g_entity_registry[] =
{
	{ "player.prefab",                    "Player",    EntityCategory::Player    },
	{ "scientistnpc_roam",                "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_roamthered",          "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_patrol",              "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_oilrig",              "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo",               "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_excavator",           "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_airfield",            "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_trainyard",           "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_powerplant",          "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_militarytunnel",      "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_labs",                "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_arena",               "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_full_any",            "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_full_lr300",          "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_full_mp5",            "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_full_pistol",         "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_full_shotgun",        "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo_turret_any",    "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo_turret_lr300",  "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo_any",           "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo_mp5",           "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_cargo_shotgun",       "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_heavy",               "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_peacekeeper",         "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_bandit",              "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_ch47_gunner",         "Scientist", EntityCategory::Scientist },
	{ "ch47scientist.entity",             "Scientist", EntityCategory::Scientist },
	{ "scientistnpc_junkpile_pistol",     "Scientist", EntityCategory::Scientist },
	{ "patrolhelicopter",                 "Scientist", EntityCategory::Scientist },
	{ "npc_tunneldweller",                "Dweller",   EntityCategory::Dweller   },
	{ "npc_underwaterdweller",            "Dweller",   EntityCategory::Dweller   },
};

struct NameListEntry
{
	const char* prefab;
	const char* display_name;
	ListType type;
};

inline const std::vector<NameListEntry> g_name_list =
{
	{ "player.prefab",                    "Player",          ListType::Player   },
	{ "patrolhelicopter",                 "Scientist",       ListType::Player   },
	{ "ch47scientist.entity",             "Scientist",       ListType::Player   },
	{ "scientistnpc_cargo",               "Scientist",       ListType::Player   },
	{ "scientistnpc_cargo_turret_any",    "Scientist",       ListType::Player   },
	{ "scientistnpc_cargo_turret_lr300",  "Scientist",       ListType::Player   },
	{ "scientistnpc_ch47_gunner",         "Scientist",       ListType::Player   },
	{ "scientistnpc_excavator",           "Scientist",       ListType::Player   },
	{ "scientistnpc_full_any",            "Scientist",       ListType::Player   },
	{ "scientistnpc_full_lr300",          "Scientist",       ListType::Player   },
	{ "scientistnpc_full_mp5",            "Scientist",       ListType::Player   },
	{ "scientistnpc_full_pistol",         "Scientist",       ListType::Player   },
	{ "scientistnpc_full_shotgun",        "Scientist",       ListType::Player   },
	{ "scientistnpc_heavy",               "Scientist",       ListType::Player   },
	{ "scientistnpc_junkpile_pistol",     "Scientist",       ListType::Player   },
	{ "scientistnpc_oilrig",              "Scientist",       ListType::Player   },
	{ "scientistnpc_patrol",              "Scientist",       ListType::Player   },
	{ "scientistnpc_roam",                "Scientist",       ListType::Player   },
	{ "scientistnpc_roamthered",          "Scientist",       ListType::Player   },
	{ "npc_tunneldweller",                "Dweller",         ListType::Player   },
	{ "npc_underwaterdweller",            "Dweller",         ListType::Player   },

	{ "stone-ore",                        "Stone",           ListType::Resource },
	{ "sulfur-ore",                       "Sulfur",          ListType::Resource },
	{ "metal-ore",                        "Metal",           ListType::Resource },
	{ "hemp",                             "Hemp",            ListType::Resource },
	{ "wood-pile",                        "Wood",            ListType::Resource },

	{ "bear.prefab",                      "Bear",            ListType::World    },
	{ "polarbear.prefab",                 "Polar Bear",      ListType::World    },
	{ "wolf.prefab",                      "Wolf",            ListType::World    },
	{ "stag.prefab",                      "Stag",            ListType::World    },
	{ "boar.prefab",                      "Boar",            ListType::World    },
	{ "horse.prefab",                     "Horse",           ListType::World    },
	{ "chicken.prefab",                   "Chicken",         ListType::World    },
	{ "shark.prefab",                     "Shark",           ListType::World    },
	{ "simpleshark.prefab",               "Shark",           ListType::World    },

	{ "crate_elite.prefab",              "Elite Crate",      ListType::World    },
	{ "crate_normal.prefab",             "Military Crate",   ListType::World    },
	{ "crate_normal_2.prefab",           "Crate",            ListType::World    },
	{ "supply_drop",                     "AirDrop",          ListType::World    },
	{ "codelockedhackablecrate",         "Locked Crate",     ListType::World    },
	{ "autoturret_deployed.prefab",      "Autoturret",       ListType::World    },
	{ "flameturret.deployed.prefab",     "Flameturret",      ListType::World    },
	{ "guntrap.deployed.prefab",         "Shotgun Trap",     ListType::World    },
	{ "player_corpse.prefab",            "Corpse",           ListType::World    },
	{ "item_drop_backpack.prefab",       "Corpse",           ListType::World    },

	{ "cave_small_static",               "Cave",             ListType::World    },
	{ "cave_medium_static",              "Cave",             ListType::World    },
	{ "cave_large_static",               "Cave",             ListType::World    },

	{ "(world)",                          "Dropped Item",    ListType::Item     },
};

inline const std::vector<std::string> g_animal_names = {
	"Bear", "Polar Bear", "Wolf", "Stag", "Boar", "Horse", "Chicken", "Shark"
};

inline bool IsAnimal(const std::string& display_name)
{
	for (const auto& name : g_animal_names)
		if (display_name == name) return true;
	return false;
}

inline bool IsWorldEntityEnabled(const std::string& display_name)
{
	if (IsAnimal(display_name))           return cfg.world_esp.animals;
	if (display_name == "Elite Crate")    return cfg.world_esp.elite_crate;
	if (display_name == "Military Crate") return cfg.world_esp.military_crate;
	if (display_name == "Crate")          return cfg.world_esp.crate;
	if (display_name == "AirDrop")        return cfg.world_esp.airdrop;
	if (display_name == "Locked Crate")   return cfg.world_esp.locked_crate;
	if (display_name == "Autoturret")     return cfg.world_esp.auto_turret;
	if (display_name == "Flameturret")    return cfg.world_esp.flame_turret;
	if (display_name == "Shotgun Trap")   return cfg.world_esp.shotgun_trap;
	if (display_name == "Corpse")         return cfg.world_esp.corpse;
	if (display_name == "Cave")           return cfg.world_esp.caves;
	if (display_name == "Stone")          return cfg.world_esp.stone;
	if (display_name == "Metal")          return cfg.world_esp.metal;
	if (display_name == "Sulfur")         return cfg.world_esp.sulfur;
	if (display_name == "Hemp")           return cfg.world_esp.hemp;
	return false;
}

class World
{
public:
	std::vector<RustPlayer> GetAllPlayerList()
	{
		std::vector<RustPlayer> list;

		uint64_t player_list = GetEntityList();
		if (!player_list)
			return list;

		uint64_t items = driver->Read<uint64_t>(player_list + Offsets::BaseNetworkableClass::List);
		uint32_t count = driver->Read<uint32_t>(player_list + Offsets::BaseNetworkableClass::Count);

		if (!items || !count)
			return list;

		CacheLocalPlayer(items);

		for (uint32_t i = 0; i < count; i++)
		{
			uint64_t entity = driver->Read<uint64_t>(items + 0x20 + (i * 0x8));
			if (!entity)
				continue;

			const auto* definition = ResolveEntity(entity);
			if (!definition)
				continue;

			RustPlayer player;
			if (!BuildPlayerSnapshot(entity, *definition, player))
				continue;

			list.push_back(std::move(player));
		}
		return list;
	}

	std::vector<RustPlayer> GetPlayerList()
	{
		return FilterPlayerList(GetAllPlayerList());
	}

	std::vector<RustPlayer> FilterPlayerList(const std::vector<RustPlayer>& players) const
	{
		std::vector<RustPlayer> filtered;
		filtered.reserve(players.size());

		for (const auto& player : players)
		{
			if (!ShouldIncludeFilteredPlayer(player))
				continue;

			filtered.push_back(player);
		}

		return filtered;
	}

	std::vector<WorldEntity> GetWorldEntityList()
	{
		std::vector<WorldEntity> list;

		uint64_t entity_realm = GetEntityList();
		if (!entity_realm)
			return list;

		uint64_t entity_buffer = driver->Read<uint64_t>(entity_realm + Offsets::BaseNetworkableClass::List);
		uint32_t entity_count = driver->Read<uint32_t>(entity_realm + Offsets::BaseNetworkableClass::Count);

		if (!entity_buffer || !entity_count)
			return list;

		for (uint32_t i = 0; i < entity_count; i++)
		{
			uint64_t entity = driver->Read<uint64_t>(entity_buffer + 0x20 + (i * 0x8));
			if (!entity) continue;

			const auto* entry = ResolveNameList(entity);
			if (!entry) continue;

			if (entry->type != ListType::World && entry->type != ListType::Resource) continue;
			if (!IsWorldEntityEnabled(entry->display_name)) continue;

			Vector3 position = GetEntityPosition(entity);
			if (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f) continue;

			WorldEntity world_ent;
			world_ent.instance = entity;
			world_ent.position = position;
			world_ent.display_name = entry->display_name;
			world_ent.type = entry->type;
			list.push_back(std::move(world_ent));
		}

		return list;
	}

	std::vector<RustItem> GetDroppedItemList()
	{
		std::vector<RustItem> list;

		uint64_t entity_realm = GetEntityList();
		if (!entity_realm)
			return list;

		uint64_t entity_buffer = driver->Read<uint64_t>(entity_realm + Offsets::BaseNetworkableClass::List);
		uint32_t entity_count = driver->Read<uint32_t>(entity_realm + Offsets::BaseNetworkableClass::Count);

		if (!entity_buffer || !entity_count)
			return list;

		for (uint32_t i = 0; i < entity_count; i++)
		{
			uint64_t entity = driver->Read<uint64_t>(entity_buffer + 0x20 + (i * 0x8));
			if (!entity)
				continue;

			std::string prefab_name = GetPrefabName(entity);
			if (prefab_name.empty())
				continue;

			if (prefab_name.find("(world)") == std::string::npos)
				continue;

			Vector3 position = GetEntityPosition(entity);
			if (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f)
				continue;

			std::string item_name = prefab_name;

			size_t last_slash = item_name.rfind('/');
			if (last_slash != std::string::npos)
				item_name = item_name.substr(last_slash + 1);

			size_t prefab_ext = item_name.find(".prefab");
			if (prefab_ext != std::string::npos)
				item_name.erase(prefab_ext);

			size_t world_pos = item_name.find("(world)");
			if (world_pos != std::string::npos)
				item_name.erase(world_pos, 7);

			if (item_name.empty())
				continue;

			auto it = g_item_display_names.find(item_name);
			if (it != g_item_display_names.end())
				item_name = it->second.display_name;

			RustItem dropped;
			dropped.instance = entity;
			dropped.position = position;
			dropped.name = item_name;

			list.push_back(std::move(dropped));
		}
		return list;
	}

private:
	bool BuildPlayerSnapshot(uint64_t entity, const EntityDefinition& definition, RustPlayer& out_player)
	{
		auto* base_player = reinterpret_cast<BasePlayer*>(entity);
		if (!base_player)
			return false;

		auto held = base_player->GetHeldItem();
		bool is_npc = (definition.category == EntityCategory::Scientist || definition.category == EntityCategory::Dweller);

		out_player.instance = entity;
		out_player.name = base_player->GetName();
		out_player.weapon = held.get_display_name();
		out_player.weapon_tier = held.get_weapon_tier();
		out_player.team_id = base_player->GetCurrentTeam();
		out_player.health_status = base_player->GetHealthStatus();
		out_player.is_local = (entity == reinterpret_cast<uint64_t>(players.local_player));
		out_player.is_npc = is_npc;
		out_player.is_dead = base_player->IsDead();
		out_player.is_sleeping = base_player->IsSleeping();
		out_player.is_spectating = base_player->IsSpectating();
		out_player.is_incapacitated = base_player->IsIncapacitated();
		out_player.is_receiving_snapshot = base_player->HasFlag(PlayerFlags::ReceivingSnapshot);

		if (out_player.is_local)
		{
			out_player.health = base_player->GetHealth();
			out_player.max_health = base_player->GetMaxHealth();
		}

		return true;
	}

	bool ShouldIncludeFilteredPlayer(const RustPlayer& player) const
	{
		if (player.is_dead)
			return false;

		if (player.is_sleeping)
			return false;

		if (player.is_spectating)
			return false;

		if (player.is_incapacitated)
			return false;

		if (player.is_receiving_snapshot)
			return false;

		return true;
	}

	std::string GetPrefabName(uint64_t entity)
	{
		uint64_t step1 = driver->Read<uint64_t>(entity + Offsets::GameObject::Object);
		if (!step1) return {};
		uint64_t step2 = driver->Read<uint64_t>(step1 + Offsets::GameObject::Transform);
		if (!step2) return {};
		uint64_t game_buffer = driver->Read<uint64_t>(step2 + Offsets::GameObject::PrefabName);
		if (!game_buffer) return {};
		return driver->ReadChar(game_buffer);
	}

	Vector3 GetEntityPosition(uint64_t entity)
	{
		uint64_t s1 = driver->Read<uint64_t>(entity + Offsets::GameObject::Object);
		if (!s1) return {};
		uint64_t s2 = driver->Read<uint64_t>(s1 + Offsets::GameObject::Transform);
		if (!s2) return {};
		uint64_t s3 = driver->Read<uint64_t>(s2 + Offsets::TransformChain::VisualState);
		if (!s3) return {};
		uint64_t s4 = driver->Read<uint64_t>(s3 + Offsets::TransformChain::StateData);
		if (!s4) return {};
		uintptr_t position_ptr = driver->Read<uintptr_t>(s4 + Offsets::TransformChain::TransformArray);
		if (!position_ptr) return {};
		return driver->Read<Vector3>(position_ptr + Offsets::TransformChain::Position);
	}

	const NameListEntry* ResolveNameList(uint64_t entity)
	{
		std::string prefab_name = GetPrefabName(entity);
		if (prefab_name.empty())
			return nullptr;

		for (const auto& entry : g_name_list)
		{
			if (prefab_name.find(entry.prefab) != std::string::npos)
				return &entry;
		}
		return nullptr;
	}

	uint64_t GetEntityList()
	{
		uint64_t base_networkable = driver->Read<uint64_t>(globals.game_assembly + Offsets::BaseNetworkable);
		if (!base_networkable) return 0;
		std::cout << "base_networkable : " << base_networkable << std::endl;
		uint64_t static_fields = driver->Read<uint64_t>(base_networkable + Offsets::BaseNetworkableClass::static_fields);
		if (!static_fields) return 0;
		std::cout << "static_fields : " << static_fields << std::endl;

		uint64_t wrapper_ptr = driver->Read<uint64_t>(static_fields + Offsets::BaseNetworkableClass::wrapper_class_ptr);
		if (!wrapper_ptr) return 0;
		std::cout << "wrapper_ptr : " << wrapper_ptr << std::endl;

		uint64_t wrapper_class = decryption.ClientEntities(wrapper_ptr);
		if (!wrapper_class) return 0;
		std::cout << "wrapper_class : " << wrapper_class << std::endl;

		uint64_t parent_ptr = driver->Read<uint64_t>(wrapper_class + Offsets::BaseNetworkableClass::parent_static_fields);
		if (!parent_ptr) return 0;
		std::cout << "parent_ptr : " << parent_ptr << std::endl;

		uint64_t parent_class = decryption.EntityList(parent_ptr);
		if (!parent_class) return 0;
		std::cout << "parent_class : " << parent_class << std::endl;

		uint64_t entity_list = driver->Read<uint64_t>(parent_class + Offsets::BaseNetworkableClass::entity);
		return entity_list;
	}

	void CacheLocalPlayer(uint64_t items)
	{
		uint64_t local = driver->Read<uint64_t>(items + 0x20);
		if (local)
			players.local_player = reinterpret_cast<BasePlayer*>(local);
	}

	const EntityDefinition* ResolveEntity(uint64_t entity)
	{
		std::string prefab_name = GetPrefabName(entity);
		if (prefab_name.empty())
			return nullptr;

		for (const auto& def : g_entity_registry)
		{
			if (prefab_name.find(def.prefab) != std::string::npos)
				return &def;
		}
		return nullptr;
	}
} world;