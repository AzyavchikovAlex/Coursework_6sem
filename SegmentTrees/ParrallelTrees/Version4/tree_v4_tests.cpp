#include <gtest/gtest.h>

#include "tree_v4.h"
#include "tree_v4_modified.h"
#include "Utils/test_templates.h"
#include "tree_v4_modified.h"

TEST(TreeV4, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V4>();
}

TEST(TreeV4, RandomTest) {
  RandomTest<ParallelSegmentTree_V4, 1>();
  RandomTest<ParallelSegmentTree_V4, 5>();
}

TEST(TreeV4, BenchmarkTest) {
  std::cerr << "TreeV4 2 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4, 2, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 4 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4, 4, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 6 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4, 6, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 8 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4, 8, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV4, SlowDataBenchmarkTest) {
  std::cerr << "TreeV4 6 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V4, 6, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 8 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V4, 8, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV4_Modified, BenchmarkTest) {
  std::cerr << "TreeV4 (modified) 2 + 3 + 3 threads\n";
  BenchmarkTest<ParallelSegmentTree_V4_modified, 2, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV4, BatchingBenchmarkTest) {
  std::cerr << "TreeV4 2 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V4, 2>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 4 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V4, 4>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV4 8 threads (batching)\n";
  BatchingBenchmarkTest<ParallelSegmentTree_V4, 8>();
  std::cerr << "-----------------------------------------------\n";
}
