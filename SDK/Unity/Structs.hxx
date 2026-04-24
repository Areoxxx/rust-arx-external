#pragma once
#include <mutex>

class BasePlayer;

enum class WeaponTier : int
{
	None,
	Melee,
	Bow,
	Pistol,
	SMG,
	Shotgun,
	Rifle,
	Sniper,
	LMG,
	Launcher,
	Throwable,
	Medical,
	Tool,
	Misc
};

enum class ListType : int
{
	Player,
	Resource,
	World,
	Item
};

enum class HealthStatus : int
{
	Alive,
	Wounded,
	Incapacitated,
	Dead
};

struct RustPlayer
{
	uint64_t instance;
	std::string name;
	std::string weapon;
	WeaponTier weapon_tier = WeaponTier::None;
	uint64_t team_id = 0;
	float health = 100.0f;
	float max_health = 100.0f;
	HealthStatus health_status = HealthStatus::Alive;
	bool is_local = false;
	bool is_npc = false;
	bool is_dead = false;
	bool is_sleeping = false;
	bool is_spectating = false;
	bool is_incapacitated = false;
	bool is_receiving_snapshot = false;
	std::vector<std::string> belt_items;
};

struct RustRessource
{
	uintptr_t instance = NULL;
	uintptr_t position_ptr = NULL;
	Vector3 position = {};
	std::string name = "";
	unsigned int name_hash = 0;
};

struct RustWorld
{
	uintptr_t instance = NULL;
	uintptr_t position_ptr = NULL;
	Vector3 position = {};
	std::string name = "";
	unsigned int name_hash = 0;
};

struct RustItem
{
	uintptr_t instance = NULL;
	uintptr_t position_ptr = NULL;
	Vector3 position = {};
	std::string name = "";
	unsigned int name_hash = 0;
};

struct DynamicArray
{
	uintptr_t base;
	uint32_t sz;
};

struct WorldEntity
{
	uintptr_t instance = NULL;
	Vector3 position = {};
	std::string display_name = "";
	ListType type = ListType::World;
};

struct AllList
{
	std::vector<RustPlayer> player;
	std::vector<RustRessource> ressource;
	std::vector<RustWorld> world;
	std::vector<RustItem> item;
};

struct TargetPlayerClosestToCenter
{
	BasePlayer* Player = nullptr;
	std::string Name;
};

struct Global
{
	std::vector<RustPlayer> all_player_list = {};
	std::vector<RustPlayer> player_list = {};
	std::vector<RustRessource> resource_list = {};
	std::vector<RustWorld> world_list = {};
	std::vector<RustItem> item_list = {};
	std::vector<WorldEntity> world_entity_list = {};

	RustRessource target_resource = {};
	bool find_target = true;
	int ticks = 0;
} global;

inline TargetPlayerClosestToCenter g_target_player_center;

inline std::mutex g_global_mutex;

struct Ammo
{
	int capacity;
	int contents;
}ammo;

struct Bullet
{
	Vector3 bullet_pointer{};
	Vector2 bullet_pos{};
};
inline std::vector<Bullet> BulletList;

Vector3 head3d;
Vector2 head2d;

Vector3 lfoot3d;
Vector2 lfoot2d;

Vector3 rfoot3d;
Vector2 rfoot2d;

enum BoneList : int
{
	pelvis = 0,
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	penis = 6,
	GenitalCensor = 7,
	GenitalCensor_LOD0 = 8,
	Inner_LOD0 = 9,
	GenitalCensor_LOD1 = 11,
	GenitalCensor_LOD2 = 12,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	r_ankle_scale = 17,
	spine1 = 18,
	spine1_scale = 19,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_clavicle = 23,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	l_index1 = 27,
	l_index2 = 28,
	l_index3 = 29,
	l_little1 = 30,
	l_little2 = 31,
	l_little3 = 32,
	l_middle1 = 33,
	l_middle2 = 34,
	l_middle3 = 35,
	l_prop = 36,
	l_ring1 = 37,
	l_ring2 = 38,
	l_ring3 = 39,
	l_thumb1 = 40,
	l_thumb2 = 41,
	l_thumb3 = 42,
	IKtarget_righthand_min = 43,
	IKtarget_righthand_max = 44,
	l_ulna = 45,
	neck = 46,
	head = 47,
	jaw = 48,
	eyeTranform = 49,
	l_eye = 50,
	l_Eyelid = 51,
	r_eye = 52,
	r_Eyelid = 53,
	r_clavicle = 54,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
	r_index1 = 58,
	r_index2 = 59,
	r_index3 = 60,
	r_little1 = 61,
	r_little2 = 62,
	r_little3 = 63,
	r_middle2 = 65,
	r_middle3 = 66,
	r_prop = 67,
	r_ring1 = 68,
	r_ring2 = 69,
	r_ring3 = 70,
	r_thumb1 = 71,
	r_thumb2 = 72,
	r_thumb3 = 73,
	IKtarget_lefthand_min = 74,
	IKtarget_lefthand_max = 75,
	r_ulna = 76,
	l_breast = 77,
	r_breast = 78,
	BoobCensor = 79,
	BreastCensor_LOD0 = 80,
	BreastCensor_LOD1 = 83,
	BreastCensor_LOD2 = 84
};
