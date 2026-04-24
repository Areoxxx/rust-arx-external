#pragma once
class PlayerModel
{
public:
	auto GetPosition()
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return Vector3(0, 0, 0);
		auto pos = driver->Read<Vector3>(reinterpret_cast<uintptr_t>(this) + Offsets::PlayerModel::position);
		if (!pos.IsValid() || !std::isfinite(pos.x) || !std::isfinite(pos.y) || !std::isfinite(pos.z)) {
			return Vector3(-1.0f, -1.0f, -1.0f);
		}
		return pos;
	}
};