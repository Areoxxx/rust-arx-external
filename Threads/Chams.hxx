#pragma once
#include <vector>
#include <string>

namespace Chams {

	struct DynamicArray {
		uint64_t base;
		uint64_t mem_id;
		uint64_t sz;
		uint64_t cap;
	};

	inline std::string read_il2cpp_string(uintptr_t string_ptr) {
		if (!string_ptr || !driver->is_valid(string_ptr))
			return {};
		int length = driver->Read<int>(string_ptr + 0x10);
		if (length <= 0 || length > 256)
			return {};
		std::wstring wide = driver->ReadWString(string_ptr + 0x14, static_cast<uint32_t>(length));
		return std::string(wide.begin(), wide.end());
	}

	inline void GetComponentsInChildren(uintptr_t game_object, std::vector<uintptr_t>& render_weapon, std::vector<uintptr_t>& render_hand) {
		if (!game_object || !driver->is_valid(game_object))
			return;

		uint64_t component_list = driver->Read<uint64_t>(game_object + 0x30);
		int component_size = driver->Read<int>(game_object + 0x40);
		if (component_size < 0 || component_size > 128)
			return;

		uintptr_t go_name_ptr = driver->Read<uintptr_t>(game_object + 0x60);
		std::string game_object_name = read_il2cpp_string(go_name_ptr);

		for (int j = 0; j < component_size; ++j) {
			uintptr_t component = driver->Read<uint64_t>(component_list + (0x10 * j + 0x8));
			if (!component || !driver->is_valid(component))
				continue;

			uint64_t component_inst = driver->Read<uint64_t>(component + 0x28);
			if (!component_inst || !driver->is_valid(component_inst))
				continue;
			uint64_t component_object = driver->Read<uint64_t>(component_inst + 0x0);
			if (!component_object || !driver->is_valid(component_object))
				continue;
			uint64_t component_name_ptr = driver->Read<uint64_t>(component_object + 0x10);
			std::string name = read_il2cpp_string(component_name_ptr);

			if (name == "SkinnedMeshRenderer") {
				bool is_hand = (game_object_name.find("hand") != std::string::npos);
				if (is_hand)
					render_hand.push_back(component);
				else
					render_weapon.push_back(component);
			}
			if (name == "Transform") {
				uintptr_t child_list = driver->Read<uint64_t>(component + 0x70);
				int child_size = driver->Read<int>(component + 0x80);
				if (child_size < 0 || child_size > 128)
					continue;
				for (int i = 0; i < child_size; ++i) {
					uint64_t child_transform = driver->Read<uint64_t>(child_list + (0x8 * i));
					if (!child_transform || !driver->is_valid(child_transform))
						continue;
					uint64_t child_game_object = driver->Read<uint64_t>(child_transform + 0x30);
					if (!child_game_object || !driver->is_valid(child_game_object))
						continue;
					GetComponentsInChildren(static_cast<uintptr_t>(child_game_object), render_weapon, render_hand);
				}
			}
		}
	}

	inline uint32_t get_material_id_for_type(int cham_type) {
		switch (cham_type) {
		case 0: return 794440u;
		case 1: return 1348630u;
		case 2: return 291414u;
		case 3: return 291342u;
		default: return 794440u;
		}
	}

	inline void ProcessSkinnedMeshRenderer(uintptr_t renderer, uint32_t weaponmaterial) {
		if (!renderer || !driver->is_valid(renderer))
			return;

		for (uint32_t idx = 0; idx < 2; idx++) {
			uintptr_t render_entry = driver->Read<uintptr_t>(renderer + 0x20 + (idx * 0x8));
			if (!render_entry || !driver->is_valid(render_entry))
				continue;

			uintptr_t unity_object = driver->Read<uintptr_t>(render_entry + 0x10);
			if (!unity_object || !driver->is_valid(unity_object))
				continue;

			DynamicArray mat_list = driver->Read<DynamicArray>(unity_object + Offsets::PlayerModel::MaterialList);
			if (mat_list.sz < 1 || mat_list.sz > 5 || !mat_list.base || !driver->is_valid(mat_list.base))
				continue;

			for (uint32_t i = 0; i < mat_list.sz; i++)
				driver->Write<uint32_t>(mat_list.base + (i * 0x4), weaponmaterial);
		}
	}

	inline void HandleHandWeaponChams() {
		if (!players.local_player)
			return;
		HeldItem held = players.local_player->GetHeldItem();
		if (!held.base_projectile || !driver->is_valid(held.base_projectile))
			return;

		uintptr_t base_projectile = held.base_projectile;
		uintptr_t viewmodel = driver->Read<uintptr_t>(base_projectile + Offsets::HeldEntity::viewModel);
		if (!viewmodel || !driver->is_valid(viewmodel))
			return;

		uintptr_t viewmodel_instance = driver->Read<uintptr_t>(viewmodel + 0x28);
		if (!viewmodel_instance || !driver->is_valid(viewmodel_instance))
			return;

		uintptr_t base_viewmodel = driver->Read<uintptr_t>(viewmodel_instance + 0x10);
		if (!base_viewmodel || !driver->is_valid(base_viewmodel))
			return;

		driver->Write<bool>(viewmodel_instance + 0x40, false);

		uint64_t view_model_object = driver->Read<uint64_t>(base_viewmodel + 0x30);
		if (!view_model_object || !driver->is_valid(view_model_object))
			return;

		std::vector<uintptr_t> render_weapon;
		std::vector<uintptr_t> render_hand;
		GetComponentsInChildren(static_cast<uintptr_t>(view_model_object), render_weapon, render_hand);

		if (cfg.weapon.hand_chams) {
			uint32_t hand_mat = get_material_id_for_type(cfg.weapon.hands_type);
			for (uintptr_t r : render_hand)
				ProcessSkinnedMeshRenderer(r, hand_mat);
		}
		if (cfg.weapon.weapon_chams) {
			uint32_t weapon_mat = get_material_id_for_type(cfg.weapon.weapon_type);
			for (uintptr_t r : render_weapon)
				ProcessSkinnedMeshRenderer(r, weapon_mat);
		}
	}
}
