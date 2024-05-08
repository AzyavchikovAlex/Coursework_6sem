#pragma once

#include <thread>
#include <chrono>

#include "Policies/abstract_mass_policy.h"

template<typename T>
class AssignSumSlowPolicy : public AbstractMassPolicy<T, T> {
 public:
  AssignSumSlowPolicy() = default;
  virtual ~AssignSumSlowPolicy() = default;

  T GetNullState() override {
    return 0;
  }
  T GetState(T first_state, T second_state) override {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start + duration;
    while (std::chrono::high_resolution_clock::now() < finish) {

    }
    return first_state + second_state;
  }
  T GetModifiedState(T /*init_state*/,
                     size_t values_count,
                     T modifier) override {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start + duration;
    while (std::chrono::high_resolution_clock::now() < finish) {

    }
    return modifier * static_cast<T>(values_count);
  }

  void SetDuration(uint new_duration) {
    duration = std::chrono::nanoseconds(new_duration);
  }

 private:
  std::chrono::nanoseconds duration{10'000};
};
