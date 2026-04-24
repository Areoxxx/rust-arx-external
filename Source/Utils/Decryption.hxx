#pragma once
#include "decrypts.hpp"

class Decryption
{
private:
	static inline uint32_t rotl32(uint32_t v, int n) {
		return (v << n) | (v >> (32 - n));
	}

	bool testBITD(uint32_t Value, uint32_t BitPosition) {
		return (Value & (1 << BitPosition)) != 0;
	}

	uintptr_t Il2cppGetHandle(int32_t ObjectHandleId)
	{
		const uint64_t Index = static_cast<uint64_t>(ObjectHandleId >> 3);
		const uint64_t Table = static_cast<uint64_t>((ObjectHandleId & 7) - 1);

		const uint64_t HandleBase = globals.game_assembly + Offsets::Il2CppGetHandle;
		const uint64_t TableBase = HandleBase + (Table * 0x28);

		const uint64_t ObjectArrayBase = driver->Read<uint64_t>(TableBase + 0x8) + (Index << 3);
		const uint8_t Mode = driver->Read<uint8_t>(TableBase + 0x14);

		if (Mode > 1) {
			return driver->Read<uint64_t>(ObjectArrayBase);
		}

		uint64_t Val = driver->Read<uint64_t>(ObjectArrayBase);
		Val = ~Val;
		if (Val == ~0ULL) return 0;
		return Val;
	}

public:
	uint64_t ClientEntities(uint64_t a1)
	{
		const auto value = driver->Read<uint32_t>(a1 + 0x18);
		return Il2cppGetHandle(decrypts::BaseNetworkable::decrypt_bn1(value));
	}

	uint64_t EntityList(uint64_t a1)
	{
		const auto value = driver->Read<uint32_t>(a1 + 0x18);
		return Il2cppGetHandle(decrypts::BaseNetworkable::decrypt_bn2(value));
	}

	inline uint64_t decrypt_ClActive_data(uint64_t a1)
	{
		return decrypts::BasePlayer::decrypt_clActiveItem(a1);
	}
} decryption;
