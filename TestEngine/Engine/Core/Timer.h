#pragma once
#include <stdint.h>
#include <chrono>

class Timer
{
public:
	Timer();
	float deltaTime() const; 
	void OnReset(); 
	void OnTick();

private:
	double m_deltaTime;
	std::chrono::system_clock::time_point m_prevTime;
	std::chrono::system_clock::time_point m_currentTime;
	std::chrono::duration<double> m_elapsedSecond;
	bool bIsActive;
};
