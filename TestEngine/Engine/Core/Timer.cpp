#include <TestEngine/Engine/Core/Timer.h>

Timer::Timer():bIsActive(true),m_deltaTime(0),m_elapsedSecond(0)
{
}

float Timer::deltaTime() const
{
    return (float)m_deltaTime;
}

void Timer::OnReset()
{
    m_deltaTime = 0.0f;
    bIsActive = true;
}

void Timer::OnTick()
{
    if (!bIsActive)
    {
        m_deltaTime = 0.0;
        return;
    }

    //deltaTime:
    m_currentTime = std::chrono::system_clock::now();
    m_elapsedSecond = std::chrono::duration<double>();

    if (m_prevTime.time_since_epoch().count())
    {
        m_elapsedSecond = m_currentTime - m_prevTime;
    }

    m_prevTime = m_currentTime;

    m_deltaTime = (float)m_elapsedSecond.count();


}
