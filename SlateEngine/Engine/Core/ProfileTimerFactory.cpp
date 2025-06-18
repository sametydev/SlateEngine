#include "ProfileTimerFactory.h"

ProfileTimerFactory* ProfileTimerFactory::Instance = nullptr;
ProfileTimerFactory::ProfileTimerFactory()
{
	if (Instance == nullptr) {
		Instance = this;
	}
}

ProfileTimerFactory::~ProfileTimerFactory()
{
}

void ProfileTimerFactory::CreateTimer(std::string Name)
{
	if (timers.count(Name)) {
		delete timers[Name];
	}

	ProfileTimer<std::function<void(ProfilerTimerOutputData)>>* timer = new ProfileTimer<std::function<void(ProfilerTimerOutputData)>>();

	timer->StartManual(Name.c_str(), [&](ProfilerTimerOutputData profileResult) {
		ProfileTimerFactory::Instance->profileResults[profileResult.mName] = (profileResult.mDuration);
	});

	timers[Name] = timer;

	currentTimer = timers[Name];
}
