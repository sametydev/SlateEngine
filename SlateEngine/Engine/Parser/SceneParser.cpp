#include "SceneParser.h"

SceneParser* SceneParser::Instance = nullptr;

SceneParser::SceneParser()
{
	if (Instance == nullptr) {
		Instance = this;
	}
}

SceneParser::~SceneParser()
{
}

void SceneParser::LoadSceneFromFile(const char* pFile)
{

}

Scene* SceneParser::LoadSceneFromFileAsScene(const char* pFile)
{
	return nullptr;
}
