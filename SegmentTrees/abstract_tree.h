#pragma once

#include <vector>
#include <memory>

#include "../Policies/abstract_mass_policy.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 0>
class AbstractTree {
 public:
  explicit AbstractTree(P policy) : policy_(policy) {}
  virtual ~AbstractTree() = default;

  virtual void ModifyTree(size_t l, size_t r, M modifier) = 0;

  virtual T GetTreeState(size_t l, size_t r) = 0;

 protected:
  P policy_;
};