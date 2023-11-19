#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IShader.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Component/Component.h>
#include <vector>
struct MaterialData
{
    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
    vec4f reflect;
};

using MaterialProperty = std::variant<int, uint32_t, float, vec2f, vec3f, vec4f, mat4x4,std::vector<float>, std::vector<vec4f>, std::vector<mat4x4>,std::string>;

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

struct MaterialComponent : public Component {
public:
    MaterialComponent() = default;
    ~MaterialComponent() {};

    void OnInternalInit() override;
    inline void AddShader(IShader* shader) {
        shaders.emplace_back(shader);
    }

    inline void AddTexture(DXTexture* tex) {
        textures.emplace_back(tex);
    }

    inline void BindPipeline() {

    }

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
        if (std::holds_alternative<float>(mprop))
        {
            return MaterialPropertyType::SFLOAT;
        }
        else if (std::holds_alternative<int>(mprop)) {
            return MaterialPropertyType::SINT;
        }
        else if (std::holds_alternative<uint32_t>(mprop)) {
            return MaterialPropertyType::SUINT;
        }
        else if (std::holds_alternative<vec2f>(mprop)) {
            return MaterialPropertyType::SVEC2;
        }
        else if (std::holds_alternative<vec3f>(mprop)) {
            return MaterialPropertyType::SVEC3;
        }
        else if (std::holds_alternative<vec4f>(mprop)) {
            return MaterialPropertyType::SVEC4;
        }
        else if (std::holds_alternative<mat4x4>(mprop)) {
            return MaterialPropertyType::SM4X4;
        }
        else if (std::holds_alternative<std::vector<float>>(mprop)) {
            return MaterialPropertyType::SVECTORF;
        }
        else if (std::holds_alternative<std::vector<vec4f>>(mprop)) {
            return MaterialPropertyType::SVECTORV4;
        }
        else if (std::holds_alternative<std::vector<mat4x4>>(mprop)) {
            return MaterialPropertyType::SVECTORM4X4;
        }
        else if (std::holds_alternative<std::string>(mprop)) {
            return MaterialPropertyType::SSTRING;
        }
        else{
            return MaterialPropertyType::SUNDEFINED;
        }
    }

    std::unordered_map<std::string_view, MaterialProperty>& GetPropMap() {
        return properties;
    }

    void OnRender() override;
    void OnUpdate(float deltaTime) override;


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
};

