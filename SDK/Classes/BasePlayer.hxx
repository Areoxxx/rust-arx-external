#pragma once
#include <optional>
#include "SDK/Unity/Transform.hxx"

enum PlayerFlags : int {
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	VoiceMuted = 512,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Incapacitated = 524288,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
	ModifyClan = 8388608,
	BuffCold = 16777216,
	BuffHot = 33554432,
	BuffTea = 67108864,
	LoadingAfterTransfer = 134217728
};

class BasePlayer
{
public:
	PlayerInput* GetPlayerInput()
	{
		return driver->Read<PlayerInput*>(Address() + Offsets::BasePlayer::playerInput);
	}

	BaseMovement* GetBaseMovement()
	{
		uintptr_t movement = driver->Read<uintptr_t>(Address() + Offsets::BasePlayer::baseMovement);
		if (!movement || !driver->is_valid(movement))
			return nullptr;
		return reinterpret_cast<BaseMovement*>(movement);
	}

	void SetOnGround(bool value)
	{
		uintptr_t model_state = driver->Read<uintptr_t>(Address() + Offsets::ModelState::Base);
		if (!model_state || !driver->is_valid(model_state))
			return;

		int flags = driver->Read<int>(model_state + Offsets::ModelState::flags);
		if (value)
			flags |= 0x2;
		else
			flags &= ~0x2;

		driver->Write<int>(model_state + Offsets::ModelState::flags, flags);
	}

	PlayerEyes* GetPlayerEyes()
	{
		uintptr_t object = driver->Read<uintptr_t>(Address() + 0x10);
		if (!object) return nullptr;
		uintptr_t unity_class = driver->Read<uintptr_t>(object + 0x30);
		if (!unity_class) return nullptr;
		return reinterpret_cast<PlayerEyes*>(unity.GetComponentByIndex(unity_class, 4));
	}

	bool IsDead()
	{
		return driver->Read<bool>(reinterpret_cast<uintptr_t>(this) + Offsets::BaseCombatEntity::lifestate);
	}

	int GetPlayerFlags()
	{
		return driver->Read<int>(Address() + Offsets::BasePlayer::playerFlags);
	}

	bool HasFlag(PlayerFlags flag)
	{
		return (GetPlayerFlags() & flag) != 0;
	}

	bool IsSleeping()
	{
		return HasFlag(PlayerFlags::Sleeping);
	}

	bool IsWounded()
	{
		return HasFlag(PlayerFlags::Wounded);
	}

	bool IsSpectating()
	{
		return HasFlag(PlayerFlags::Spectating);
	}

	bool IsConnected()
	{
		return HasFlag(PlayerFlags::Connected);
	}

	bool IsIncapacitated()
	{
		return HasFlag(PlayerFlags::Incapacitated);
	}

	uint64_t GetCurrentTeam()
	{
		return driver->Read<uint64_t>(Address() + Offsets::BasePlayer::currentTeam);
	}

	float GetHealth()
	{
		return driver->Read<float>(Address() + Offsets::BaseCombatEntity::_health);
	}

	float GetMaxHealth()
	{
		return driver->Read<float>(Address() + Offsets::BaseCombatEntity::_maxHealth);
	}

	HealthStatus GetHealthStatus()
	{
		if (IsDead())
			return HealthStatus::Dead;
		if (HasFlag(PlayerFlags::Incapacitated))
			return HealthStatus::Incapacitated;
		if (HasFlag(PlayerFlags::Wounded))
			return HealthStatus::Wounded;
		return HealthStatus::Alive;
	}

	void Set_ClothingMoveSpeedReduction(float value)
	{
		driver->Write<float>(reinterpret_cast<uintptr_t>(this) + Offsets::BasePlayer::clothingMoveSpeedReduction, value);
	}

	std::string GetName()
	{
		uint64_t pointer = driver->Read<uint64_t>(Address() + Offsets::BasePlayer::_displayName);
		if (!pointer)
			return {};

		int length = driver->Read<int>(pointer + 0x10);
		if (length <= 0)
			return {};

		std::wstring wide = driver->ReadWString(pointer + 0x14, length);
		if (wide.empty())
			return {};

		wide.erase(length);
		return { wide.begin(), wide.end() };
	}

	Transform* GetTransform(int bone)
	{
		uint64_t player_model = driver->Read<uint64_t>(Address() + Offsets::BaseEntity::model);
		if (!player_model) return nullptr;

		uint64_t bone_transforms = driver->Read<uint64_t>(player_model + Offsets::BasePlayer::boneTransforms);
		if (!bone_transforms) return nullptr;

		uint64_t bone_value = driver->Read<uint64_t>(bone_transforms + 0x20 + (bone * 0x8));
		if (!bone_value) return nullptr;

		auto result = driver->Read<uint64_t>(bone_value + 0x10);
		if (!result || result < 0x10000 || result > 0x7FFFFFFFFFFF)
			return nullptr;
		return reinterpret_cast<Transform*>(result);
	}

	Vector3 GetBone(int bone_index)
	{
		Transform* transform = GetTransform(bone_index);
		if (!transform)
			return {};

		Vector3 position = transform->GetBonePosition();
		return position.empty() ? Vector3{} : position;
	}

	Vector3 GetVelocity()
	{
		auto* movement = GetBaseMovement();
		if (!movement || !driver->is_valid(reinterpret_cast<uintptr_t>(movement)))
			return {};
		uintptr_t addr = reinterpret_cast<uintptr_t>(movement);
		Vector3 v = driver->Read<Vector3>(addr + Offsets::BaseMovement::velocity);
		return v;
	}

	HeldItem GetHeldItem()
	{
		uintptr_t addr = Address();
		if (!addr || !driver->is_valid(addr))
			return {};

		const uint64_t encrypted_active = driver->Read<uint64_t>(addr + Offsets::BasePlayer::clActiveItem);
		const uint32_t active_id = static_cast<uint32_t>(decryption.decrypt_ClActive_data(encrypted_active));

		for (const auto& item : GetBeltEntities())
		{
			if (!item || !driver->is_valid(item))
				continue;

			if (driver->Read<uint32_t>(item + Offsets::Item::uid) == active_id)
				return HeldItem(item);
		}
		return {};
	}

	std::optional<std::vector<std::string>> Get_BeltContents()
	{
		std::vector<std::string> items;
		uintptr_t contents = GetContainerContents(Offsets::PlayerInventory::containerBelt);
		if (!contents)
			return std::nullopt;

		for (int i = 0; i < 6; i++)
		{
			uint64_t current = driver->Read<uint64_t>(contents + 0x20 + (i * 8));
			if (!current || !driver->is_valid(current))
			{
				items.push_back("Empty");
				continue;
			}
			items.push_back(GetItemShortName(current));
		}
		return items;
	}

	std::optional<std::vector<std::string>> Get_ClothingBeltContents()
	{
		std::vector<std::string> items;
		uintptr_t contents = GetContainerContents(Offsets::PlayerInventory::containerWear);
		if (!contents)
			return std::nullopt;

		for (int i = 0; i < 8; i++)
		{
			uint64_t current = driver->Read<uint64_t>(contents + 0x20 + (i * 8));
			if (!current || !driver->is_valid(current))
			{
				items.push_back("Empty");
				continue;
			}
			items.push_back(GetItemShortName(current));
		}
		return items;
	}

	std::vector<uint64_t> GetBeltEntities()
	{
		static constexpr int kBeltSlots = 6;
		std::vector<uint64_t> result;
		result.reserve(kBeltSlots);

		uintptr_t contents = GetContainerContents(Offsets::PlayerInventory::containerBelt);
		if (!contents) return result;

		for (int i = 0; i < kBeltSlots; ++i)
		{
			uint64_t item = driver->Read<uint64_t>(contents + 0x20 + (i * 0x8));
			if (item && driver->is_valid(item))
				result.push_back(item);
		}
		return result;
	}

	std::string GetItemShortName(uint64_t item)
	{
		if (!item || !driver->is_valid(item))
			return "Empty";

		uintptr_t item_def = driver->Read<uintptr_t>(item + Offsets::Item::ItemDefinition);
		if (!item_def || !driver->is_valid(item_def))
			return "Empty";

		uintptr_t short_name_ptr = driver->Read<uintptr_t>(item_def + Offsets::ItemDefinition::shortname);
		if (!short_name_ptr || !driver->is_valid(short_name_ptr))
			return "Empty";

		int length = driver->Read<int>(short_name_ptr + 0x10);
		if (length <= 0 || length > 128)
			return "Empty";

		std::wstring wide = driver->ReadWString(short_name_ptr + 0x14, length);
		if (wide.empty())
			return "Empty";

		if (static_cast<int>(wide.size()) > length)
			wide.erase(length);

		return { wide.begin(), wide.end() };
	}

private:
	uintptr_t Address() const
	{
		return reinterpret_cast<uintptr_t>(this);
	}

	uintptr_t GetContainerContents(uint64_t container_offset)
	{
		uintptr_t object = driver->Read<uintptr_t>(Address() + 0x10);
		if (!object) return 0;

		uintptr_t unity_class = driver->Read<uintptr_t>(object + 0x30);
		if (!unity_class) return 0;

		uintptr_t inventory = unity.GetComponentByIndex(unity_class, 3);
		if (!inventory) return 0;

		uint64_t container = driver->Read<uint64_t>(inventory + container_offset);
		if (!container) return 0;

		uint64_t item_list = driver->Read<uint64_t>(container + Offsets::ItemContainer::itemList);
		if (!item_list) return 0;

		return driver->Read<uint64_t>(item_list + 0x10);
	}
};