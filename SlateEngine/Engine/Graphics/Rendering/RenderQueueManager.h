#pragma once
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <SlateEngine/Engine/Graphics/Bindable.h>

struct RenderQueueCommand {
	Bindable* bindable;
};

class RenderQueueManager {

public:
	RenderQueueManager(ID3D11DeviceContext* pContext, ID3D11Device* pDevice);
	~RenderQueueManager();

	void Submit(RenderQueueCommand* cmd);
	void SetIndices(UINT indices);
	void Execute();

	RenderQueueCommand* currentCommandPtr = nullptr;
private:
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11Device* pDevice = nullptr;

	UINT indices = 0;
};