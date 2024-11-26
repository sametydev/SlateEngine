#pragma once
#include <SlateEngine/Engine/Graphics/Shader/IRenderPass.h>

class ENGINE_API RenderPassManager {
public:
	RenderPassManager();
	~RenderPassManager();

	static RenderPassManager* Instance;
};

