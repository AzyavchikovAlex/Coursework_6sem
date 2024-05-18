#include <gtest/gtest.h>

#include "tree_v1.h"
#include "Utils/test_templates.h"

TEST(TreeV1, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V1>();
}

TEST(TreeV1, RandomTest) {
  // RandomTest<ParallelSegmentTree_V2, 1>();
  RandomTest<ParallelSegmentTree_V1, 4>();
}

TEST(TreeV1, BenchmarkTest) {
  std::cerr << "TreeV1\n";
  BenchmarkTest<ParallelSegmentTree_V1, 2>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV1, SlowDataBenchmarkTest) {
  std::cerr << "TreeV1 2 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V1, 2, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV1 4 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V1, 4, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV1 6 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V1, 6, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

