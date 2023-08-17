#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>

struct PointLight
{
    PointLight() = default;

    PointLight(const PointLight&) = default;
    PointLight& operator=(const PointLight&) = default;

    PointLight(PointLight&&) = default;
    PointLight& operator=(PointLight&&) = default;

    PointLight(const vec4f& _ambient, const vec4f& _diffuse, const vec4f& _specular,
        const vec3f& _position, float _range, const vec3f& _att) :
        ambient(_ambient), diffuse(_diffuse), specular(_specular), position(_position), range(_range), attenutation(_att), dummy() {}


    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec3f position;
    float range;
    vec3f attenutation;
    float dummy;
};