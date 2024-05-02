#include <gtest/gtest.h>

#include "classic_segment_tree.h"
#include "../../Utils/test_templates.h"

TEST(ClassicSegmentTree, SimpleTest) {
  SimpleTest<Tree>();
}

TEST(ClassicSegmentTree, RandomTest) {
  RandomTest<Tree>();
}

TEST(ClassicSegmentTree, BenchmarkTest) {
  std::cerr << "ClassicSegmentTree\n";
  BenchmarkTest<Tree>();
  std::cerr << "-----------------------------------------------\n";
}