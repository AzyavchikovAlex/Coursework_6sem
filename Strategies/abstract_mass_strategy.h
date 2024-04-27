#pragma once

template <typename T, typename M>
class AbstractMassPolicy {
 public:
  AbstractMassPolicy() = default;
  virtual ~AbstractMassPolicy() = default;

  // GetState(state, GetNullState()) == state
  virtual T GetState(T first_state, T second_state) = 0;
  virtual T GetNullState() = 0;
  virtual T GetModifiedState(T init_state, size_t values_count, M modifier) = 0;
};
