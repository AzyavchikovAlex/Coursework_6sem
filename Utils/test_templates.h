#pragma once

#include <random>
#include <gtest/gtest.h>
#include <chrono>

#include "SegmentTrees/ClassicTree/classic_segment_tree.h"
#include "SegmentTrees/abstract_tree.h"
#include "SegmentTrees/ClassicTree/assign_sum_tree.h"
#include "SegmentTrees/ClassicTree/classic_segment_tree.h"
#include "Policies/AssignAndSum/assign_sum_policy.h"
#include "Policies/AssignAndSum/assign_sum_slow_policy.h"
#include "Benchmarks/bench.h"

// template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount>
// std::shared_ptr<AbstractTree<int, int, AssignSumPolicy<int>>> GetTree(const std::vector<int>& values) {
//   auto strategy = AssignSumPolicy<int>();
//   return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(values.begin(), values.end(), strategy);
// }
//
// template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount, uint duration>
// std::shared_ptr<AbstractTree<int, int, AssignSumSlowPolicy<int>>> GetSlowDataTree(const std::vector<int>& values) {
//   auto strategy = AssignSumSlowPolicy<int>();
//   strategy.SetDuration(duration);
//   return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(values.begin(), values.end(), strategy);
// }

template <template<class T, class M, class P, size_t> class TreeType>
void SimpleTest() {
  {
    std::vector<int> values = {1, 2, 3};
    auto tree = GetTree<TreeType, 1>(values);
    EXPECT_EQ(tree->GetTreeState(0, 3), 6);
    EXPECT_EQ(tree->GetTreeState(0, 2), 3);
    tree->ModifyTree(0, 1, 5);
    EXPECT_EQ(tree->GetTreeState(0, 3), 10);
    EXPECT_EQ(tree->GetTreeState(0, 3), 10);
  }{
    std::vector<int> values = {1, 2, 3, 4, 5};
    auto tree = GetTree<TreeType, 2>(values);
    EXPECT_EQ(tree->GetTreeState(0, 5), 15);
    tree->ModifyTree(0, 1, 5); // {5, 2, 3, 4, 5}
    EXPECT_EQ(tree->GetTreeState(0, 3), 10);
    tree->ModifyTree(1, 3, -1); // {5, -1, -1, 4, 5}
    EXPECT_EQ(tree->GetTreeState(0, 3), 3);
    EXPECT_EQ(tree->GetTreeState(0, 5), 12);
  } {
    size_t n = 10;
    std::vector<int> values(n, 0);
    auto tree = GetTree<TreeType, 5>(values);
    size_t l = 3;
    size_t r = 9;
    int value = 2;
    tree->ModifyTree(l, r, value);
    for (size_t i = l; i < r; ++i) {
      values[i] = value;
    }
    ASSERT_EQ(tree->GetTreeState(l, r), 12);
  }
}

template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 3>
void RandomTest() {
  size_t n = 1000;
  size_t tests_count = 20000;
  std::vector<int> values(n, 0);
  auto tree = GetTree<TreeType, ThreadsCount>(values);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, (int) n - 1);

  for (size_t test = 0; test < tests_count; ++test) {
    int l = distrib(gen);
    int r = distrib(gen);
    if (l > r) std::swap(l, r);
    ++r;
    if ((test % 5) == 0) {
      // modify
      int value = distrib(gen);
      ASSERT_NO_THROW(tree->ModifyTree(l, r, value));
      for (size_t i = l; i < r; ++i) {
        values[i] = value;
      }
    }
    // state
    ASSERT_EQ(tree->GetTreeState(l, r),
              std::accumulate(std::next(values.begin(), l),
                              std::next(values.begin(), r),
                              0));
  }
}

template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 100'000'000>
void BenchmarkTest() {
  size_t n = 1e7;
  std::vector<int> values(n);
  auto tree = GetTree<TreeType, ThreadsCount>(values);
  auto fast_tree = GetTree<Tree, ThreadsCount>(values);

  auto t1 = bench::Measure([&tree, n]() {
    tree->ModifyTree(1, n - 1, 1);
    tree->GetTreeState(1, n - 1);
  }, std::chrono::nanoseconds(NanoSeconds));
  auto t2 =  bench::Measure([&fast_tree, n]() {
    fast_tree->ModifyTree(1, n - 1, 1);
    fast_tree->GetTreeState(1, n - 1);
  }, std::chrono::nanoseconds(NanoSeconds));

  std::cerr << "Testing tree: " <<  t1 << "\n";
  std::cerr << "Fast    tree: " <<  t2 << "\n";
  ASSERT_TRUE(t1 <= t2 * 2);
}

template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 100'000'000>
void SlowDataBenchmarkTest() {
  size_t n = 1e7;
  std::vector<int> values(n);
  auto tree = GetSlowDataTree<TreeType, ThreadsCount>(values);
  auto fast_tree = GetSlowDataTree<Tree, ThreadsCount>(values);

  auto t1 = bench::Measure([&tree, n]() {
    tree->ModifyTree(1, n - 1, 1);
    tree->GetTreeState(1, n - 1);
  }, std::chrono::nanoseconds(NanoSeconds));
  auto t2 =  bench::Measure([&fast_tree, n]() {
    fast_tree->ModifyTree(1, n - 1, 1);
    fast_tree->GetTreeState(1, n - 1);
  }, std::chrono::nanoseconds(NanoSeconds));

  std::cerr << "Testing tree: " <<  t1 << "\n";
  std::cerr << "Fast    tree: " <<  t2 << "\n";
  ASSERT_TRUE(t1 <= t2 * 2);
}


template <template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 100000000>
void BatchingBenchmarkTest() {
  size_t n = 2e7;
  size_t batch_size = 10;
  std::vector<int> values(n);
  auto tree = GetTree<TreeType, ThreadsCount>(values);
  auto fast_tree = GetTree<Tree, ThreadsCount>(values);

  auto t1 = bench::Measure([&tree, n, batch_size]() {
    std::vector<std::future<int>> states;
    for (size_t i = 0; i < batch_size; ++i) {
      states.emplace_back(tree->GetFutureTreeState(1, n - 2));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (auto& state: states) {
      state.get();
    }
  }, std::chrono::nanoseconds(NanoSeconds));
  auto t2 =  bench::Measure([&fast_tree, n, batch_size]() {
    std::vector<std::future<int>> states;
    for (size_t i = 0; i < batch_size; ++i) {
      states.emplace_back(fast_tree->GetFutureTreeState(1, n - 2));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (auto& state: states) {
      state.get();
    }
  }, std::chrono::nanoseconds(NanoSeconds));

  std::cerr << "Testing tree: " <<  t1 << "\n";
  std::cerr << "Fast    tree: " <<  t2 << "\n";
  ASSERT_TRUE(t1 <= t2 * 2);
}
