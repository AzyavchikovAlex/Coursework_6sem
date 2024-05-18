#include <gtest/gtest.h>

#include "tree_v0.h"
#include "Utils/test_templates.h"

TEST(TreeV0, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V0>();
}

TEST(TreeV0, RandomTest) {
  RandomTest<ParallelSegmentTree_V0>();
}

TEST(TreeV0, BenchmarkTest) {
  std::cerr << "TreeV0\n";
  BenchmarkTest<ParallelSegmentTree_V0>();
  std::cerr << "-----------------------------------------------\n";
}
