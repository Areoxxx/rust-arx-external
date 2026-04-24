#pragma once

uintptr_t EncryptFloat(float fov)
{
	uint32_t eax = std::bit_cast<uint32_t>(fov);
	eax ^= 0x4CF8075;
	eax += 0x9F713A3E;
	uint32_t ecx = eax;
	eax <<= 0x1B;
	ecx >>= 5;
	ecx |= eax;
	return ecx;
}

inline std::uint32_t decrypt_fov(std::uint64_t a1)
{
	std::uint32_t v0 = static_cast<std::uint32_t>(a1);
	v0 = (v0 >> 0x1B) | (v0 << 5);
	v0 = v0 + 0x608EC5C2;
	v0 = v0 ^ 0x4CF8075;
	return v0;
}

inline void set_game_fov(float fov)
{
	uintptr_t convar_graphics = driver->Read<uintptr_t>(globals.game_assembly + Offsets::ConvarGraphics::Base);
	if (!driver->is_valid(convar_graphics))
		return;

	uintptr_t static_fields = driver->Read<uintptr_t>(convar_graphics + 0xB8);
	if (!driver->is_valid(static_fields))
		return;

	uint32_t encrypted = EncryptFloat(fov);
	driver->Write<uint32_t>(static_fields + Offsets::ConvarGraphics::fov, encrypted);
}

inline void HandleRemoveLayers()
{
	static bool last_active = false;
	static bool last_construction = false;
	static bool last_terrain = false;
	static bool last_tree = false;
	static bool last_clutter = false;
	static bool last_debris = false;
	static bool last_deployed = false;
	static bool last_transparent = false;
	static bool last_vehicle_large = false;
	static bool last_prevent_building = false;

	static bool key_was_pressed = false;
	bool key_pressed = (GetAsyncKeyState(cfg.player_visuals.remove_layer_keybind) & 0x8000) != 0;
	if (key_pressed && !key_was_pressed)
		cfg.player_visuals.remove_layer_active = !cfg.player_visuals.remove_layer_active;
	key_was_pressed = key_pressed;

	if (!cfg.player_visuals.remove_layer)
		cfg.player_visuals.remove_layer_active = false;

	bool changed =
		(cfg.player_visuals.remove_layer_active != last_active) ||
		(cfg.player_visuals.remove_construction != last_construction) ||
		(cfg.player_visuals.remove_terrain != last_terrain) ||
		(cfg.player_visuals.remove_tree != last_tree) ||
		(cfg.player_visuals.remove_clutter != last_clutter) ||
		(cfg.player_visuals.remove_debris != last_debris) ||
		(cfg.player_visuals.remove_deployed != last_deployed) ||
		(cfg.player_visuals.remove_transparent != last_transparent) ||
		(cfg.player_visuals.remove_vehicle_large != last_vehicle_large) ||
		(cfg.player_visuals.remove_prevent_building != last_prevent_building);

	if (!changed) return;

	bool hide = cfg.player_visuals.remove_layer_active;

	camera.SetLayerVisible(LayerMasks::Construction, cfg.player_visuals.remove_construction ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Terrain, cfg.player_visuals.remove_terrain ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Tree, cfg.player_visuals.remove_tree ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Clutter, cfg.player_visuals.remove_clutter ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Debris, cfg.player_visuals.remove_debris ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Deployed, cfg.player_visuals.remove_deployed ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Transparent, cfg.player_visuals.remove_transparent ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Vehicle_Large, cfg.player_visuals.remove_vehicle_large ? !hide : true);
	camera.SetLayerVisible(LayerMasks::Prevent_Building, cfg.player_visuals.remove_prevent_building ? !hide : true);

	last_active = cfg.player_visuals.remove_layer_active;
	last_construction = cfg.player_visuals.remove_construction;
	last_terrain = cfg.player_visuals.remove_terrain;
	last_tree = cfg.player_visuals.remove_tree;
	last_clutter = cfg.player_visuals.remove_clutter;
	last_debris = cfg.player_visuals.remove_debris;
	last_deployed = cfg.player_visuals.remove_deployed;
	last_transparent = cfg.player_visuals.remove_transparent;
	last_vehicle_large = cfg.player_visuals.remove_vehicle_large;
	last_prevent_building = cfg.player_visuals.remove_prevent_building;
}

inline void HandleShootInAir()
{
	if (!players.local_player) return;

	players.local_player->SetOnGround(true);
}

inline uint64_t get_tod_sky()
{
	return driver->ReadChain<uint64_t>(globals.game_assembly, {
		Offsets::TodSky::Class, 0xB8, 0x28, 0x10, 0x20
		});
}

inline void set_time_of_day(float hour)
{
	uint64_t sky = get_tod_sky();
	if (!sky) return;

	uint64_t cycle = driver->Read<uint64_t>(sky + Offsets::TodSky::Cycle);
	if (!cycle || !driver->is_valid(cycle))
		return;

	driver->Write<float>(cycle + Offsets::TodSky::TOD_CycleParameters::Hour, hour);
}

inline void set_night_brightness(float brightness)
{
	uint64_t sky = get_tod_sky();
	if (!sky) return;

	uint64_t night = driver->Read<uint64_t>(sky + Offsets::TodSky::Night);
	if (!night || !driver->is_valid(night))
		return;

	driver->Write<float>(night + Offsets::TodSky::TOD_NightParameters::LightIntensity, brightness);
	driver->Write<float>(night + Offsets::TodSky::TOD_NightParameters::AmbientMultiplier, brightness);
	driver->Write<float>(night + Offsets::TodSky::TOD_NightParameters::ReflectionMultiplier, brightness);
}

inline bool fly_active = false;
inline ULONGLONG fly_toggle_cooldown = 0;

inline void HandleFly()
{
	if (!players.local_player) return;

	ULONGLONG now = GetTickCount64();
	if ((GetAsyncKeyState(cfg.movement.fly_keybind) & 1) && (now - fly_toggle_cooldown) > 300)
	{
		fly_active = !fly_active;
		fly_toggle_cooldown = now;
	}

	auto* base_movement = players.local_player->GetBaseMovement();
	if (!base_movement || !driver->is_valid(reinterpret_cast<uintptr_t>(base_movement)))
		return;

	if (!fly_active)
	{
		if (!cfg.movement.fly_safe_mode)
		{
			base_movement->SetFlying(false);
			base_movement->SetGravityMultiplier(1.0f);
		}
		return;
	}

	if (!cfg.movement.fly_safe_mode)
	{
		base_movement->SetFlying(true);
		base_movement->SetGravityMultiplier(0.0f);
	}

	PlayerEyes* eyes = players.local_player->GetPlayerEyes();
	if (!eyes || !driver->is_valid(reinterpret_cast<uintptr_t>(eyes)))
		return;

	Vector4 rotation = eyes->GetBodyRotationFly();
	Vector3 target_movement = { 0, 0, 0 };

	if (GetAsyncKeyState('W') & 0x8000)
		target_movement += rotation * Vector3{ 0, 0, 1 };

	if (GetAsyncKeyState('S') & 0x8000)
		target_movement -= rotation * Vector3{ 0, 0, 1 };

	if (GetAsyncKeyState('A') & 0x8000)
		target_movement -= rotation * Vector3{ 1, 0, 0 };

	if (GetAsyncKeyState('D') & 0x8000)
		target_movement += rotation * Vector3{ 1, 0, 0 };

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		target_movement.y += 1.0f;

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		target_movement.y -= 1.0f;

	float speed = cfg.movement.fly_speed;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		speed *= 0.5f;

	base_movement->SetTargetMovement(target_movement * speed);

	if (target_movement.x == 0.0f && target_movement.y == 0.0f && target_movement.z == 0.0f)
		base_movement->SetTargetMovement(Vector3{ 0, 0, 0 });
}

inline void HandleHighJump()
{
	if (!players.local_player) return;

	if (fly_active) return;

	auto* base_movement = players.local_player->GetBaseMovement();
	if (!base_movement) return;

	base_movement->SetGroundAngle(cfg.movement.highjump_angle);
}

void Misc()
{
	while (true)
	{
			if (!players.local_player)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			if (cfg.sky.tod_sky)
			{
				set_time_of_day(cfg.sky.time_of_day);
				set_night_brightness(cfg.sky.night_brightness);
			}

			if (cfg.settings.zoom)
			{
				bool zoom_key_held = (GetAsyncKeyState(cfg.settings.zoom_keybind) & 0x8000) != 0;
				set_game_fov(zoom_key_held ? 20.0f : 90.0f);
			}
			else if (cfg.settings.fov_changer)
				set_game_fov(cfg.settings.game_fov);

			if (cfg.movement.fly)
				HandleFly();

			if (cfg.movement.highjump && !fly_active)
				HandleHighJump();

			if (cfg.movement.shoot_in_air)
				HandleShootInAir();

			if (cfg.exploits.fake_admin || cfg.exploits.thirdperson)
			{
				uintptr_t local_addr = reinterpret_cast<uintptr_t>(players.local_player);
				int playerFlags = driver->Read<int>(local_addr + Offsets::BasePlayer::playerFlags);
				if (playerFlags != 64 && playerFlags != 16)
				{
					if (cfg.exploits.fake_admin && playerFlags != 4)
						driver->Write<int>(local_addr + Offsets::BasePlayer::playerFlags, 4);
					if (cfg.exploits.thirdperson && playerFlags != 1024)
						driver->Write<int>(local_addr + Offsets::BasePlayer::playerFlags, 1024);
				}
			}

			HandleRemoveLayers();

			if (cfg.movement.speed && GetAsyncKeyState(cfg.movement.speedhack_keybind) & 0x8000)
			{
				players.local_player->Set_ClothingMoveSpeedReduction(-cfg.movement.speed_value);
			}
			else
			{
				players.local_player->Set_ClothingMoveSpeedReduction(0.0f);
			}

			Chams::HandleHandWeaponChams();

			HeldItem held_item = players.local_player->GetHeldItem();
			if (held_item.is_weapon())
			{
				if (held_item.is_weapon2())
				{
					uint64_t magazine = driver->Read<uint64_t>(held_item.base_projectile + Offsets::BaseProjectile::primaryMagazine);
					if (magazine && driver->is_valid(magazine))
					{
						ammo.capacity = driver->Read<int>(magazine + Offsets::Magazine::capacity);
						ammo.contents = driver->Read<int>(magazine + Offsets::Magazine::contents);
					}
					else
					{
						ammo.capacity = 0;
						ammo.contents = 0;
					}
				}
				else
				{
					ammo.capacity = 0;
					ammo.contents = 0;
				}

				ULONGLONG now = GetTickCount64();
				constexpr ULONGLONG rate_limit_ms = 50;

				{
					static bool was_recoil_on = false;
					static ULONGLONG last_recoil_write = 0;
					bool recoil_enabled = cfg.weapon.norecoil;
					if (recoil_enabled)
					{
						was_recoil_on = true;
						if (now - last_recoil_write >= rate_limit_ms)
						{
							last_recoil_write = now;
							float scale = cfg.weapon.recoil_scale / 100.0f;
							if (cfg.weapon.recoil_scale >= 100)
								held_item.restore_recoil();
							else
								held_item.apply_no_recoil(scale);
						}
					}
					else if (was_recoil_on)
					{
						held_item.restore_recoil();
						was_recoil_on = false;
					}
				}

				{
					static bool was_spread_on = false;
					static ULONGLONG last_spread_write = 0;
					bool spread_enabled = cfg.weapon.nospread;
					if (spread_enabled)
					{
						was_spread_on = true;
						if (now - last_spread_write >= rate_limit_ms)
						{
							last_spread_write = now;
							float scale = cfg.weapon.spread / 100.0f;
							if (cfg.weapon.spread >= 100)
								held_item.restore_spread();
							else
								held_item.apply_no_spread(scale);
						}
					}
					else if (was_spread_on)
					{
						held_item.restore_spread();
						was_spread_on = false;
					}
				}

			if (held_item.is_bow())
			{
				static bool was_pullback_on = false;
				static ULONGLONG last_pullback_write = 0;
				if (cfg.weapon.no_pullback)
				{
					was_pullback_on = true;
					if (now - last_pullback_write >= rate_limit_ms)
					{
						last_pullback_write = now;
						held_item.apply_no_pullback();
					}
				}
				else if (was_pullback_on)
				{
					held_item.restore_pullback();
					was_pullback_on = false;
				}

				{
					static bool was_fastbow_on = false;
					static ULONGLONG last_fastbow_write = 0;
					if (cfg.weapon.fast_bow)
					{
						was_fastbow_on = true;
						if (now - last_fastbow_write >= rate_limit_ms)
						{
							last_fastbow_write = now;
							held_item.apply_fast_bow();
						}
					}
					else if (was_fastbow_on)
					{
						held_item.restore_fast_bow();
						was_fastbow_on = false;
					}
				}
			}

			if (held_item.is_melee())
			{
				static bool was_longhand_on = false;
				static ULONGLONG last_longhand_write = 0;
				if (cfg.weapon.long_hand)
				{
					was_longhand_on = true;
					if (now - last_longhand_write >= rate_limit_ms)
					{
						last_longhand_write = now;
						held_item.apply_long_hand();
					}
				}
				else if (was_longhand_on)
				{
					held_item.restore_long_hand();
					was_longhand_on = false;
				}
			}

			held_item.instant_eoka();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
