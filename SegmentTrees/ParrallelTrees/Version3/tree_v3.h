#pragma once

#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#include "Policies/abstract_mass_policy.h"
#include "SegmentTrees/abstract_tree.h"
#include "SegmentTrees/ParrallelTrees/Version1/tree_v1.h"
#include "Utils/buffered_channel.h"
#include "Utils/fast_queue.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 8>
class ParallelSegmentTree_V3 : public AbstractTree<T, M, P> {
 private:
  struct Query;
 public:
  template<class Iter>
  explicit ParallelSegmentTree_V3(Iter begin, Iter end, P policy)
      : AbstractTree<T, M, P>(std::move(policy)) {
    if (std::distance(begin, end) < ThreadsCount) {
      throw std::runtime_error("too small array");
    }
    size_t chunk_size = std::distance(begin, end) / ThreadsCount;
    for (size_t index = 0; index < ThreadsCount; ++index) {
      size_t l_border = index * chunk_size;
      size_t r_border =
          index + 1 == ThreadsCount ? std::distance(begin, end) : (index + 1)
              * chunk_size;
      auto tree =
          std::make_shared<ParallelSegmentTree_V1<T, M, P>>(std::next(begin,
                                                                      l_border),
                                                            std::next(begin,
                                                                      r_border),
                                                            policy);
      channels_[index] = std::make_shared<MPMCBoundedQueue<Query>>(kBufferSize);
      threads_pool_.emplace_back(&ParallelSegmentTree_V3::RunThread,
                                 this,
                                 tree,
                                 channels_[index],
                                 l_border,
                                 r_border);
    }
  }

  virtual  ~ParallelSegmentTree_V3() {
    stop_threads.test_and_set();
    for (size_t index = 0; index < ThreadsCount; ++index) {
      threads_pool_[index].join();
    }
    threads_pool_.clear();
  }

  void ModifyTree(size_t l, size_t r, M modifier) override {
    all_tasks_completed.clear();
    Query q = {modifier, l, r};
    for (size_t index = 0; index < ThreadsCount; ++index) {
      while (!channels_[index]->Enqueue(q)) {

      }
    }
  }

  T GetTreeState(size_t l, size_t r) override {
    last_asked_state_ = this->policy_.GetNullState();
    counter_ = 0;
    all_tasks_completed.clear();
    Query q = {std::nullopt, l, r};
    for (size_t index = 0; index < ThreadsCount; ++index) {
      while (!channels_[index]->Enqueue(q)) {
      }
    }
    while (!all_tasks_completed.test()) {

    }
    return last_asked_state_;
  }

 private:
  struct Query {
    std::optional<M> modifier;
    size_t l, r;
  };
  std::vector<std::thread> threads_pool_;
  static constexpr size_t kBufferSize = 1;
  std::array<std::shared_ptr<MPMCBoundedQueue<Query>>, ThreadsCount> channels_;
  std::atomic<T> last_asked_state_;
  std::atomic_int counter_;
  std::atomic_flag all_tasks_completed = ATOMIC_FLAG_INIT;
  std::atomic_flag stop_threads = ATOMIC_FLAG_INIT;

  void FinishTask() {
    auto value = counter_.fetch_add(1) + 1;
    if (value == ThreadsCount) {
      all_tasks_completed.test_and_set();
    }
  }

  void RunThread(std::shared_ptr<AbstractTree<T, M, P>> tree,
                 std::shared_ptr<MPMCBoundedQueue<Query>> channel,
                 size_t l_border,
                 size_t r_border) {
    for (;;) {
      Query query;
      while (!channel->Dequeue(query)) {
        if (stop_threads.test()) {
          return;
        }
      }
      auto l = std::max(query.l, l_border);
      auto r = std::min(query.r, r_border);
      if (l >= r) {
        if (!query.modifier.has_value()) {
          FinishTask();
        }
        continue;
      }
      l -= l_border;
      r -= l_border;
      if (query.modifier.has_value()) {
        tree->ModifyTree(l, r, query.modifier.value());
        continue;
      }

      auto sub_state = tree->GetTreeState(l, r);
      for (auto old_state = last_asked_state_.load();;) {
        auto new_state = this->policy_.GetState(old_state, sub_state);
        if (last_asked_state_.compare_exchange_strong(old_state,
                                                      new_state)) {
          FinishTask();
          break;
        }
      }
    }
  }

};