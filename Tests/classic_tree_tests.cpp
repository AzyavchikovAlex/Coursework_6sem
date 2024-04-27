#include <gtest/gtest.h>
#include <random>

#include "../SegmentTree/assign_sum_tree.h"
#include "../SegmentTree/classic_segment_tree.h"
#include "../Strategies/AssignAndSum/assign_sum_strategy.h"

TEST(ClassicSegmentTree, SimpleTest) {
  std::vector<int> values = {1, 2, 3};
  std::shared_ptr<AbstractMassPolicy<int, int>>
      strategy = std::make_shared<AssignSumStrategy<int>>();
  Tree<int, int> tree(values, std::move(strategy));
  EXPECT_EQ(tree.GetTreeState(0, 3), 6);
  EXPECT_EQ(tree.GetTreeState(0, 2), 3);
  tree.ModifyTree(0, 1, 5);
  EXPECT_EQ(tree.GetTreeState(0, 3), 10);
}

TEST(ClassicSegmentTree, RandomTest) {
  size_t n = 1000;
  size_t tests_count = 20000;
  std::vector<int> values(n, 0);
  std::shared_ptr<AbstractMassPolicy<int, int>>
      strategy = std::make_shared<AssignSumStrategy<int>>();
  Tree<int, int> tree(values, std::move(strategy));
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
      ASSERT_NO_THROW(tree.ModifyTree(l, r, value));
      for (size_t i = l; i < r; ++i) {
        values[i] = value;
      }
    }
    // state
    ASSERT_EQ(tree.GetTreeState(l, r),
              std::accumulate(std::next(values.begin(), l),
                              std::next(values.begin(), r),
                              0));
  }
}

// TEST(ClassicSegmentTree, SpeedTest) {
//   size_t n = 10'000'000;
//   std::vector<int> values(n, 0);
//   std::shared_ptr<AbstractMassPolicy<int, int>>
//       strategy = std::make_shared<AssignSumStrategy<int>>();
//   // Tree<int, int> tree1(values, std::move(strategy));
//   ParallelSegmentTree<int, int> tree1(values, std::move(strategy));
//   ASTree tree2(values);
//
//   size_t tests_count = 1'000;
//   auto start1 = std::chrono::high_resolution_clock ::now();
//   for (size_t test = 0; test < tests_count; ++test) {
//     tree1.ModifyTree(1, n, 1);
//   }
//   auto finish1 = std::chrono::high_resolution_clock::now();
//   std::cerr << std::chrono::duration_cast<std::chrono::nanoseconds>(finish1 - start1) << "\n";
//
//
//   auto start2 = std::chrono::high_resolution_clock ::now();
//   for (size_t test = 0; test < tests_count; ++test) {
//     tree2.ModifyTree(1, n, 1);
//   }
//   auto finish2 = std::chrono::high_resolution_clock::now();
//   std::cerr << std::chrono::duration_cast<std::chrono::nanoseconds>(finish2 - start2) << "\n";
//
//   ASSERT_TRUE(true);
// }
