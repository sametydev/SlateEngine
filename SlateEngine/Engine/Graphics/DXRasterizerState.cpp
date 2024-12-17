#include <SlateEngine/Engine/Graphics/DXRasterizerState.h>

ComPtr<ID3D11RasterizerState> DXRasterizerState::RSNoCull = nullptr;
ComPtr<ID3D11SamplerState> DXRasterizerState::SSAnisotropicWrap = nullptr;
ComPtr<ID3D11SamplerState> DXRasterizerState::SSLinearWrap = nullptr;
ComPtr<ID3D11BlendState> DXRasterizerState::BSAlphaToCoverage = nullptr;
ComPtr<ID3D11BlendState> DXRasterizerState::BSNoColorWrite = nullptr;
ComPtr<ID3D11BlendState> DXRasterizerState::BSTransparent = nullptr;
ComPtr<ID3D11BlendState> DXRasterizerState::BSAdditive = nullptr;
ComPtr<ID3D11DepthStencilState> DXRasterizerState::DSSWriteStencil = nullptr;
ComPtr<ID3D11DepthStencilState> DXRasterizerState::DSSDrawWithStencil = nullptr;
ComPtr<ID3D11DepthStencilState> DXRasterizerState::DSState = nullptr;
ComPtr<ID3D11RasterizerState> DXRasterizerState::RSCullBack = nullptr;
ComPtr<ID3D11RasterizerState> DXRasterizerState::RSCullFront = nullptr;
ComPtr<ID3D11RasterizerState> DXRasterizerState::RSWireFrame = nullptr;
ComPtr<ID3D11SamplerState> DXRasterizerState::SSWrap = nullptr;
ComPtr<ID3D11SamplerState> DXRasterizerState::SSClamp = nullptr;

void DXRasterizerState::Initialize(ID3D11Device* device,ID3D11DeviceContext* context, bool msaaEnabled)
{
    D3D11_RASTERIZER_DESC rasterizerDesc{};

    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR(device->CreateSamplerState(&sampDesc, SSLinearWrap.GetAddressOf()));

    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MaxAnisotropy = 4;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR(device->CreateSamplerState(&sampDesc, SSAnisotropicWrap.GetAddressOf()));


    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    auto& rtDesc = blendDesc.RenderTarget[0];

    blendDesc.AlphaToCoverageEnable = true;
    blendDesc.IndependentBlendEnable = false;
    rtDesc.BlendEnable = false;
    rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    HR(device->CreateBlendState(&blendDesc, BSAlphaToCoverage.GetAddressOf()));

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    rtDesc.BlendEnable = true;
    rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

    HR(device->CreateBlendState(&blendDesc, BSTransparent.GetAddressOf()));


    rtDesc.SrcBlend = D3D11_BLEND_ONE;
    rtDesc.DestBlend = D3D11_BLEND_ONE;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

    HR(device->CreateBlendState(&blendDesc, BSAdditive.GetAddressOf()));


    rtDesc.BlendEnable = false;
    rtDesc.SrcBlend = D3D11_BLEND_ZERO;
    rtDesc.DestBlend = D3D11_BLEND_ONE;
    rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
    rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
    rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;
    rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtDesc.RenderTargetWriteMask = 0;
    HR(device->CreateBlendState(&blendDesc, BSNoColorWrite.GetAddressOf()));


    D3D11_DEPTH_STENCIL_DESC dsDesc{};


    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    HR(device->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));


    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

    HR(device->CreateDepthStencilState(&dsDesc, DSSDrawWithStencil.GetAddressOf()));

    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    //desctions if pixel is front-facing.
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    //desctions if pixel is back-facing.
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    HR(device->CreateDepthStencilState(&dsDesc, DSState.GetAddressOf()));


    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.MultisampleEnable = msaaEnabled;
    rasterizerDesc.AntialiasedLineEnable = false;

    HR(device->CreateRasterizerState(&rasterizerDesc, RSCullBack.GetAddressOf()));

    rasterizerDesc.CullMode = D3D11_CULL_FRONT;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSCullFront.GetAddressOf()));

    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    HR(device->CreateRasterizerState(&rasterizerDesc, RSWireFrame.GetAddressOf()));

    D3D11_SAMPLER_DESC sd{};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.BorderColor[0] = { 0 };
    sd.BorderColor[1] = { 0 };
    sd.BorderColor[2] = { 0 };
    sd.BorderColor[3] = { 0 };
    sd.MinLOD = 0;
    sd.MaxLOD = D3D11_FLOAT32_MAX;
    sd.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
    //sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

    //Wrap sampler to default 
    HR(device->CreateSamplerState(&sd, SSWrap.GetAddressOf()));
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    //Clamp
    HR(device->CreateSamplerState(&sd, SSClamp.GetAddressOf()));

    //Default Sampler Set;
    context->PSSetSamplers(0, 1, SSWrap.GetAddressOf());

}

void DXRasterizerState::SetRasterizerState(RasterizerState state, ID3D11DeviceContext* device)
{
    switch (state)
    {
        case CULL_BACK:
            device->RSSetState(RSCullBack.Get());
            break;
        case CULL_FRONT:
            device->RSSetState(RSCullFront.Get());
            break;
        case CULL_FRONTANDBACK:
            device->RSSetState(RSNoCull.Get());
            break;
        case CULL_WIREFRAME:
            device->RSSetState(RSWireFrame.Get());
            break;
    }
}
