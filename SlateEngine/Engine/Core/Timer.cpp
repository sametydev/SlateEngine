#include <SlateEngine/Engine/Core/Timer.h>

Timer* Timer::Instance = nullptr;

Timer::Timer():bIsActive(true),m_deltaTime(0),m_elapsedSecond(0)
{
    if (Instance == nullptr)
    {
        Instance = this;
    }
}

float Timer::deltaTime() const
{
    return m_deltaTime;
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
        m_deltaTime = 0.0f;
        return;
    }

    m_currentTime = std::chrono::system_clock::now();

    if (m_prevTime.time_since_epoch().count() != 0)
    {
        m_elapsedSecond = m_currentTime - m_prevTime;
        m_deltaTime = static_cast<float>(m_elapsedSecond.count());
    }
    else
    {
        m_deltaTime = 0.0f;
    }

    frameCount++;
    totalTime += m_deltaTime;

    if (totalTime >= 1.0f)
    {
        fps = static_cast<float>(frameCount) / totalTime;

        frameCount = 0;
        totalTime = 0.0f;
    }

    m_prevTime = m_currentTime;
}
