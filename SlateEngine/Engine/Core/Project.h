#pragma once
#include <SlateEngine\Engine\DXConfig.h>

struct Project
{
public:
	Project();

	//In future we can make ProjectManager class but rn, its useless, because technically theres can only be a ONE project :)
	static Project* Instance;

	inline std::string& GetProjectName() {
		return m_projectName;
	};

	inline void SetProjectName(std::string newName) {
		m_projectName = newName;
	};

private:
	std::string m_projectName;
};

