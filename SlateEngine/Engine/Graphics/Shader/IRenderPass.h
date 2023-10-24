#pragma once
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Component/Material.h>
#include <SlateEngine/Engine/Component/Components.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Graphics/Buffer/DXConstantBuffer.h>

class IRenderPass
{
public:
    IRenderPass() = default;
    virtual ~IRenderPass() = default;
    virtual void Bind(unsigned int offset = 0) = 0;
    virtual void UpdateConstantBuffer(unsigned int size,void* pData) = 0;
};