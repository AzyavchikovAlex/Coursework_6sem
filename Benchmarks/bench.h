#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <functional>
#include <ranges>
#include <cassert>

#include "Policies/abstract_mass_policy.h"
#include "Policies/AssignAndSum/assign_sum_policy.h"
#include "Policies/AssignAndSum/assign_sum_slow_policy.h"
#include "SegmentTrees/ClassicTree/classic_segment_tree.h"

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount>
std::shared_ptr<AbstractTree<int, int, AssignSumPolicy<int>>>
GetTree(const std::vector<int>& values) {
  auto strategy = AssignSumPolicy<int>();
  return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(
      values.begin(),
      values.end(),
      strategy);
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount>
std::shared_ptr<AbstractTree<int, int, AssignSumSlowPolicy<int>>>
GetSlowDataTree(const std::vector<int>& values) {
  auto strategy = AssignSumSlowPolicy<int>();
  return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(
      values.begin(),
      values.end(),
      strategy);
}

namespace bench {

constexpr const std::array<int, size_t(2e7 + 1)> buffer{};

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount>
std::shared_ptr<AbstractTree<int, int, AssignSumPolicy<int>>>
GetTree(size_t size) {
  assert(size < buffer.size());
  auto strategy = AssignSumPolicy<int>();
  return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(
      buffer.begin(),
      std::next(buffer.begin(), size),
      strategy);
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount>
std::shared_ptr<AbstractTree<int, int, AssignSumSlowPolicy<int>>>
GetSlowDataTree(size_t size) {
  assert(size < buffer.size());
  auto strategy = AssignSumSlowPolicy<int>();
  return std::make_shared<TreeType<int, int, decltype(strategy), ThreadsCount>>(
      buffer.begin(),
      std::next(buffer.begin(), size),
      strategy);
}

typedef std::chrono::high_resolution_clock clock;
// typedef std::chrono::system_clock clock;
typedef std::chrono::nanoseconds duration;

duration Measure(const std::function<void()>& command,
                 duration time_limit = std::chrono::seconds(2));

std::vector<duration>
MeasureWithBaseline(const std::vector<size_t>& sizes,
                    auto&& get_tree,
                    auto&& tree_operation) {
  std::vector<duration> result;
  for (size_t size : sizes) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto tree = get_tree(size);
    auto time = Measure([&]() { tree_operation(tree); },
                        std::chrono::milliseconds(2000));
    result.push_back(time);
    std::cerr << std::setw(9) << size << std::setw(10) << time.count() << "\n";
  }
  return result;
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 1'000'000'000>
std::vector<duration> ModifyOnFastData(const std::vector<size_t>& sizes) {
  return MeasureWithBaseline(sizes,
                             [](size_t size) {
                               return GetTree<TreeType, ThreadsCount>(size);
                             },
                             [](auto&& tree) {
                               tree->ModifyTree(1, tree->Size() - 1, 1);
                             });
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 3'000'000'000>
std::vector<duration> ModifyOnSlowData(const std::vector<size_t>& sizes) {
  return MeasureWithBaseline(sizes,
                             [](size_t size) {
                               return GetSlowDataTree<TreeType, ThreadsCount>(
                                   size);
                             },
                             [](auto&& tree) {
                               tree->ModifyTree(1, tree->Size() - 1, 1);
                             });
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 1'000'000'000>
std::vector<duration> GetStateOnFastData(const std::vector<size_t>& sizes) {
  return MeasureWithBaseline(sizes,
                             [](size_t size) {
                               return GetTree<TreeType, ThreadsCount>(size);
                             },
                             [](auto&& tree) {
                               tree->GetTreeState(1, tree->Size() - 1);
                             });
}

template<template<class T, class M, class P, size_t> class TreeType, size_t ThreadsCount = 8, int64_t NanoSeconds = 3'000'000'000>
std::vector<duration> GetStateOnSlowData(const std::vector<size_t>& sizes) {
  return MeasureWithBaseline(sizes,
                             [](size_t size) {
                               return GetSlowDataTree<TreeType, ThreadsCount>(
                                   size);
                             },
                             [](auto&& tree) {
                               tree->GetTreeState(1, tree->Size() - 1);
                             });
}

} // namespace bench
