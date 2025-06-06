#pragma once
#define NOMINMAX
#include <algorithm>
#undef min
#undef max
#include <assert.h>
#include <ostream>

class ENGINE_API vec3f
{
public:
	float x, y, z;
	vec3f() : x(0.0), y(0.0), z(0.0) {}
	vec3f(const vec3f& v) : x(v.x), y(v.y), z(v.z) {}
	vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3f(float* v) : x(v[0]),y(v[1]),z(v[2]) {}
	explicit vec3f(float f) : x(f), y(f), z(f) {}

	float& operator[](unsigned int i);
	float operator[](unsigned int i) const;

	vec3f& operator = (const vec3f& v);
	vec3f operator-() const;
	vec3f& operator+=(const vec3f& v);
	vec3f& operator-=(const vec3f& v);
	vec3f& operator*=(const vec3f& v);
	vec3f& operator/=(const vec3f& v);
	vec3f& operator*=(const float& f);
	vec3f& operator/= (const float& f);

	/*----------------- LOCAL -------------------------*/
	float length2() const;
	float length() const;
	float normalize();
	vec3f normalized() const;
	vec3f negative() const;
	inline float dot(const vec3f& v);
	inline vec3f cross(const vec3f& v);

	/*----------------- STATICS -----------------------*/
	static float dot(const vec3f& v1, const vec3f& v2);
	static float fabsdot(const vec3f& v1, const vec3f& v2);
	static vec3f cross(const vec3f& v1, const vec3f& v2);
	static vec3f reflect(const vec3f& dir, const vec3f& n);
	static vec3f min(const vec3f& v1, const vec3f& v2);
	static vec3f max(const vec3f& v1, const vec3f& v2);
	static vec3f lerp(const vec3f& start, const vec3f& end, float factor);

	/*-------------------------------------------------*/
	float maxComponent() const { return std::max(std::max(x, y), z); }
	float minComponent() const { return std::min(std::min(x, y), z); }

	friend std::ostream& operator<<(std::ostream& o, const vec3f& v);
};

inline float& vec3f::operator[](unsigned int i)
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}
inline float vec3f::operator[](unsigned int i) const
{
	assert(i >= 0 && i <= 2);
	return (&x)[i];
}

inline vec3f& vec3f::operator=(const vec3f& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
inline vec3f vec3f::operator-() const
{
	return vec3f(-x, -y, -z);
}

inline vec3f& vec3f::operator+=(const vec3f& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline vec3f& vec3f::operator-=(const vec3f& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline vec3f& vec3f::operator*=(const vec3f& v)
{
#ifdef USE_SSE2
	__m128 a = _mm_set_ps(0.0f, z, y, x);
	__m128 b = _mm_set_ps(0.0f, v.z, v.y, v.x);
	__m128 result = _mm_mul_ps(a, b);
	float temp[4];
	_mm_store_ps(temp, result);
	//for avoiding temporary vector, we need add padding on our class! we will handle this in future
	x = temp[0];
	y = temp[1];
	z = temp[2];
	return *this;
#else
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
#endif
}

inline vec3f& vec3f::operator/=(const vec3f& v)
{
#ifdef USE_SSE2
	__m128 a = _mm_set_ps(0.0f, z, y, x);
	__m128 b = _mm_set_ps(0.0f, v.z, v.y, v.x);
	__m128 result = _mm_div_ps(a, b);
	//for avoiding temporary vector, we need add padding on our class! we will handle this in future
	float temp[4];
	_mm_store_ps(temp, result);
	x = temp[0];
	y = temp[1];
	z = temp[2];
	return *this;
#else
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
#endif
}

inline vec3f& vec3f::operator*=(const float& f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline vec3f& vec3f::operator/=(const float& f)
{
	float inv = 1.f / f;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

//EXTERNAL OPERATOR

inline vec3f operator+(const vec3f& v1, const vec3f& v2)
{
	return vec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vec3f operator-(const vec3f& v1, const vec3f& v2)
{
	return vec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline vec3f operator*(const vec3f& v1, const vec3f& v2)
{
	return vec3f(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline vec3f operator*(const vec3f& v, float f)
{
	return vec3f(f * v.x, f * v.y, f * v.z);
}

inline vec3f operator*(float f, const vec3f& v)
{
	return vec3f(f * v.x, f * v.y, f * v.z);
}

inline vec3f operator/(const vec3f& v, float f)
{
	float inv = 1.f / f;
	return vec3f(v.x * inv, v.y * inv, v.z * inv);
}

inline vec3f operator/(float f, const vec3f& v)
{
	return vec3f(f / v.x, f / v.y, f / v.z);
}

inline vec3f operator/(const vec3f& v1, const vec3f& v2)
{
	return vec3f(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline bool operator==(const vec3f& r, const vec3f& l)
{
	return (r.x == l.x && r.y == l.y && r.z == l.z);
}

inline bool operator!=(const vec3f& r, const vec3f& l)
{
	return (r.x != l.x && r.y != l.y && r.z != l.z);
}

inline bool operator>(const vec3f& r, const vec3f& l)
{
	return (r.length2() > l.length2());
}

inline bool operator<(const vec3f& r, const vec3f& l)
{
	return (r.length2() < l.length2());
}

//FUNC

inline float vec3f::length2() const
{
	return x * x + y * y + z * z;
}

inline float vec3f::length() const
{
	return std::sqrt(length2());
}

inline float vec3f::normalize()
{
	float len = length(); *this /= len; return len;
}

inline vec3f vec3f::normalized() const
{
	vec3f nor(*this);
	nor.normalize();
	return nor;
}

inline vec3f vec3f::negative() const
{
	return vec3f(-x, -y, -z);
}

inline float vec3f::dot(const vec3f& v)
{
	return x * v.x + y * v.y + z * v.z;
}


inline vec3f vec3f::cross(const vec3f& v)
{
	return vec3f(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x);
}

//STATICS

inline float vec3f::dot(const vec3f& v1, const vec3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float vec3f::fabsdot(const vec3f& v1, const vec3f& v2)
{
	return fabs(vec3f::dot(v1, v2));
}

inline vec3f vec3f::cross(const vec3f& v1, const vec3f& v2)
{
	return vec3f(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

inline vec3f vec3f::reflect(const vec3f& dir, const vec3f& n)
{
	return dir - n * 2 * vec3f::dot(n, dir);
}

inline vec3f vec3f::min(const vec3f& v1, const vec3f& v2)
{
	return vec3f(
		std::min(v1.x, v2.x),
		std::min(v1.y, v2.y),
		std::min(v1.z, v2.z));
}

inline vec3f vec3f::max(const vec3f& v1, const vec3f& v2)
{
	return vec3f(
		std::max(v1.x, v2.x),
		std::max(v1.y, v2.y),
		std::max(v1.z, v2.z));
}

inline vec3f vec3f::lerp(const vec3f& start, const vec3f& end, float factor)
{
	return start + (end - start) * factor;
	//return (start + vec3f(factor) * (end - start));
}

inline std::ostream& operator<<(std::ostream& o, const vec3f& v)
{
	o << "vec3( " << v.x << ", " << v.y << " ," << v.z << ')';
	return o;
}
