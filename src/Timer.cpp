#include "Timer.h"

Timer::Timer(int timeoutMS)
{
    m_timeout = std::chrono::duration<double>(timeoutMS / 1000.0);

    Reset();
}

void Timer::Reset()
{
    m_startTime = std::chrono::steady_clock::now();
}

bool Timer::IsExpired()
{
    auto elapsedTime = std::chrono::steady_clock::now() - m_startTime;
    return elapsedTime >= m_timeout;
}

void Timer::SetTimeout(int timeoutMS)
{
    m_timeout = std::chrono::duration<double>(timeoutMS / 1000.0);
}