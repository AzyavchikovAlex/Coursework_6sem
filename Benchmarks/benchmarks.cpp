#include <bits/stdc++.h>

#include "bench.h"

int main() {
  bench::duration average_duration = bench::Measure([]() {
    int64_t x = 0;
    while (x < 100'000'000) {
      ++x;
    }
  });
  std::cerr << average_duration << "\n";
}