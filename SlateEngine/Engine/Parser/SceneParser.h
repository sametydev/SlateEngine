#pragma once
#include <SlateEngine\Engine\Core\Scene.h>

class SceneParser
{
public:
	SceneParser();
	~SceneParser();

	void LoadSceneFromFile(const char* pFile);
	Scene* LoadSceneFromFileAsScene(const char* pFile);

	static SceneParser* Instance;
};

