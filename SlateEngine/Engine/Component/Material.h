#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Component/Component.h>
#include <SlateEngine/Engine/Graphics/Shader/DXPixelShader.h>
#include <SlateEngine/Engine/Graphics/Shader/DXVertexShader.h>
#include <SlateEngine/Engine/Graphics/DXRasterizerState.h>
#include <vector>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>
#include <typeindex>

struct MaterialData
{
    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec4f reflect;
};

using MaterialProperty = std::variant
<
    int,
    uint32_t, float, 
    vec2f, vec3f, vec4f,
    mat4x4,
    std::vector<float>, 
    std::vector<vec4f>, 
    std::vector<mat4x4>,
    std::string
>;

enum MaterialPropertyType {
    SINT = 0,
    SFLOAT,
    SUINT,
    SVEC2,
    SVEC3,
    SVEC4,
    SM4X4,
    SVECTORF,
    SVECTORV4,
    SVECTORM4X4,
    SSTRING,
    SUNDEFINED
};

constexpr std::array<MaterialPropertyType, std::variant_size_v<MaterialProperty>> TYPE_LOOKUP = {
    MaterialPropertyType::SINT,       // int
    MaterialPropertyType::SUINT,      // uint32_t
    MaterialPropertyType::SFLOAT,     // float
    MaterialPropertyType::SVEC2,      // vec2f
    MaterialPropertyType::SVEC3,      // vec3f
    MaterialPropertyType::SVEC4,      // vec4f
    MaterialPropertyType::SM4X4,      // mat4x4
    MaterialPropertyType::SVECTORF,   // std::vector<float>
    MaterialPropertyType::SVECTORV4,  // std::vector<vec4f>
    MaterialPropertyType::SVECTORM4X4,// std::vector<mat4x4>
    MaterialPropertyType::SSTRING     // std::string
};


class ENGINE_API MaterialComponent : public Component {
public:
    MaterialComponent() = default;
    ~MaterialComponent() {};

    void OnInternalInit() override;
    inline void AddShader(const char* shaderName) {
        shaders.emplace_back(ShaderCache::GetShader(shaderName));
    }

    void AddTexture(DXTexture* tex);

    void BindPipeline();

    template<class T>
    void Set(std::string_view name, const T& value)
    {
        properties[name] = value;
    }

    template<class T>
    const T& Get(std::string_view name) const
    {
        return std::get<T>(properties.find(name)->second);
    }

    template<class T>
    T& Get(std::string_view name)
    {
        return const_cast<T&>(static_cast<const MaterialComponent*>(this)->Get<T>(name));
    }

    inline MaterialData& GetMaterialData() {
        return matData;
    }

    template<class T>
    bool Has(std::string_view name) const
    {
        auto p = properties.find(name);
        if (p == properties.end() || !std::holds_alternative<T>(p->second))
            return false;
        return true;
    }

    template<class T>
    const T* TryGet(std::string_view name) const
    {
        auto p = properties.find(name);
        if (p != properties.end())
            return &std::get<T>(p->second);
        else
            return nullptr;
    }

    template<class T>
    T* TryGet(std::string_view name)
    {
        return const_cast<T*>(static_cast<const MaterialComponent*>(this)->TryGet<T>(name));
    }

    bool HasProperty(std::string_view name) const
    {
        return properties.find(name) != properties.end();
    }

    MaterialPropertyType GetType(std::string_view name) {
        MaterialProperty mprop = TryGetMProp(name);
        size_t index = mprop.index();

        if (index >= TYPE_LOOKUP.size()) {
            return MaterialPropertyType::SUNDEFINED;
        }
        return TYPE_LOOKUP[index];
    }

    template<class T>
    T* GetPointer(std::string_view name)
    {
        auto it = properties.find(name);
        if (it != properties.end() && std::holds_alternative<T>(it->second)) {
            return &std::get<T>(it->second);
        }
        return nullptr;
    }

    std::unordered_map<std::string_view, MaterialProperty>& GetPropMap() {
        return properties;
    }

    void OnRender(ID3D11DeviceContext* pDeviceContext) override;
    void inline SetRasterizerState(RasterizerState state) {
        rs = state;
    };
    void OnUpdate(float deltaTime) override;
    void OnInit() override;
    void OnShutdown() override;

    void LateRender();

    inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) {
        topology = _topology;
    }

    inline void SetIndices(UINT i) { indices = i; }


private:
    MaterialProperty TryGetMProp(std::string_view name)
    {
        auto it = properties.find(name);
        if (it != properties.end())
            return it->second;
        else
            return NULL;
    }

    MaterialData matData;
protected:
    std::vector<IShader*> shaders;
    std::vector<DXTexture*> textures;

    std::unordered_map<std::string_view, MaterialProperty> properties;
    RasterizerState rs;
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    UINT indices = 0;
};

