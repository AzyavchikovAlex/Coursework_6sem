#include <gtest/gtest.h>

#include "tree_v2.h"
#include "Utils/test_templates.h"

TEST(TreeV2, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V2>();
}

TEST(TreeV2, RandomTest) {
  RandomTest<ParallelSegmentTree_V2, 1>();
  // RandomTest<ParallelSegmentTree_V2, 4>();
}

TEST(TreeV2, BenchmarkTest) {
  std::cerr << "TreeV2\n";
  BenchmarkTest<ParallelSegmentTree_V2, 2>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV2, BatchingBenchmarkTest) {
  std::cerr << "TreeV2 (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V2, 2>();
  std::cerr << "-----------------------------------------------\n";
}
