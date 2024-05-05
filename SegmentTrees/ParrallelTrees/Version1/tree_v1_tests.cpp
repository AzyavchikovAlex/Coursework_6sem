#include <gtest/gtest.h>

#include "tree_v1.h"
#include "Utils/test_templates.h"

TEST(TreeV1, SimpleTest) {
  SimpleTest<ParallelSegmentTree_V1>();
}

TEST(TreeV1, RandomTest) {
  RandomTest<ParallelSegmentTree_V1>();
}

TEST(TreeV1, BenchmarkTest) {
  std::cerr << "TreeV1\n";
  BenchmarkTest<ParallelSegmentTree_V1>();
  std::cerr << "-----------------------------------------------\n";
}
