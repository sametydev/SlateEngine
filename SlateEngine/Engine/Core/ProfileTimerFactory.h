#pragma once
#include <SlateEngine\Engine\Core\ProfileTimer.h>
#include <SlateEngine\Engine\Utils.h>
#include <unordered_map>
#include <functional>

constexpr bool PROFILER_ENABLED = true;

#define PROFILER_START(name) ProfileTimerFactory::Instance->CreateTimer(name);
#define PROFILER_STOP() ProfileTimerFactory::Instance->StopCurrentTimer();

class ENGINE_API ProfileTimerFactory
{
public:
	ProfileTimerFactory();
	~ProfileTimerFactory();

	void CreateTimer(std::string Name);

	inline void StopCurrentTimer() {
		if (!PROFILER_ENABLED) return;
		currentTimer->Stop();
	};

	inline std::unordered_map<std::string, float> GetResults() {
		return profileResults;
	}
	
	static ProfileTimerFactory* Instance;
private:
	std::unordered_map<std::string, ProfileTimer<std::function<void(ProfilerTimerOutputData)>>*> timers;
	std::unordered_map<std::string,float> profileResults;


	ProfileTimer<std::function<void(ProfilerTimerOutputData)>>* currentTimer = nullptr;
};

