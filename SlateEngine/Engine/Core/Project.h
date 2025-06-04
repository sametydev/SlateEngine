#pragma once
#include <SlateEngine\Engine\DXConfig.h>

struct ENGINE_API Project
{
public:
	Project();

	//In future we can make ProjectManager class but rn, its useless, because technically theres can only be a ONE project :)
	static Project* Instance;

	inline std::string& GetProjectName() {
		return m_projectName;
	};

	inline std::string& GetProjectVersion() {
		return m_projectVersion;
	};

	inline uint64_t& GetProjectVSYNC() {
		return m_vsync;
	};

	inline void SetProjectName(std::string newName) {
		m_projectName = newName;
	};

	inline void SetProjectVersion(std::string newVersion) {
		m_projectVersion = newVersion;
	};

	inline void SetProjectVSYNC(uint64_t newVsync) {
		m_vsync = newVsync;
	};

private:
	std::string m_projectName;
	std::string m_projectVersion;
	uint64_t m_vsync;
};

