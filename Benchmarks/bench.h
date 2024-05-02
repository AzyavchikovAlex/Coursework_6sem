#pragma once

#include <chrono>
#include <functional>

namespace bench {

typedef std::chrono::high_resolution_clock clock;
typedef clock::time_point time_point;
typedef std::chrono::nanoseconds duration;

duration Measure(const std::function<void()>& command,
                 duration time_limit = std::chrono::seconds(1));

} // namespace bench
