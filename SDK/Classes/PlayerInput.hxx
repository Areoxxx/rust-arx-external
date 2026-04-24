class PlayerInput
{
public:
	void SetViewAngles(Vector2 angles)
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return;
		driver->Write<Vector2>(reinterpret_cast<uintptr_t>(this) + 0x44, angles);
	}
	auto GetViewAngles() -> Vector2
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return { 0,0 };
		return driver->Read<Vector2>(reinterpret_cast<uintptr_t>(this) + 0x44);
	}

	bool GetAutoRun()
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return false;
		return driver->Read<bool>(reinterpret_cast<uintptr_t>(this) + 0xA4);
	}
	void SetAutoRun(bool value)
	{
		if (!Utils::ValidPointer(reinterpret_cast<uintptr_t>(this))) return;
		driver->Write<bool>(reinterpret_cast<uintptr_t>(this) + 0xA4, value);
	}
};