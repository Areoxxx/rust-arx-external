#pragma once
#include <array>
#include <unordered_map>
#include <vector>
#include <mutex>
#include "Utils/Maths.hxx"
#include "SDK/Unity/Structs.hxx"

static constexpr int kBoneArraySize = 58;

inline const std::vector<BoneList> kBonesToCache = {
	BoneList::pelvis, BoneList::l_foot, BoneList::l_knee, BoneList::l_hip,
	BoneList::r_foot, BoneList::r_knee, BoneList::r_hip,
	BoneList::spine1, BoneList::spine3, BoneList::neck, BoneList::head,
	BoneList::l_upperarm, BoneList::l_forearm, BoneList::l_hand,
	BoneList::r_upperarm, BoneList::r_forearm, BoneList::r_hand
};

struct PlayerBones
{
	std::array<Vector3, kBoneArraySize> bones{};
	uint64_t frame = 0;

	bool get(int bone_id, Vector3& out) const
	{
		if (static_cast<unsigned>(bone_id) >= kBoneArraySize) return false;
		const auto& v = bones[bone_id];
		if (v.x == 0.f && v.y == 0.f && v.z == 0.f) return false;
		out = v;
		return true;
	}

	void set(int bone_id, const Vector3& v)
	{
		if (static_cast<unsigned>(bone_id) < kBoneArraySize)
			bones[bone_id] = v;
	}
};

class BoneCache
{
	std::unordered_map<uintptr_t, PlayerBones> data_;
	mutable std::mutex mtx_;
	uint64_t frame_ = 0;

public:
	void tick()
	{
		std::lock_guard<std::mutex> lk(mtx_);
		frame_++;
	}

	void remove_stale(const std::vector<uintptr_t>& valid)
	{
		std::lock_guard<std::mutex> lk(mtx_);
		if (data_.empty()) return;

		std::vector<uintptr_t> to_remove;
		for (auto& [key, _] : data_)
		{
			bool found = false;
			for (auto v : valid)
				if (v == key) { found = true; break; }
			if (!found)
				to_remove.push_back(key);
		}

		for (auto key : to_remove)
			data_.erase(key);
	}

	void update(uintptr_t instance, PlayerBones src)
	{
		std::lock_guard<std::mutex> lk(mtx_);
		auto& entry = data_[instance];
		entry = src;
		entry.frame = frame_;
	}

	bool should_update(uintptr_t instance, float distance) const
	{
		std::lock_guard<std::mutex> lk(mtx_);
		auto it = data_.find(instance);
		if (it == data_.end()) return true;
		int interval = 1;
		if (distance > 80.0f)      interval = 4;
		else if (distance > 40.0f) interval = 2;
		return (frame_ - it->second.frame) >= static_cast<uint64_t>(interval);
	}

	bool get_bone(uintptr_t instance, int bone_id, Vector3& out) const
	{
		std::lock_guard<std::mutex> lk(mtx_);
		auto it = data_.find(instance);
		if (it == data_.end()) return false;
		return it->second.get(bone_id, out);
	}

	void copy_for_render(std::unordered_map<uintptr_t, PlayerBones>& out) const
	{
		std::lock_guard<std::mutex> lk(mtx_);
		out = data_;
	}
};

inline BoneCache g_bone_cache;
