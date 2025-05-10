#pragma once
#include <SlateEngine\Engine\Utils.h>
#include <SlateEngine\Engine\Math\vec3f.h>

struct SceneItem {
	~SceneItem() {
		for (auto c : childs) {
			delete c;
		}
	};

	const char* objectName;
	vec3f prs[3];
	std::vector<std::string> componentNames;
	std::vector<SceneItem*> childs;
	void* parent;
};

struct SceneHierarchy {
	std::vector<SceneItem*> items;
};

class Scene
{
public:
	Scene() = default;
	~Scene() = delete;

	inline void SetHierarchy(std::shared_ptr<SceneHierarchy> ptr) {
		mHierarchy = std::move(ptr);
	}

private:
	std::shared_ptr<SceneHierarchy> mHierarchy = nullptr;
};

