#pragma once
#include <SlateEngine\Engine\Utils.h>
#include <chrono>


struct ENGINE_API ProfilerTimerOutputData {
	ProfilerTimerOutputData(std::string name, float duration) : mName(name), mDuration(duration) {};
	std::string mName;
	float mDuration;
};

template<typename T>
class ENGINE_API ProfileTimer
{
public:
	ProfileTimer() {};

	ProfileTimer(std::string name,T&& func)
		: mName(name), mStopped(false), mFunc(func)
	{
		mFunc = std::forward<T>(func);
	}

	~ProfileTimer()
	{
		if (!mStopped)
			Stop();
	}

	inline void StartManual(std::string name, T&& func) {
		mName = name;
		mStopped = false;
		mFunc = std::forward<T>(func);

		mStart = std::chrono::steady_clock::now();
	}

	inline void Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();

		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(mStart).time_since_epoch();
		mStopped = true;

		float duration = elapsedTime.count() * 0.001f;
		
		mFunc({mName,duration});
	}
private:
	std::string mName;
	std::chrono::time_point<std::chrono::steady_clock> mStart;
	bool mStopped;
	T mFunc;
};