#pragma once

#include <optional>
#include <queue>
#include <cstdint>
#include <mutex>
#include <condition_variable>

template <class T>
class BufferedChannel {
 public:
  explicit BufferedChannel(uint32_t size) : max_size_(size) {
  }

  template <typename K>
  void Send(K&& value) {
    std::unique_lock lock(global_);
    send_cv_.wait(lock, [this]() { return is_closed_ || queue_.size() < max_size_; });
    if (is_closed_) {
      throw std::runtime_error("Channel is closed");
    }
    queue_.push(std::forward<K>(value));
    receive_cv_.notify_one();
  }

  std::optional<T> Recv() {
    std::unique_lock lock(global_);
    receive_cv_.wait(lock, [this]() { return !queue_.empty() || is_closed_; });
    if (queue_.empty()) {
      return std::nullopt;
    }
    T result(std::move(queue_.front()));
    queue_.pop();
    send_cv_.notify_one();
    return result;
  }

  void Close() {
    std::lock_guard lock(global_);
    is_closed_ = true;
    send_cv_.notify_all();
    receive_cv_.notify_all();
  }

 private:
  std::mutex global_;
  std::condition_variable send_cv_, receive_cv_;
  std::queue<T> queue_;
  const uint32_t max_size_{1};
  bool is_closed_{false};
};
