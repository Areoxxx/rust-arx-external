#pragma once

struct S_Aimbot {
	bool enable = false;
	bool showfov = false;
	bool silentaim = false;
	bool prediction = false;
	int hitchance = 100;
	float fov = 100.0f;
	float smoothness = 5.0f;
	bool smooth = true;
	float fov_color[4]{ 1.f, 1.f, 1.f, 1.f };
	int selected_bone = 0;
	int keybind = 0x06;
	int keybind_mode = 0;
	float max_distance = 100.0f;
	bool show_aim_line = false;
	float aim_line_color[4]{ 1.f, 0.f, 0.f, 1.f };
};

struct S_PlayerVisuals {
	bool box = false;
	int box_type = 0;
	float box_color[4]{ 1.f, 1.f, 1.f, 1.f };
	bool show_name = false;
	bool show_distance = false;
	bool show_team_id = false;
	bool show_health = false;
	float text_color[4]{ 1.f, 1.f, 1.f, 1.f };
	float distance_color[4]{ 1.f, 1.f, 1.f, 1.f };
	bool weapon = false;
	float weapon_color[4]{ 1.f, 1.f, 1.f, 1.f };
	bool bone_esp = false;
	float bone_esp_color[4]{ 1.f, 1.f, 1.f, 1.f };
	bool ignore_sleepers = false;
	bool ignore_npcs = false;
	bool ignore_teammates = false;
	bool ignore_friends = false;
	float max_distance = 100.0f;
	int esp_font = 0;
	float esp_font_size = 16.0f;
	bool clothing_bar = false;
	bool itemlist = false;
	float clothing_bar_pos[2]{ -1.f, -1.f };
	float itemlist_pos[2]{ -1.f, -1.f };
	bool radar = false;
	float radar_range = 150.0f;
	float radar_size = 120.0f;
	float radar_zoom = 1.0f;
	bool remove_layer = false;
	bool remove_layer_active = false;
	int remove_layer_keybind = 0x74;
	bool remove_construction = true;
	bool remove_terrain = true;
	bool remove_tree = true;
	bool remove_clutter = true;
	bool remove_debris = true;
	bool remove_deployed = false;
	bool remove_transparent = false;
	bool remove_vehicle_large = false;
	bool remove_prevent_building = false;
};

struct S_Weapon {
	bool eoka = false;
	bool reloadbar = false;
	bool ammocounter = false;
	bool norecoil = false;
	int recoil_scale = 0;
	bool nospread = false;
	int spread = 100;
	bool no_pullback = false;
	bool long_hand = false;
	bool fast_bow = false;
	bool hand_chams = false;
	int hands_type = 0;
	bool weapon_chams = false;
	int weapon_type = 0;
	bool player_chams = false;
	int player_chams_type = 0;
};

struct S_WorldESP {
	bool elite_crate = false;
	bool military_crate = false;
	bool crate = false;
	bool airdrop = false;
	bool locked_crate = false;
	bool auto_turret = false;
	bool flame_turret = false;
	bool shotgun_trap = false;
	bool corpse = false;
	bool caves = false;
	bool stone = false;
	bool metal = false;
	bool sulfur = false;
	bool hemp = false;
	bool animals = false;
	float animals_max_distance = 150.0f;
	bool dropped_items = false;
	float dropped_items_color[4]{ 0.4f, 0.8f, 1.0f, 1.0f };
	float dropped_items_max_distance = 50.0f;
	bool draw_name = true;
	bool draw_distance = true;
	float max_distance = 200.0f;
	float color[4]{ 1.f, 1.f, 1.f, 1.f };
};

struct S_Sky {
	bool tod_sky = false;
	float time_of_day = 12.0f;
	float night_brightness = 1.0f;
	bool custom_sky_color = false;
	float sky_color[4]{ 0.5f, 0.7f, 1.0f, 1.0f };
	bool custom_sky_image = false;
	bool custom_ambient = false;
	float ambient_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
	bool custom_fog = false;
	float fogginess = 0.0f;
	bool bright_night = false;
	bool custom_stars = false;
	float star_size = 1.0f;
	float star_brightness = 1.0f;
};

struct S_Exploits {
	bool fake_admin = false;
	bool thirdperson = false;
};

struct S_Settings {
	bool crosshair = false;
	bool fov_changer = false;
	float game_fov = 90.0f;
	bool zoom = false;
	int zoom_keybind = 0x05;
	int fps_limit = 300;
	bool watermark = false;
	bool fps_counter = true;
	bool streamproof = false;
	float gui_color[4]{ 0.47f, 0.37f, 1.0f, 1.0f };
};

struct S_Movement {
	bool fly = false;
	int fly_keybind = 0x59;
	float fly_speed = 5.0f;
	bool fly_safe_mode = true;
	bool highjump = false;
	float highjump_angle = 45.0f;
	bool shoot_in_air = false;
	bool speed = false;
	int speedhack_keybind = 0x73;
	float speed_value = 10.0f;
	bool freecam = false;
	int freecam_keybind = 0x59;
	float freecam_speed = 5.0f;
	float freecam_sensitivity = 0.15f;
};

struct Config {
	S_Aimbot aimbot;
	S_PlayerVisuals player_visuals;
	S_Weapon weapon;
	S_WorldESP world_esp;
	S_Sky sky;
	S_Movement movement;
	S_Exploits exploits;
	S_Settings settings;
};

extern Config cfg;

void SaveConfig(const char* config_name);
void LoadConfig(const char* config_name);
