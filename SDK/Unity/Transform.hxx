#pragma once

class Transform
{
public:
	struct TransformAccessReadOnly
	{
		std::uint64_t data;
	};

	struct TransformData
	{
		std::uint64_t array;
		std::uint64_t indices;
	};

	struct Matrix34
	{
		BYTE translation[16];
		BYTE rotation[16];
		BYTE scale[16];
	};

	static constexpr __m128 kMulVec0 = { -2.0f,  2.0f, -2.0f, 0.0f };
	static constexpr __m128 kMulVec1 = { 2.0f, -2.0f, -2.0f, 0.0f };
	static constexpr __m128 kMulVec2 = { -2.0f, -2.0f,  2.0f, 0.0f };

	static constexpr std::uint32_t kMaxHierarchyDepth = 15;

	[[nodiscard]] Vector3 GetBonePosition() const
	{
		const auto address = reinterpret_cast<std::uintptr_t>(this);
		if (!address)
			return Vector3::Zero();
		return ResolveWorldPosition(address);
	}

	[[nodiscard]] Vector3 GetPosition(std::uintptr_t transform_address) const
	{
		if (!transform_address)
			return Vector3::Zero();
		return ResolveWorldPosition(transform_address);
	}

	[[nodiscard]] static __m128 ApplyParentTransform(std::uint64_t array, std::int32_t parent_index, __m128 child_pos)
	{
		const auto mtx = driver->Read<Matrix34>(array + 0x30 * parent_index);
		const auto* quat = reinterpret_cast<const __m128i*>(&mtx.rotation);
		const __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0x00));
		const __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0x55));
		const __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0xAA));
		const __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0x8E));
		const __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0xDB));
		const __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*quat, 0x71));
		const __m128 scaled = _mm_mul_ps(*reinterpret_cast<const __m128*>(&mtx.scale), child_pos);
		const __m128 term_z = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(xxxx, kMulVec1), zwxy), _mm_mul_ps(_mm_mul_ps(yyyy, kMulVec2), wzyw)), _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(scaled), 0xAA)));
		const __m128 term_y = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(zzzz, kMulVec2), wzyw), _mm_mul_ps(_mm_mul_ps(xxxx, kMulVec0), yxwy)), _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(scaled), 0x55)));
		const __m128 term_x = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(yyyy, kMulVec0), yxwy), _mm_mul_ps(_mm_mul_ps(zzzz, kMulVec1), zwxy)), _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(scaled), 0x00)));
		const auto* translation = reinterpret_cast<const __m128*>(&mtx.translation);
		return _mm_add_ps(_mm_add_ps(_mm_add_ps(term_z, term_y), _mm_add_ps(term_x, scaled)), *translation);
	}

private:
	[[nodiscard]] static Vector3 ResolveWorldPosition(std::uintptr_t address)
	{
		const auto access = driver->Read<TransformAccessReadOnly>(address + 0x38);
		if (!access.data) return Vector3::Zero();
		const auto index = driver->Read<std::uint32_t>(address + 0x40);
		const auto data = driver->Read<TransformData>(access.data + 0x18);

		if (!data.array || !data.indices)
			return Vector3::Zero();

		auto result = driver->Read<__m128>(data.array + 0x30 * index);
		auto hierarchy_index = driver->Read<std::int32_t>(data.indices + 0x4 * index);

		for (std::uint32_t depth = 0; hierarchy_index >= 0 && depth < kMaxHierarchyDepth; ++depth)
		{
			result = ApplyParentTransform(data.array, hierarchy_index, result);
			hierarchy_index = driver->Read<std::int32_t>(data.indices + 0x4 * hierarchy_index);
		}
		return { result.m128_f32[0], result.m128_f32[1], result.m128_f32[2] };
	}
};
