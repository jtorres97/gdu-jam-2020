#pragma once

#include <chrono>

class Timer
{
public:
    Timer(int timeoutMS);
    
    void Reset();
    bool IsExpired();
    void SetTimeout(int timeoutMS);

private:
    std::chrono::duration<double> m_timeout;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> m_startTime;
};
