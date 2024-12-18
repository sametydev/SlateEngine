#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>

struct ENGINE_API vec2f
{
	vec2f() : x(0.0f), y(0.0f) {}
	vec2f(float x) : x(x), y(x) {}
	vec2f(float x, float y) : x(x), y(y) {}

	vec2f& operator+=(const vec2f& v);
	vec2f& operator-=(const vec2f& v);
	vec2f& operator*=(const vec2f& v);
	vec2f& operator/=(const vec2f& v);
	bool operator==(const vec2f& v);
	bool operator!=(const vec2f& v);



	float lenght2() const;
	float lenght() const;
	float dot(const vec2f& v);
	float x, y;

	static float dot(const vec2f& v1, const vec2f& v2);
};

inline vec2f& vec2f::operator+=(const vec2f& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

inline vec2f& vec2f::operator-=(const vec2f& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

inline vec2f& vec2f::operator*=(const vec2f& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

inline vec2f& vec2f::operator/=(const vec2f& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

inline float vec2f::lenght2() const
{
	return (x * x) + (y * y);
}

inline float vec2f::lenght() const
{
	return sqrtf(lenght2()); // == |A|
}

inline float vec2f::dot(const vec2f& v)
{
	return x * v.x + y * v.y;
}

inline float vec2f::dot(const vec2f& v1, const vec2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
inline vec2f operator-(const vec2f& v1, const vec2f& v2) {
	return vec2f(v1.x - v2.x, v1.y - v2.y);
}

inline bool vec2f::operator==(const vec2f& v) {
	return (x == v.x && y == v.y);
}

inline bool vec2f::operator!=(const vec2f& v) {
	return !(*this == v);
}