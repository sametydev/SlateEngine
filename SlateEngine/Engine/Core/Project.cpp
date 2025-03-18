#include <SlateEngine\Engine\Core\Project.h>
Project* Project::Instance = nullptr;

Project::Project()
{
	if (Instance == nullptr) {
		Instance = this;
	}
	else if (Instance != nullptr && Instance != this) {
		Instance = this;
	}
}
