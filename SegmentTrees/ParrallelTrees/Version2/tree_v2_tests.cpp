#include <gtest/gtest.h>

#include "tree_v2.h"
#include "Utils/test_templates.h"

TEST(TreeV2, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V2>();
}

TEST(TreeV2, RandomTest) {
  // RandomTest<ParallelSegmentTree_V2, 1>();
  RandomTest<ParallelSegmentTree_V2, 4>();
}

TEST(TreeV2, BenchmarkTest) {
  std::cerr << "TreeV2\n";
  BenchmarkTest<ParallelSegmentTree_V2, 2>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV2, SlowDataBenchmarkTest) {
  std::cerr << "TreeV2 2 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V2, 2, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV2 4 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V2, 4, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV2 6 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V2, 6, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV2, BatchingBenchmarkTest) {
  std::cerr << "TreeV2 (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V2, 2, 4'500'000'000>();
  std::cerr << "-----------------------------------------------\n";
}
