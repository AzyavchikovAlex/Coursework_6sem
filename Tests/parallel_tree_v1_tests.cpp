#include <gtest/gtest.h>
#include <random>

#include "../SegmentTree/tree_v1.h"
#include "../Strategies/AssignAndSum/assign_sum_strategy.h"

TEST(TreeV1, SimpleTest) {
  std::vector<int> values = {1, 2, 3};
  std::shared_ptr<AbstractMassPolicy<int, int>>
      strategy = std::make_shared<AssignSumStrategy<int>>();
  ParallelSegmentTree<int, int> tree(values, std::move(strategy));
  EXPECT_EQ(tree.GetTreeState(0, 3), 6);
  EXPECT_EQ(tree.GetTreeState(0, 2), 3);
  tree.ModifyTree(0, 1, 5);
  EXPECT_EQ(tree.GetTreeState(0, 3), 10);
}

TEST(TreeV1, RandomTest) {
  size_t n = 1000;
  size_t tests_count = 20000;
  std::vector<int> values(n, 0);
  std::shared_ptr<AbstractMassPolicy<int, int>>
      strategy = std::make_shared<AssignSumStrategy<int>>();
  ParallelSegmentTree<int, int> actual_tree(values, std::move(strategy));
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, (int) n - 1);

  for (size_t test = 0; test < tests_count; ++test) {
    int l = distrib(gen);
    int r = distrib(gen);
    if (l > r) std::swap(l, r);
    ++r;
    if ((test % 10) == 0) {
      // modify
      int value = distrib(gen);
      ASSERT_NO_THROW(actual_tree.ModifyTree(l, r, value));
      for (size_t i = l; i < r; ++i) {
        values[i] = value;
      }
    }
    // state
    ASSERT_EQ(actual_tree.GetTreeState(l, r),
              std::accumulate(std::next(values.begin(), l),
                              std::next(values.begin(), r),
                              0));
  }
}
