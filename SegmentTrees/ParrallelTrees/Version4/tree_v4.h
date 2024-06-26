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
#include "SegmentTrees/ParrallelTrees/Version1/tree_v1.h"
#include "Utils/buffered_channel.h"

template<typename T, typename M, typename P, size_t SegmentsCount = 8>
class ParallelSegmentTree_V4 : public AbstractTree<T, M, P> {
 private:
  struct Query;
 public:
  template<class Iter>
  explicit ParallelSegmentTree_V4(Iter begin, Iter end, P policy)
      : AbstractTree<T, M, P>(std::move(policy), std::distance(begin, end)) {
    if (this->Size() < kThreadsCount) {
      throw std::runtime_error("too small array");
    }
    size_t chunk_size = this->Size() / SegmentsCount;
    for (size_t index = 0; index < SegmentsCount; ++index) {
      size_t l_border = index * chunk_size;
      size_t r_border =
          index + 1 == SegmentsCount ? std::distance(begin, end) : (index + 1)
              * chunk_size;
      trees_[index] = Tree<T, M, P>(std::next(begin, l_border),
                                    std::next(begin, r_border),
                                    this->policy_);
    }
    threads_pool_.emplace_back(&ParallelSegmentTree_V4::RunThread,
                               this,
                               0,
                               chunk_size);
    threads_pool_.emplace_back(&ParallelSegmentTree_V4::RunThread,
                               this,
                               1,
                               chunk_size);
    threads_pool_.emplace_back(&ParallelSegmentTree_V4::RunThread,
                               this,
                               2,
                               chunk_size);
  }

  virtual  ~ParallelSegmentTree_V4() {
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
  static constexpr size_t kThreadsCount = 3;
  std::array<Tree<T, M, P>, SegmentsCount> trees_{Tree<T, M, P>{}};
  std::vector<std::thread> threads_pool_;
  Query last_query_{};
  std::atomic<int64_t> last_query_id_{0};
  std::atomic<T> last_asked_state_;
  std::atomic_int counter_;
  std::atomic_flag all_tasks_finished = ATOMIC_FLAG_INIT;
  std::atomic_flag stop_threads_ = ATOMIC_FLAG_INIT;

  void StartTask() {
    counter_ = 0;
    all_tasks_finished.clear();
    ++last_query_id_;
  }

  void WaitTask() {
    while (!all_tasks_finished.test()) {}
  }

  void FinishTask() {
    auto value = 1 + counter_.fetch_add(1);
    if (value == kThreadsCount) {
      all_tasks_finished.test_and_set();
    }
  }

  void RunThread(size_t thread_id, size_t chunk_size) {
    for (int64_t current_task_id = 1;; ++current_task_id, FinishTask()) {
      while (last_query_id_ != current_task_id) {
        if (stop_threads_.test()) {
          return;
        }
      }
      Query query = last_query_;
      auto l = query.l;
      auto r = query.r;
      if (l >= r) {
        continue;
      }
      size_t first_tree_id = l / chunk_size;
      size_t last_tree_id = r / chunk_size;
      if (last_tree_id >= SegmentsCount || r % chunk_size == 0) {
        --last_tree_id;
      }
      if (thread_id == 0) {
          size_t zero_pos = first_tree_id * chunk_size;
          if (last_tree_id == first_tree_id) {
            ProcessQuery(query, first_tree_id, l - zero_pos, r - zero_pos);
          } else {
            ProcessQuery(query, first_tree_id, l - zero_pos, chunk_size);
          }
        } else if (thread_id == 2) {
          if (first_tree_id != last_tree_id) {
            size_t zero_pos = last_tree_id * chunk_size;
            ProcessQuery(query, last_tree_id, 0, r - zero_pos);
          }
        } else if (thread_id == 1) {
          if (query.modifier.has_value()) {
            for (size_t tree_index = first_tree_id + 1;
                 tree_index + 1 <= last_tree_id;
                 ++tree_index) {
              trees_[tree_index].ModifyTree(0, chunk_size, query.modifier.value());
            }
            continue;
          }
          T result = this->GetNullState();
          for (size_t tree_index = first_tree_id + 1;
               tree_index + 1 <= last_tree_id;
               ++tree_index) {
            result = trees_[tree_index].GetState(result,
                                                 trees_[tree_index].GetTreeState(0,
                                                                                 chunk_size));
          }
          for (auto old_state = last_asked_state_.load();;) {
            auto new_state = trees_[first_tree_id + 1].GetState(old_state, result);
            if (last_asked_state_.compare_exchange_strong(old_state,
                                                          new_state)) {
              break;
            }
          }
        }
    }
  }


  void ProcessQuery(Query query, size_t tree_index, size_t l, size_t r) {
    if (query.modifier.has_value()) {
      trees_[tree_index].ModifyTree(l, r, query.modifier.value());
      return;
    }

    auto sub_state = trees_[tree_index].GetTreeState(l, r);
    for (auto old_state = last_asked_state_.load();;) {
      auto new_state = trees_[tree_index].GetState(old_state, sub_state);
      if (last_asked_state_.compare_exchange_strong(old_state,
                                                    new_state)) {
        break;
      }
    }
  }

};