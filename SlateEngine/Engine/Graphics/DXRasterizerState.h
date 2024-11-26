#pragma once
#include <SlateEngine/Engine/DXConfig.h>

enum RasterizerState {
	CULL_BACK = 0,
	CULL_FRONT,
	CULL_FRONTANDBACK,
	CULL_WIREFRAME
};

class ENGINE_API DXRasterizerState
{
public:
    static void Initialize(ID3D11Device* device,ID3D11DeviceContext* context,bool msaaEnabled);
    static void SetRasterizerState(RasterizerState state, ID3D11DeviceContext* device);

public:	           
    static ComPtr<ID3D11RasterizerState> RSNoCull;   

    static ComPtr<ID3D11SamplerState> SSLinearWrap;
    static ComPtr<ID3D11SamplerState> SSAnisotropicWrap;		       

    static ComPtr<ID3D11BlendState> BSNoColorWrite;		               
    static ComPtr<ID3D11BlendState> BSTransparent;		               
    static ComPtr<ID3D11BlendState> BSAlphaToCoverage;	               
    static ComPtr<ID3D11BlendState> BSAdditive;			               


    static ComPtr<ID3D11DepthStencilState> DSSWriteStencil;
    static ComPtr<ID3D11DepthStencilState> DSSDrawWithStencil;

    //State
    static ComPtr<ID3D11RasterizerState> RSCullBack;
    static ComPtr<ID3D11RasterizerState> RSCullFront;
    static ComPtr<ID3D11RasterizerState> RSWireFrame;

    //Sampler
    static ComPtr<ID3D11SamplerState> SSWrap;
    static ComPtr<ID3D11SamplerState> SSClamp;
};

