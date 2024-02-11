#include <gtest/gtest.h>

#include "segment_tree.h"

TEST(CcassicSegmentTree, SimpleTest) {
  std::vector<int> values = {1, 2, 3};
  Tree<int> tree(values);
  EXPECT_EQ(tree.Sum(0, 3), 6);
  EXPECT_EQ(tree.Sum(0, 2), 3);
  tree.Assign(0, 1, 5);
  EXPECT_EQ(tree.Sum(0, 3), 10);
}