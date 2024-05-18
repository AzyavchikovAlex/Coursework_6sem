#include <gtest/gtest.h>

#include "tree_v4.h"
#include "Utils/test_templates.h"

TEST(TreeV4, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V4>();
}

TEST(TreeV4, RandomTest) {
  RandomTest<ParallelSegmentTree_V4, 1>();
  RandomTest<ParallelSegmentTree_V4, 5>();
}

TEST(TreeV4, BenchmarkTest) {
  std::cerr << "TreeV4 4 segments and 3 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4, 4, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV4, SlowDataBenchmarkTest) {
  std::cerr << "TreeV4 4 segments and 3 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V4, 4, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 5 segments and 3 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V4, 5, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}
