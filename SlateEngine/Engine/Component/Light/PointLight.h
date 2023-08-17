#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>

struct PointLight
{
    PointLight() {
        position = vec3f(0.0f, 0.0f, -10.0f);
        ambient = vec4f(0.3f, 0.3f, 0.3f, 1.0f);
        diffuse = vec4f(0.7f, 0.7f, 0.7f, 1.0f);
        specular = vec4f(0.5f, 0.5f, 0.5f, 1.0f);
        attenutation = vec3f(0.0f, 0.1f, 0.0f);
        range = 25.0f;
    }

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