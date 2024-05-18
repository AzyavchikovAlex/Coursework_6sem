#pragma once

#include <vector>
#include <memory>
#include <future>

#include "Policies/abstract_mass_policy.h"

template<typename T, typename M, typename P, size_t /*ThreadsCount*/ = 0>
class AbstractTree {
 public:
  AbstractTree() = default;
  explicit AbstractTree(P policy, size_t size)
      : policy_(policy), array_size_(size) {}
  virtual ~AbstractTree() = default;

  virtual void ModifyTree(size_t l, size_t r, M modifier) = 0;

  virtual T GetTreeState(size_t l, size_t r) = 0;

  virtual std::future<T> GetFutureTreeState(size_t l, size_t r) {
    std::promise<T> result;
    result.set_value(GetTreeState(l, r));
    return result.get_future();
  }

  T GetState(T first_state, T second_state) {
    return policy_.GetState(first_state, second_state);
  }
  T GetNullState() {
    return policy_.GetNullState();
  }
  T GetModifiedState(T init_state, size_t values_count, M modifier) {
    return policy_.GetModifiedState(init_state, values_count, modifier);
  }

  size_t Size() const {
    return array_size_;
  }

 protected:
  P policy_;

 private:
  size_t array_size_{};
};