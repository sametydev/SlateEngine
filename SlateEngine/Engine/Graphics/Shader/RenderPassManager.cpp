#include "RenderPassManager.h"
RenderPassManager* RenderPassManager::Instance = nullptr;

RenderPassManager::RenderPassManager() {
	if (Instance == nullptr)
	{
		Instance = this;
	}

	//lsp = std::make_unique<LitShaderPass>();
}

RenderPassManager::~RenderPassManager() {
	//lsp.release();
}

