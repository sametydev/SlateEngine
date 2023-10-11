#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IRenderPass.h>

class RenderPassManager {
public:
	RenderPassManager();
	~RenderPassManager();

	static RenderPassManager* Instance;
};

