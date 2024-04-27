#pragma once
#include <chrono>
#include <functional>

namespace bench {

typedef std::chrono::high_resolution_clock clock;
typedef clock::time_point time_point;
typedef std::chrono::milliseconds duration;
// typedef std::chrono::nanoseconds duration;

duration Measure(const std::function<void()>& command,
                 duration time_limit = std::chrono::seconds(1)) {
  auto start = clock::now();
  auto finish = start + time_limit;
  int64_t count = 0;
  while (clock::now() < finish) {
    command();
    ++count;
  }
  finish = clock::now();
  return std::chrono::duration_cast<duration>(finish - start) / count;
}

} // namespace bench
