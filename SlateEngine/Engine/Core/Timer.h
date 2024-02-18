#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	float deltaTime() const; 
	void OnReset(); 
	void OnTick();


	Timer* Instance = nullptr;
private:
	float m_deltaTime;
	std::chrono::system_clock::time_point m_prevTime;
	std::chrono::system_clock::time_point m_currentTime;
	std::chrono::duration<double> m_elapsedSecond;
	bool bIsActive;
};
