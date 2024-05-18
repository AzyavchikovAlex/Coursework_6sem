#pragma once

#include <atomic>
#include <vector>
#include <thread>

template<class T>
class FastQueue {
 private:
  struct Node;

 public:
  explicit FastQueue(size_t size) : queue_(size), mod_(size - 1) {
    shift_ = 0;
    while (size > 1) {
      ++shift_;
      size >>= 1;
    }
  }

  bool Enqueue(const T& value) {
    for (uint64_t push_index = push_index_.load(std::memory_order_acquire);;) {
      auto& node = queue_[push_index & mod_];
      uint64_t expected_epoch = (push_index >> shift_) << 1;
      if (expected_epoch == node.epoch.load(std::memory_order_acquire)) {
        if (push_index_.compare_exchange_strong(push_index, push_index + 1)) {
          node.value = value;
          node.epoch.fetch_add(1, std::memory_order_release);
          return true;
        }
        std::this_thread::yield();
      } else {
        uint64_t old_push_index = push_index;
        push_index = push_index_.load(std::memory_order_acquire);
        if (old_push_index == push_index) {
          return false;
        }
      }
    }
  }

  bool Dequeue(T& data) {
    for (uint64_t pop_index = pop_index_.load(std::memory_order_acquire);;) {
      auto& node = queue_[pop_index & mod_];
      uint64_t expected_epoch = 1 + ((pop_index >> (shift_) << 1));
      if (expected_epoch == node.epoch.load(std::memory_order_acquire)) {
        if (pop_index_.compare_exchange_strong(pop_index, pop_index + 1)) {
          data = std::move(node.value);
          node.epoch.fetch_add(1, std::memory_order_release);
          return true;
        }
        std::this_thread::yield();
      } else {
        uint64_t old_pop_index = pop_index;
        pop_index = pop_index_.load(std::memory_order_acquire);
        if (old_pop_index == pop_index) {
          return false;
        }
      }
    }
  }

 private:
  struct Node {
    char pad1[64];
    std::atomic<uint64_t> epoch;
    char pad2[64];
    T value;
    char pad3[64];
  };
  char pad1_[1024];
  std::vector<Node> queue_;
  char pad2_[1024];
  std::atomic<uint64_t> push_index_ = 0;
  char pad3_[1024];
  std::atomic<uint64_t> pop_index_ = 0;
  char pad4_[1024];
  const uint64_t mod_;
  char pad5_[1024];
  size_t shift_;
  char pad6_[1024];
};
