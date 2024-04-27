#include <bits/stdc++.h>

int main() {
  size_t tests_count = 100'000'000;
  std::optional<int64_t> a = 10;
  auto start1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < tests_count; ++i) {
    ++a.value();
  }
  auto finish1 = std::chrono::high_resolution_clock::now();
  std::cerr
      << std::chrono::duration_cast<std::chrono::milliseconds>(finish1 - start1)
      << "\n";

  int64_t b = 10;
  auto start2 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < tests_count; ++i) {
    ++b;
  }
  auto finish2 = std::chrono::high_resolution_clock::now();
  std::cerr
      << std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - start2)
      << "\n";
  assert(a.value() == b);


  start1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < tests_count; ++i) {
    ++a.value();
  }
  finish1 = std::chrono::high_resolution_clock::now();
  std::cerr
      << std::chrono::duration_cast<std::chrono::milliseconds>(finish1 - start1)
      << "\n";
}