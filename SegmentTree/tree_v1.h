#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <mutex>

#include "../Strategies/abstract_mass_strategy.h"

template<typename T, typename M>
class ParallelSegmentTree {
 private:
  struct Node;
 public:
  explicit ParallelSegmentTree(const std::vector<T>& values,
                std::shared_ptr<AbstractMassPolicy<T, M>>&& strategy)
      : strategy_(strategy) {
    // TODO: need mutex?
    size_ = ProperSize(values.size());
    tree_ = std::vector<Node>(2 * size_, {strategy->GetNullState(), M(), false});
    mutexes_ = std::vector<std::mutex>(2 * size_);
    Build(values, 0, 0, size_);
  }

  void ModifyTree(size_t l, size_t r, M modifier) {
    assert(l < r);
    std::lock_guard<std::mutex> lock(mutexes_.front());
    Modify(l, r, modifier, 0, 0, size_);
  }

  T GetTreeState(size_t l, size_t r) {
    return GetState(l, r, 0, 0, size_);
  }

 private:
  static size_t ProperSize(size_t size) {
    size_t proper_size = 1;
    while (proper_size < size) {
      proper_size <<= 1;
    }
    return proper_size;
  }

  void Propagate(size_t pos, size_t l, size_t r) {
    assert(l < r);
    assert(pos >= 0 && pos < tree_.size());
    auto& node = tree_[pos];
    if (!node.has_operation) {
      return;
    }
    if (r - l == 1) {
      node.state = strategy_->GetModifiedState(node.state, 1, node.modifier);
      node.has_operation = false;
      return;
    }
    assert(pos * 2 + 2 < tree_.size());
    size_t mid = (r + l) >> 1;
    size_t left = (pos << 1) + 1;
    auto& left_node = tree_[left];
    size_t right = left + 1;
    auto& right_node = tree_[right];
    left_node.state =
        strategy_->GetModifiedState(left_node.state, mid - l, node.modifier);
    left_node.modifier = node.modifier;
    left_node.has_operation = true;

    right_node.state =
        strategy_->GetModifiedState(right_node.state, r - mid, node.modifier);
    right_node.modifier = node.modifier;
    right_node.has_operation = true;

    node.state = strategy_->GetState(left_node.state, right_node.state);
    node.has_operation = false;
  }

  void Modify(size_t searching_l, size_t searching_r, M modifier,
              size_t pos, size_t l, size_t r) {
    assert(l < r);
    assert(searching_l >= l && searching_r <= r);
    // std::lock_guard<std::mutex> lock(mutexes_[pos]);
    if (searching_l == l && searching_r == r) {
      tree_[pos].state = strategy_->GetModifiedState(tree_[pos].state, r - l, modifier);
      tree_[pos].modifier = modifier;
      tree_[pos].has_operation = true;
      return;
    }
    Propagate(pos, l, r);
    size_t mid = (l + r) >> 1;
    size_t left = (pos << 1) + 1;
    size_t right = left + 1;
    if (searching_r <= mid) {
      Modify(searching_l, searching_r, modifier, left, l, mid);
    } else if (searching_l >= mid) {
      Modify(searching_l, searching_r, modifier, right, mid, r);
    } else {
      Modify(searching_l, mid, modifier, left, l, mid);
      Modify(mid, searching_r, modifier, right, mid, r);
    }
    tree_[pos].state =
        strategy_->GetState(tree_[left].state, tree_[right].state);
  }

  T GetState(size_t searching_l, size_t searching_r,
             size_t pos, size_t l, size_t r) {
    assert(l < r);
    assert(searching_l >= l && searching_r <= r);
    // std::lock_guard<std::mutex> lock(mutexes_[pos]);
    Propagate(pos, l, r);
    if (searching_l == l && searching_r == r) {
      return tree_[pos].state;
    }
    size_t mid = (l + r) >> 1;
    size_t left = (pos << 1) + 1;
    size_t right = left + 1;
    if (searching_r <= mid) {
      return GetState(searching_l, searching_r, left, l, mid);
    }
    if (searching_l >= mid) {
      return GetState(searching_l, searching_r, right, mid, r);
    }
    return strategy_->GetState(GetState(searching_l, mid, left, l, mid),
                               GetState(mid, searching_r, right, mid, r));
  }

  void Build(const std::vector<T>& values, size_t pos, size_t l, size_t r) {
    assert (l < r);
    if (r - l == 1) {
      if (l < values.size()) {
        tree_[pos].state = values[l];
      }
      return;
    }
    size_t mid = (l + r) >> 1;
    size_t left = (pos << 1) + 1;
    size_t right = left + 1;
    Build(values, left, l, mid);
    Build(values, right, mid, r);
    tree_[pos].state =
        strategy_->GetState(tree_[left].state, tree_[right].state);
  }

  std::vector<Node> tree_;
  std::vector<std::mutex> mutexes_;
  std::shared_ptr<AbstractMassPolicy<T, M>> strategy_;
  size_t size_;
  struct Node {
    T state;
    M modifier;
    bool has_operation{false};
  };
};