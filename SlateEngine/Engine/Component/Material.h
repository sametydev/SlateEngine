#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
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
        shaders.emplace_back(shader);
    }

    inline void AddTexture(DXTexture* tex) {
        textures.emplace_back(tex);
    }

    inline void BindPipeline() {

    }
    inline MaterialData& GetMaterialData() {
        return matData;
    }

    void OnRender();
    void OnUpdate();

private:
    MaterialData matData;
protected:
    std::vector<IShader*> shaders;
    std::vector<DXTexture*> textures;

};

