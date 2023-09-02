#pragma once
#include <SlateEngine/Engine/Graphics/Shader/DXShader.h>

struct MaterialData
{
    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec4f reflect;
};

struct MaterialComponent {
public:
    MaterialComponent() = default;

    inline void SetShader(DXShader* shader) {
        currentShader = shader;
    }
    inline void BindPipeline() {
        currentShader->Bind();
    }
    inline MaterialData& GetMaterialData() {
        return matData;
    }

private:
    MaterialData matData;
    DXShader* currentShader = nullptr;
};

