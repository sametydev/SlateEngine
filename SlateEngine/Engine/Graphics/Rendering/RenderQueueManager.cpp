#include "RenderQueueManager.h"

RenderQueueManager::RenderQueueManager(ID3D11DeviceContext* pContext, ID3D11Device* pDevice)
{
	this->pContext = pContext;
	this->pDevice = pDevice;
}

RenderQueueManager::~RenderQueueManager()
{
}

void RenderQueueManager::Submit(RenderQueueCommand* cmd)
{
	currentCommandPtr = cmd;
	cmd->bindable->Bind();

}

void RenderQueueManager::SetIndices(UINT indices)
{
	this->indices = indices;
}

void RenderQueueManager::Execute()
{
	pContext->DrawIndexed(indices, 0u, 0u);
	currentCommandPtr = nullptr;
	indices = 0;
}
