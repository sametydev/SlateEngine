#pragma once
#include <SlateEngine/Engine/Math/LineerMath.h>

struct DirectionalLight
{

    DirectionalLight() {
        ambient = vec4f(0.2f, 0.2f, 0.2f, 1.0f);
        diffuse = vec4f(0.8f, 0.8f, 0.8f, 1.0f);
        specular = vec4f(0.5f, 0.5f, 0.5f, 1.0f);
    }

    DirectionalLight(const DirectionalLight&) = default;
    DirectionalLight& operator=(const DirectionalLight&) = default;

    DirectionalLight(DirectionalLight&&) = default;
    DirectionalLight& operator=(DirectionalLight&&) = default;

    DirectionalLight(const vec4f& _ambient, const vec4f& _diffuse, const vec4f& _specular,
        const vec3f& _direction) :
        ambient(_ambient), diffuse(_diffuse), specular(_specular), direction(_direction), dummy() {}

    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec3f direction;
    float dummy;
};