#pragma once

#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#include "Policies/abstract_mass_policy.h"
#include "SegmentTrees/abstract_tree.h"
#include "SegmentTrees/ClassicTree/classic_segment_tree.h"
#include "SegmentTrees/ParrallelTrees/Version4/tree_v4.h"
#include "Utils/buffered_channel.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 2, size_t InnerThreadsCount = 2>
class ParallelSegmentTree_V3_modified : public AbstractTree<T, M, P> {
 private:
  struct Query;
 public:
  template<class Iter>
  explicit ParallelSegmentTree_V3_modified(Iter begin, Iter end, P policy)
      : AbstractTree<T, M, P>(std::move(policy), std::distance(begin, end)) {
    if (this->Size() < ThreadsCount) {
      throw std::runtime_error("too small array");
    }
    size_t chunk_size = this->Size() / ThreadsCount;
    for (size_t index = 0; index < ThreadsCount; ++index) {
      size_t l_border = index * chunk_size;
      size_t r_border =
          index + 1 == ThreadsCount ? std::distance(begin, end) : (index + 1)
              * chunk_size;
      auto tree =
          std::make_shared<ParallelSegmentTree_V3<T, M, P, InnerThreadsCount>>(std::next(begin, l_border),
                                                               std::next(begin, r_border),
                                                               this->policy_);
      threads_pool_.emplace_back([this, tree = std::move(tree),
                                     index, l_border, r_border,
                                     tasks_count = ThreadsCount]() {
        for (int64_t current_task_id = 1;; ++current_task_id, FinishTask()) {
          while (last_query_id_ != current_task_id) {
            if (stop_threads_.test()) {
              return;
            }
          }
          auto l = std::max(last_query_.l, l_border);
          auto r = std::min(last_query_.r, r_border);
          if (l >= r) {
            continue;
          }
          l -= l_border;
          r -= l_border;
          if (last_query_.modifier.has_value()) {
            tree->ModifyTree(l, r, last_query_.modifier.value());
            continue;
          }

          auto sub_state = tree->GetTreeState(l, r);
          for (auto old_state = last_asked_state_.load();;) {
            auto new_state = this->GetState(old_state, sub_state);
            if (last_asked_state_.compare_exchange_strong(old_state,
                                                          new_state)) {
              break;
            }
          }
        }
      });
    }
  }

  virtual  ~ParallelSegmentTree_V3_modified() {
    stop_threads_.test_and_set();
    for (auto& thread : threads_pool_) {
      thread.join();
    }
    threads_pool_.clear();
  }

  void ModifyTree(size_t l, size_t r, M modifier) override {
    last_query_ = {modifier, l, r};
    StartTask();
    WaitTask();
  }

  T GetTreeState(size_t l, size_t r) override {
    last_query_ = {std::nullopt, l, r};
    last_asked_state_ = this->GetNullState();
    StartTask();
    WaitTask();
    return last_asked_state_;
  }

 private:
  struct Query {
    std::optional<M> modifier{std::nullopt};
    size_t l{0}, r{0};
  };
  std::vector<std::thread> threads_pool_;
  Query last_query_{};
  std::atomic<int64_t> last_query_id_{0};
  std::atomic<T> last_asked_state_;
  std::atomic_int counter_;
  std::atomic_flag stop_threads_ = ATOMIC_FLAG_INIT;

  void StartTask() {
    counter_ = 0;
    ++last_query_id_;
  }

  void WaitTask() {
    while (counter_ < ThreadsCount) {

    }
  }

  void FinishTask() {
    counter_.fetch_add(1);
  }

};