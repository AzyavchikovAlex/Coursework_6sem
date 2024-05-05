#pragma once

#include <chrono>
#include <functional>

namespace bench {

typedef std::chrono::high_resolution_clock clock;
// typedef std::chrono::system_clock clock;
typedef std::chrono::nanoseconds duration;

duration Measure(const std::function<void()>& command,
                 duration time_limit = std::chrono::seconds(2));

} // namespace bench
