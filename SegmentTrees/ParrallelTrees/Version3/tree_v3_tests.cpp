#include <gtest/gtest.h>

#include "tree_v3.h"
#include "Utils/test_templates.h"

TEST(TreeV3, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V3>();
}

TEST(TreeV3, RandomTest) {
  RandomTest<ParallelSegmentTree_V3, 1>();
  RandomTest<ParallelSegmentTree_V3, 5>();
}

TEST(TreeV3, BenchmarkTest) {
  std::cerr << "TreeV3 2 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 2, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 4 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 4, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 6 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 6, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV3, SlowDataBenchmarkTest) {
  std::cerr << "TreeV3 2 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V3, 2, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 4 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V3, 4, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 6 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V3, 6, 2'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV3, BatchingBenchmarkTest) {
  std::cerr << "TreeV3 2 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V3, 2>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 4 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V3, 4>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 8 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V3, 8>();
  std::cerr << "-----------------------------------------------\n";
}
