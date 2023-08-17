#pragma once
#include <SlateEngine/Engine/DXConfig.h>

struct SpotLight
{

    SpotLight() = default;

    SpotLight(const SpotLight&) = default;
    SpotLight& operator=(const SpotLight&) = default;

    SpotLight(SpotLight&&) = default;
    SpotLight& operator=(SpotLight&&) = default;

    SpotLight(const vec4f& _ambient, const vec4f& _diffuse, const vec4f& _specular,
        const vec3f& _position, float _range, const vec3f& _direction,
        float _spot, const vec3f& _att) :
        ambient(_ambient), diffuse(_diffuse), specular(_specular),
        position(_position), range(_range), direction(_direction), spot(_spot), attenutation(_att), dummy() {}

    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec3f position;
    float range;
    vec3f direction;
    float spot;
    vec3f attenutation;
    float dummy;
};