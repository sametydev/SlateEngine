#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>
#include <vector>
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

    inline void AddShader(IShader* shader) {
        shaders.push_back(shader);
    }
    inline void BindPipeline() {

    }
    inline MaterialData& GetMaterialData() {
        return matData;
    }

private:
    MaterialData matData;
protected:
    std::vector<IShader*> shaders;

};

