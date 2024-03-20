#include "consuming_timer.h"

#include <plog/Log.h>
#include <tinyformat.h>

ConsumingTimer::ConsumingTimer(std::string_view name)
    : name_(name)
    , start_time_(std::chrono::system_clock::now())
{
}

ConsumingTimer::~ConsumingTimer()
{
}

int64_t ConsumingTimer::Tick()
{
    auto curr_time = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - start_time_).count();
}

void ConsumingTimer::PrintLog()
{
    PLOGD << tinyformat::format("%s:(million secs=%d)", name_, Tick());
}
