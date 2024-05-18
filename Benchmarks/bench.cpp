#include "bench.h"

bench::duration bench::Measure(const std::function<void()>& command,
                 bench::duration time_limit) {
  auto start = bench::clock::now();
  auto finish = start + time_limit;
  int64_t count = 0;
  while (clock::now() < finish) {
    command();
    ++count;
  }
  finish = clock::now();
  return std::chrono::duration_cast<duration>(finish - start) / count;
}