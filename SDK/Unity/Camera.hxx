#pragma once

enum LayerMasks : int {
	Default = 1,
	TransparentFX = 2,
	Ignore_Raycast = 4,
	Reserved1 = 8,
	Water = 16,
	UI = 32,
	Reserved2 = 64,
	Reserved3 = 128,
	Deployed = 256,
	Ragdoll = 512,
	Invisible = 1024,
	AI = 2048,
	Player_Movement = 4096,
	Vehicle_Detailed = 8192,
	Game_Trace = 16384,
	Vehicle_World = 32768,
	World = 65536,
	Player_Server = 131072,
	Trigger = 262144,
	Player_Model_Rendering = 524288,
	Physics_Projectile = 1048576,
	Construction = 2097152,
	Construction_Socket = 4194304,
	Terrain = 8388608,
	Transparent = 16777216,
	Clutter = 33554432,
	Debris = 67108864,
	Vehicle_Large = 134217728,
	Prevent_Movement = 268435456,
	Prevent_Building = 536870912,
	Tree = 1073741824,
};

class Camera
{
public:
	std::uint64_t cam = 0;
	Matrix ViewMatrix;
	Vector3 CameraPos;

	uint64_t GetCamera()
	{
		return driver->ReadChain<uint64_t>(globals.game_assembly, { Offsets::Camera::Base, Offsets::Camera::StaticOffset, Offsets::Camera::ObjectOffset, Offsets::Camera::Offset });
	}

	bool WorldToScreen(Vector3& vIn, Vector2& vOut)
	{
		Matrix m = ViewMatrix.transpose();
		Vector3 translationVector = Vector3(m._41, m._42, m._43);
		Vector3 up = Vector3(m._21, m._22, m._23);
		Vector3 right = Vector3(m._11, m._12, m._13);

		float w = translationVector.dot_product(vIn) + m._44;

		if (w < 0.098f)
			return false;

		float y = up.dot_product(vIn) + m._24;
		float x = right.dot_product(vIn) + m._14;
		vOut = Vector2((GetSystemMetrics(SM_CXSCREEN) / 2) * (1.f + x / w), (GetSystemMetrics(SM_CYSCREEN) / 2) * (1.f - y / w));

		return true;
	}

	int GetCurrentCullingMask()
	{
		if (!cam) return 0;
		return driver->Read<int>(cam + 0x43C);
	}

	bool IsLayerVisible(LayerMasks layer)
	{
		return (GetCurrentCullingMask() & layer) != 0;
	}

	bool SetCullingMask(int mask)
	{
		if (!cam) return false;
		driver->Write<int>(cam + 0x43C, mask);
		return true;
	}

	void SetLayerVisible(LayerMasks layer, bool visible)
	{
		int mask = GetCurrentCullingMask();
		if (visible)
			mask |= layer;
		else
			mask &= ~layer;
		SetCullingMask(mask);
	}
} camera;
