#ifndef CONSUMING_TIMER_H
#define CONSUMING_TIMER_H

#include <cstdint>

#include <chrono>
#include <string>

class ConsumingTimer
{
public:
    ConsumingTimer(ConsumingTimer const&) = delete;
    ConsumingTimer& operator=(ConsumingTimer const&) = delete;
    ConsumingTimer(ConsumingTimer&&) = delete;
    ConsumingTimer& operator=(ConsumingTimer&&) = delete;

    explicit ConsumingTimer(std::string_view name);
    ~ConsumingTimer();

    int64_t Tick();

    void PrintLog();

private:
    std::string name_;
    std::chrono::time_point<std::chrono::system_clock> start_time_;
};

#endif
