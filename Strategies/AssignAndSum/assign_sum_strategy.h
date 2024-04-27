#pragma once

#include "../abstract_mass_strategy.h"

template<typename T>
class AssignSumStrategy : public AbstractMassPolicy<T, T> {
 public:
  AssignSumStrategy() = default;
  ~AssignSumStrategy() = default;

  T GetNullState() override {
    return 0;
  }
  T GetState(T first_state, T second_state) override {
    return first_state + second_state;
  }
  T GetModifiedState(T init_state,
                     size_t values_count,
                     T modifier) override {
    return modifier * static_cast<T>(values_count);
  }
};
