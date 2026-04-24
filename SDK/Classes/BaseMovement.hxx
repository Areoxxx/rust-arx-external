#pragma once

namespace MovementEncryption
{
	inline uint64_t encrypt_flying(uint32_t a1)
	{
		uint64_t v1;
		uint32_t eax = a1;
		eax = (eax << 0x04) | (eax >> 0x1C);
		eax = eax + 0xE518CC82;
		eax = (eax << 0x13) | (eax >> 0x0D);
		*(uint32_t*)&v1 = eax;
		return v1;
	}

	inline uint32_t decrypt_flying(uint64_t a1)
	{
		uint32_t eax = static_cast<uint32_t>(a1);
		eax = (eax >> 0x13) | (eax << 0x0D);
		eax = eax - 0xE518CC82;
		eax = (eax >> 0x04) | (eax << 0x1C);
		return eax;
	}

	inline uint64_t encrypt_gravity(uint32_t a1)
	{
		uint64_t v1;
		uint32_t eax = a1;
		*(uint32_t*)&v1 = eax;
		return v1;
	}

	inline uint32_t decrypt_gravity(uint64_t a1)
	{
		uint32_t eax = static_cast<uint32_t>(a1);
		return eax;
	}

	inline uint64_t encrypt_ground_angle(uint32_t a1)
	{
		uint64_t v1;
		uint32_t eax = a1;
		eax = eax + 0x92CC5E66;
		eax = (eax << 0x05) | (eax >> 0x1B);
		eax = eax + 0x68124071;
		eax = (eax << 0x0E) | (eax >> 0x12);
		eax = eax - a1;
		*(uint32_t*)&v1 = eax;
		return v1;
	}

	inline uint32_t decrypt_ground_angle(uint64_t a1)
	{
		uint32_t eax = static_cast<uint32_t>(a1);
		eax = eax + a1;
		eax = (eax >> 0x0E) | (eax << 0x12);
		eax = eax - 0x68124071;
		eax = (eax >> 0x05) | (eax << 0x1B);
		eax = eax - 0x92CC5E66;
		return eax;
	}
}

class BaseMovement
{
public:
	void SetFlying(bool value)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);
		if (!driver->is_valid(addr)) return;
		uint32_t val = value ? 1 : 0;
		driver->Write<uint32_t>(addr + Offsets::BaseMovement::Flying,
			static_cast<uint32_t>(MovementEncryption::encrypt_flying(val)));
	}

	bool GetFlying()
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);
		if (!driver->is_valid(addr)) return false;
		return MovementEncryption::decrypt_flying(
			driver->Read<uint32_t>(addr + Offsets::BaseMovement::Flying));
	}

	void SetGravityMultiplier(float value)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);
		if (!driver->is_valid(addr)) return;
		driver->Write<uint32_t>(addr + Offsets::BaseMovement::GravityMultiplier,
			static_cast<uint32_t>(MovementEncryption::encrypt_gravity(*reinterpret_cast<uint32_t*>(&value))));
	}

	void SetGroundAngle(float value)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);
		if (!driver->is_valid(addr)) return;
		driver->Write<uint32_t>(addr + Offsets::BaseMovement::GroundAngle,
			static_cast<uint32_t>(MovementEncryption::encrypt_ground_angle(*reinterpret_cast<uint32_t*>(&value))));
	}

	void SetTargetMovement(Vector3 movement)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);
		if (!driver->is_valid(addr)) return;
		driver->Write<Vector3>(addr + Offsets::BaseMovement::TargetMovement, movement);
	}
};