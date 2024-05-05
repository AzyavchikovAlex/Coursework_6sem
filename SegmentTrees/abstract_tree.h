#pragma once

#include <vector>
#include <memory>
#include <future>

#include "Policies/abstract_mass_policy.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 0>
class AbstractTree {
 public:
  explicit AbstractTree(P policy) : policy_(policy) {}
  virtual ~AbstractTree() = default;

  virtual void ModifyTree(size_t l, size_t r, M modifier) = 0;

  virtual T GetTreeState(size_t l, size_t r) = 0;

  virtual std::future<T> GetFutureTreeState(size_t l, size_t r)  {
    std::promise<T> result;
    result.set_value(GetTreeState(l, r));
    return result.get_future();
  }

 protected:
  P policy_;
};