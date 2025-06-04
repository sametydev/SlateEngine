#pragma once
//FASTMATHLIBX - Samet YILMAZ

#define NOMINMAX

constexpr float M_PI = 3.14159265358979323846f;
constexpr float RADIANS = (M_PI / 180.f);
constexpr float DEGREES = (180.f / M_PI);		

#include <algorithm>
#include <iostream>

#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

//Use SSE2 instructions
#define USE_SSE2 true

//AVX coming soon!
#define USE_AVX false

#include <SlateEngine/Engine/Utils.h>

#include <SlateEngine/Engine/Math/vec2f.h>
#include <SlateEngine/Engine/Math/vec3f.h>
#include <SlateEngine/Engine/Math/vec4f.h>
#include <SlateEngine/Engine/Math/mat4x4.h>
