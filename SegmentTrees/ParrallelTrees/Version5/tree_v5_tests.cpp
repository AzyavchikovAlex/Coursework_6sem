#include <gtest/gtest.h>

#include "tree_v5.h"
#include "Utils/test_templates.h"

TEST(TreeV5, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V5>();
}

TEST(TreeV5, RandomTest) {
  RandomTest<ParallelSegmentTree_V5, 1>();
  RandomTest<ParallelSegmentTree_V5, 5>();
}

TEST(TreeV5, BenchmarkTest) {
  std::cerr << "TreeV5 4 segments and 3 threads\n";
  BenchmarkTest<ParallelSegmentTree_V5, 4, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV5, SlowDataBenchmarkTest) {
  std::cerr << "TreeV5 4 segments and 3 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V5, 4, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV5 5 segments and 3 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V5, 5, 3'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}
