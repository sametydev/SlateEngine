#pragma once
#include <SlateEngine/Engine/DXConfig.h>

enum RasterizerState {
	CULL_BACK = 0,
	CULL_FRONT,
	CULL_FRONTANDBACK,
	CULL_WIREFRAME
};

class DXRasterizerState
{
public:
	DXRasterizerState();
	~DXRasterizerState() {};

	void Create();

	void SetRasterizerState(RasterizerState state);

	bool bStateCreated = false;

	static DXRasterizerState* Instance;
private:
	//State
	ComPtr<ID3D11RasterizerState> RSCullBack = nullptr;
	ComPtr<ID3D11RasterizerState> RSCullFront = nullptr;
	ComPtr<ID3D11RasterizerState> RSCullFrontAndBack = nullptr;
	ComPtr<ID3D11RasterizerState> RSWireFrame = nullptr;

	//Sampler
	ComPtr<ID3D11SamplerState> SSWrap = nullptr;
	ComPtr<ID3D11SamplerState> SSClamp = nullptr;

	ComPtr<ID3D11BlendState> BSTransparent = nullptr;
	ComPtr<ID3D11BlendState> BSBlendDisable = nullptr;
	ComPtr<ID3D11BlendState> BSBlend = nullptr;

	ComPtr<ID3D11DepthStencilState> DSState = nullptr;

	void CreateRSState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateDepthStencilState();
};

