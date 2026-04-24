#pragma once
#include <math.h>
#include <cmath>

#define M_PI 3.14159265358979323846f
#define M_RADPI 57.295779513082f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))

struct Vector2
{
	float x, y;

	inline Vector2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
	inline Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
	inline Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
	inline Vector2 operator*(const Vector2& other) const { return { x * other.x, y * other.y }; }
	inline Vector2 operator/(const Vector2& other) const { return { x / other.x, y / other.y }; }
	inline Vector2 operator-() const { return { -x, -y }; }

	inline Vector2 normalized() const
	{
		float len = sqrtf(x * x + y * y);
		return { x / len, y / len };
	}

	inline float Length() {
		return std::sqrt(x * x + y * y);
	}

	inline Vector2 Normalize() {
		return Vector2(this->x / Length(), this->y / Length());
	}

	inline bool Empty() {
		return x == 0 && y == 0;
	}

	float Distance(Vector2 b)
	{
		return sqrt(pow(b.x - x, 2) + pow(b.y - y, 2));
	}

	inline bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}
};

struct Vector3
{
	float x, y, z, w;

	inline Vector3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	inline Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
	inline Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
	inline Vector3 operator*(const Vector3& other) const { return { x * other.x, y * other.y, z * other.z }; }
	inline Vector3 operator/(const Vector3& other) const { return { x / other.x, y / other.y, z / other.z }; }
	inline Vector3 operator-() const { return { -x, -y, -z }; }
	inline Vector3& operator+=(const Vector3& other) { return *this = *this + other; }
	inline Vector3& operator-=(const Vector3& other) { return *this = *this - other; }

	bool operator==(const Vector3& in) const
	{
		return (x == in.x && y == in.y && z == in.z);
	}

	inline bool operator!=(const Vector3& in) const
	{
		return (x != in.x || y != in.y || z != in.z);
	}

	inline float dot_product(Vector3 input) const {
		return (x * input.x) + (y * input.y) + (z * input.z);
	}

	inline Vector3 Normalized() const
	{
		float len = sqrtf(x * x + y * y + z * z);
		return { x / len, y / len, z / len };
	}

	inline float Magnitude()
	{
		return std::sqrt((x * x) + (y * y) + (z * z));
	}

	inline Vector3 Normalize()
	{
		Vector3 out = *this;
		auto l = Magnitude();
		if (l == 0)
			return *this;
		out.x /= l;
		out.y /= l;
		out.z /= l;
		return out;
	}

	inline float Dot(const Vector3& Vec2)
	{
		return this->x * Vec2.x + this->y * Vec2.y + this->z * Vec2.z;
	}

	static inline Vector3 Zero()
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 abs()
	{
		return Vector3(std::abs(x), std::abs(y), std::abs(z));
	}

	inline bool Empty()
	{
		return x == 0 && y == 0 && z == 0;
	}

	inline bool IsValid()
	{
		return x && y;
	}

	inline float Distance(const Vector3& vector)
	{
		return std::sqrt(
			(x - vector.x) * (x - vector.x) +
			(y - vector.y) * (y - vector.y) +
			(z - vector.z) * (z - vector.z));
	}

	inline Vector3 Cross(const Vector3& other) const
	{
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	Vector3 Multiply(const Vector3& Point)
	{
		float num = x * 2.00f;
		float num2 = y * 2.00f;
		float num3 = z * 2.00f;
		float num4 = x * num;
		float num5 = y * num2;
		float num6 = z * num3;
		float num7 = x * num2;
		float num8 = x * num3;
		float num9 = y * num3;
		float num10 = w * num;
		float num11 = w * num2;
		float num12 = w * num3;

		Vector3 result;
		result.x = (1.00f - (num5 + num6)) * Point.x +
			(num7 - num12) * Point.y + (num8 + num11) * Point.z;
		result.y = (num7 + num12) * Point.x +
			(1.00f - (num4 + num6)) * Point.y +
			(num9 - num10) * Point.z;
		result.z = (num8 - num11) * Point.x + (num9 + num10) * Point.y +
			(1.00f - (num4 + num5)) * Point.z;
		return result;
	}

	inline float length_sqr() const {
		return (x * x) + (y * y) + (z * z);
	}

	inline float length() const {
		return sqrt(length_sqr());
	}

	inline float Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	inline bool empty() { return x == 0.000000 && y == 0.000000 && z == 0.000000; }
};

struct Vector4
{
	float x, y, z, w;

	inline Vector4 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
	inline Vector4 operator+(const Vector4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
	inline Vector4 operator-(const Vector4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
	inline Vector4 operator/(const Vector4& other) const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
	inline Vector4 operator-() const { return { -x, -y, -z, -w }; }

	inline Vector4 operator*(const Vector4& other) const
	{
		return {
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y + y * other.w + z * other.x - x * other.z,
			w * other.z + z * other.w + x * other.y - y * other.x,
			w * other.w - x * other.x - y * other.y - z * other.z
		};
	}

	inline Vector3 operator*(const Vector3& vec) const
	{
		float _x = x * 2.0f;
		float _y = y * 2.0f;
		float _z = z * 2.0f;
		float xx = x * _x;
		float yy = y * _y;
		float zz = z * _z;
		float xy = x * _y;
		float xz = x * _z;
		float yz = y * _z;
		float wx = w * _x;
		float wy = w * _y;
		float wz = w * _z;

		Vector3 result;
		result.x = (1.0f - (yy + zz)) * vec.x + (xy - wz) * vec.y + (xz + wy) * vec.z;
		result.y = (xy + wz) * vec.x + (1.0f - (xx + zz)) * vec.y + (yz - wx) * vec.z;
		result.z = (xz - wy) * vec.x + (yz + wx) * vec.y + (1.0f - (xx + yy)) * vec.z;

		return result;
	}

	static inline Vector4 Euler(const Vector3& euler)
	{
		float cx = cosf(euler.x * 0.5f);
		float sx = sinf(euler.x * 0.5f);
		float cy = cosf(euler.y * 0.5f);
		float sy = sinf(euler.y * 0.5f);
		float cz = cosf(euler.z * 0.5f);
		float sz = sinf(euler.z * 0.5f);

		Vector4 q;
		q.w = cx * cy * cz + sx * sy * sz;
		q.x = sx * cy * cz - cx * sy * sz;
		q.y = cx * sy * cz + sx * cy * sz;
		q.z = cx * cy * sz - sx * sy * cz;
		return q;
	}

	static inline Vector4 FromEuler(const Vector3& euler)
	{
		float cx = cosf(euler.x * 0.5f);
		float sx = sinf(euler.x * 0.5f);
		float cy = cosf(euler.y * 0.5f);
		float sy = sinf(euler.y * 0.5f);
		float cz = cosf(euler.z * 0.5f);
		float sz = sinf(euler.z * 0.5f);

		Vector4 q;
		q.w = cx * cy * cz + sx * sy * sz;
		q.x = sx * cy * cz - cx * sy * sz;
		q.y = cx * sy * cz + sx * cy * sz;
		q.z = cx * cy * sz - sx * sy * cz;
		return q;
	}

	static inline Vector4 LookRotation(const Vector3& forward, const Vector3& up)
	{
		Vector3 f = forward.Normalized();
		Vector3 u = up.Normalized();
		Vector3 r = u.Cross(f).Normalized();
		u = f.Cross(r);

		float m00 = r.x, m01 = u.x, m02 = f.x;
		float m10 = r.y, m11 = u.y, m12 = f.y;
		float m20 = r.z, m21 = u.z, m22 = f.z;

		float trace = m00 + m11 + m22;
		Vector4 q;

		if (trace > 0.0f)
		{
			float s = sqrtf(trace + 1.0f) * 2.0f;
			q.w = 0.25f * s;
			q.x = (m21 - m12) / s;
			q.y = (m02 - m20) / s;
			q.z = (m10 - m01) / s;
		}
		else if ((m00 > m11) && (m00 > m22))
		{
			float s = sqrtf(1.0f + m00 - m11 - m22) * 2.0f;
			q.w = (m21 - m12) / s;
			q.x = 0.25f * s;
			q.y = (m01 + m10) / s;
			q.z = (m02 + m20) / s;
		}
		else if (m11 > m22)
		{
			float s = sqrtf(1.0f + m11 - m00 - m22) * 2.0f;
			q.w = (m02 - m20) / s;
			q.x = (m01 + m10) / s;
			q.y = 0.25f * s;
			q.z = (m12 + m21) / s;
		}
		else
		{
			float s = sqrtf(1.0f + m22 - m00 - m11) * 2.0f;
			q.w = (m10 - m01) / s;
			q.x = (m02 + m20) / s;
			q.y = (m12 + m21) / s;
			q.z = 0.25f * s;
		}

		return q;
	}

	inline Vector4 conjugate() const { return { -x, -y, -z, w }; }
};

Vector2 CalculateAngle(const Vector3& player, const Vector3& target)
{
	Vector3 direction = Vector3(player.x - target.x, player.y - target.y, player.z - target.z);
	return Vector2(RAD2DEG(asin(direction.y / direction.Length())), RAD2DEG(-atan2(direction.x, -direction.z)));
}

inline Vector3 CalculatePrediction(const Vector3& camPos, const Vector3& targetPos, const Vector3& targetVel, float bulletSpeed)
{
	if (bulletSpeed < 1.0f) return targetPos;
	float dx = targetPos.x - camPos.x;
	float dy = targetPos.y - camPos.y;
	float dz = targetPos.z - camPos.z;
	float dist = sqrtf(dx * dx + dy * dy + dz * dz);
	if (dist < 0.001f) return targetPos;
	float t = dist / bulletSpeed;
	return Vector3(
		targetPos.x + targetVel.x * t,
		targetPos.y + targetVel.y * t,
		targetPos.z + targetVel.z * t
	);
}

inline const double ToRad(double degree)
{
	double pi = 3.14159265359;
	return (degree * (pi / 180));
}

inline Vector4 ToQuat(Vector3 Euler) {
	float c1 = cos(ToRad(Euler.x) / 2);
	float s1 = sin(ToRad(Euler.x) / 2);
	float c2 = cos(ToRad(Euler.y) / 2);
	float s2 = sin(ToRad(Euler.y) / 2);
	float c3 = cos(ToRad(Euler.z) / 2);
	float s3 = sin(ToRad(Euler.z) / 2);
	float c1c2 = c1 * c2;
	float s1s2 = s1 * s2;
	float c1s2 = c1 * s2;
	float s1c2 = s1 * c2;
	Vector4 Quat = {
		s1c2 * c3 + c1s2 * s3,
		c1s2 * c3 - s1c2 * s3,
		c1c2 * s3 - s1s2 * c3,
		c1c2 * c3 + s1s2 * s3
	};
	return Quat;
}

Vector2 NormalizeAngle(Vector2 angle)
{
	if (angle.x > 180.0f) angle.x -= 360.0f;
	if (angle.x < -180.0f) angle.x += 360.0f;
	if (angle.y > 180.0f) angle.y -= 360.0f;
	if (angle.y < -180.0f) angle.y += 360.0f;

	return angle;
}

Vector3 quaternion_mult(const Vector3* point, Vector4* quat)
{
	float num = quat->x * 2.f;
	float num2 = quat->y * 2.f;
	float num3 = quat->z * 2.f;
	float num4 = quat->x * num;
	float num5 = quat->y * num2;
	float num6 = quat->z * num3;
	float num7 = quat->x * num2;
	float num8 = quat->x * num3;
	float num9 = quat->y * num3;
	float num10 = quat->w * num;
	float num11 = quat->w * num2;
	float num12 = quat->w * num3;
	Vector3 result{};
	result.x = (1.f - (num5 + num6)) * point->x + (num7 - num12) * point->y + (num8 + num11) * point->z;
	result.y = (num7 + num12) * point->x + (1.f - (num4 + num6)) * point->y + (num9 - num10) * point->z;
	result.z = (num8 - num11) * point->x + (num9 + num10) * point->y + (1.f - (num4 + num5)) * point->z;
	return result;
}

Vector2 BezierCurve(Vector2 start, Vector2 control, Vector2 end, float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;

	Vector2 result = start * uu + control * (2.0f * u * t) + end * tt;
	return result;
}

inline Vector3 QuatToEuler(Vector4 q) {
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	float pitch = atan2f(sinr_cosp, cosr_cosp) * (180.0f / 3.14159265358979323846f);

	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	float yaw;
	if (fabsf(sinp) >= 1.0f)
		yaw = copysignf(90.0f, sinp);
	else
		yaw = asinf(sinp) * (180.0f / 3.14159265358979323846f);

	float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	float roll = atan2f(siny_cosp, cosy_cosp) * (180.0f / 3.14159265358979323846f);

	return Vector3(pitch, yaw, roll);
}

class Matrix
{
public:
	inline float* operator[](int i) {
		return m[i];
	}

	inline const float* operator[](int i) const {
		return m[i];
	}

	inline float* Base() {
		return &m[0][0];
	}

	inline const float* Base() const {
		return &m[0][0];
	}

	static Matrix identityMatrix()
	{
		return
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Vector3 MultiplyPoint3x4(Vector3 point)
	{
		Vector3 result;
		result.x = m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + m[0][3];
		result.y = m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + m[1][3];
		result.z = m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + m[2][3];
		return result;
	}

	inline Matrix() {
		Init(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		);
	}

	inline Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		Init(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		);
	}

	inline void Init(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	) {
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;
		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	Matrix transpose() const {
		return Matrix(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
