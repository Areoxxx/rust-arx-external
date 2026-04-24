#pragma once
#include <string>

class Globals
{
public:
	std::uint64_t game_assembly = 0;
	std::uint64_t unity_player = 0;
}globals;

namespace Utils
{
	inline auto ValidPointer(uintptr_t pointer) -> bool
	{
		return (pointer && pointer > 0xFFFFFF && pointer < 0x7FFFFFFFFFFF);
	}

	unsigned HashString(const std::string& str)
	{
		unsigned int hash = 1315423911;

		for (std::size_t i = 0; i < str.length(); i++)
		{
			hash ^= ((hash << 5) + str[i] + (hash >> 2));
		}
		return (hash & 0x7FFFFFFF);
	}
}
