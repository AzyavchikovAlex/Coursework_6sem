#pragma once

#include "../abstract_mass_policy.h"

template<typename T>
class AssignSumPolicy : public AbstractMassPolicy<T, T> {
 public:
  AssignSumPolicy() = default;
  virtual ~AssignSumPolicy() = default;

  T GetNullState() override {
    return 0;
  }
  T GetState(T first_state, T second_state) override {
    return first_state + second_state;
  }
  T GetModifiedState(T /*init_state*/,
                     size_t values_count,
                     T modifier) override {
    return modifier * static_cast<T>(values_count);
  }
};
