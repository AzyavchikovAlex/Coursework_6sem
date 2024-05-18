#include <gtest/gtest.h>

#include "tree_v3.h"
#include "tree_v3_modified.h"
#include "Utils/test_templates.h"
#include "tree_v3_modified.h"

TEST(TreeV3, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V3>();
}

TEST(TreeV3, RandomTest) {
  RandomTest<ParallelSegmentTree_V3, 1>();
  RandomTest<ParallelSegmentTree_V3, 5>();
}

TEST(TreeV3, BenchmarkTest) {
  std::cerr << "TreeV3 2 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 2, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 4 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 4, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 6 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 6, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 8 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3, 8, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV3, SlowDataBenchmarkTest) {
  std::cerr << "TreeV3 6 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V3, 6, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
  std::cerr << "TreeV3 8 threads (slow data)\n";
  SlowDataBenchmarkTest<ParallelSegmentTree_V3, 8, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}

TEST(TreeV3_Modified, BenchmarkTest) {
  std::cerr << "TreeV3 (modified) 2 + 3 + 3 threads\n";
  BenchmarkTest<ParallelSegmentTree_V3_modified, 2, 1'000'000'000>();
  std::cerr << "-----------------------------------------------\n";
}
