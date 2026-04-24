#pragma once

struct Unity
{
public:
	uintptr_t GetComponentByIndex(uintptr_t game_object, int index)
	{
		if (!game_object)
			return NULL;

		uintptr_t list = driver->Read<uintptr_t>(game_object + 0x30);
		uintptr_t component = driver->Read<uintptr_t>(list + (0x10 * index + 0x8));

		if (!component)
			return NULL;

		uintptr_t unk1 = driver->Read<uintptr_t>(component + 0x28);

		if (unk1)
			return unk1;

		return NULL;
	}

	static uintptr_t GetComponentFromGameObject(uint64_t game_object, const char* component_name)
	{
		uintptr_t game_object_class = driver->Read<uintptr_t>(game_object + 0x30);
		for (int i = 0; i < 20; i++)
		{
			uintptr_t component = driver->Read<uintptr_t>(game_object_class + (0x10 * i + 0x8));
			if (!component) continue;

			uintptr_t fields = driver->Read<uintptr_t>(component + 0x28);
			if (!fields) continue;

			uintptr_t name_ptr = driver->Read<uintptr_t>(fields);
			name_ptr = driver->Read<uintptr_t>(name_ptr + 0x10);
			if (!name_ptr) continue;

			auto buffer = driver->ReadString(name_ptr);

			if (strcmp(buffer.c_str(), component_name) == 0)
				return fields;
		}
		return -1;
	}
}unity;
