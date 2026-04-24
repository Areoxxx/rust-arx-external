#pragma once
class PlayerEyes
{
public:
	auto SetBodyRotation(Vector4 angles) -> Vector4
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return Vector4(0, 0, 0, 0);
		driver->Write<Vector4>(reinterpret_cast<uintptr_t>(this) + 0x50, angles);
	}
	auto GetBodyRotation() -> Vector3
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return Vector3(0, 0, 0, 0);
		return driver->Read<Vector3>(reinterpret_cast<uintptr_t>(this) + 0x50);
	}
	auto GetBodyRotationFly() -> Vector4
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return Vector4{ 0, 0, 0, 1 };
		return driver->Read<Vector4>(reinterpret_cast<uintptr_t>(this) + 0x50);
	}
};