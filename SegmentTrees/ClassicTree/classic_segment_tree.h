#pragma once

#include <vector>
#include <memory>

#include "Policies/abstract_mass_policy.h"
#include "SegmentTrees/abstract_tree.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 0>
class Tree : public AbstractTree<T, M, P> {
 private:
  struct Node;
 public:
  Tree() = default;
  template<class Iter>
  explicit Tree(Iter begin, Iter end, P policy)
      : AbstractTree<T, M, P>(std::move(policy), std::distance(begin, end)), size_(ProperSize(this->Size())) {
    tree_ = std::vector<Node>(2 * size_, {this->GetNullState(), M(), false});
    Build(begin, this->Size(), 0, 0, size_);
  }

  void ModifyTree(size_t l, size_t r, M modifier) override {
    Modify(l, r, modifier, 0, 0, size_);
  }

  T GetTreeState(size_t l, size_t r) override {
    return GetTreeState(l, r, 0, 0, size_);
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
    auto& node = tree_[pos];
    if (!node.has_operation) {
      return;
    }
    if (r - l == 1) {
      node.state =
          this->GetModifiedState(node.state, 1, node.modifier);
      node.has_operation = false;
      return;
    }
    size_t mid = (r + l) >> 1;
    size_t left = (pos << 1) + 1;
    auto& left_node = tree_[left];
    size_t right = left + 1;
    auto& right_node = tree_[right];
    left_node.state =
        this->GetModifiedState(left_node.state,
                                       mid - l,
                                       node.modifier);
    left_node.modifier = node.modifier;
    left_node.has_operation = true;

    right_node.state =
        this->GetModifiedState(right_node.state,
                                       r - mid,
                                       node.modifier);
    right_node.modifier = node.modifier;
    right_node.has_operation = true;

    node.state = this->GetState(left_node.state, right_node.state);
    node.has_operation = false;
  }

  void Modify(size_t searching_l, size_t searching_r, M modifier,
              size_t pos, size_t l, size_t r) {
    if (searching_l == l && searching_r == r) {
      tree_[pos].state =
          this->GetModifiedState(tree_[pos].state, r - l, modifier);
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
        this->GetState(tree_[left].state, tree_[right].state);
  }

  T GetTreeState(size_t searching_l, size_t searching_r,
             size_t pos, size_t l, size_t r) {
    Propagate(pos, l, r);
    if (searching_l == l && searching_r == r) {
      return tree_[pos].state;
    }
    size_t mid = (l + r) >> 1;
    size_t left = (pos << 1) + 1;
    size_t right = left + 1;
    if (searching_r <= mid) {
      return GetTreeState(searching_l, searching_r, left, l, mid);
    }
    if (searching_l >= mid) {
      return GetTreeState(searching_l, searching_r, right, mid, r);
    }
    return this->GetState(GetTreeState(searching_l, mid, left, l, mid),
                                  GetTreeState(mid, searching_r, right, mid, r));
  }

  template<class Iter>
  void Build(Iter begin, size_t array_size, size_t pos, size_t l, size_t r) {
    if (r - l == 1) {
      if (l < array_size) {
        tree_[pos].state = *std::next(begin, l);
      }
      return;
    }
    size_t mid = (l + r) >> 1;
    size_t left = (pos << 1) + 1;
    size_t right = left + 1;
    Build(begin, array_size, left, l, mid);
    Build(begin, array_size, right, mid, r);
    tree_[pos].state =
        this->GetState(tree_[left].state, tree_[right].state);
  }

  std::vector<Node> tree_;
  size_t size_;
  struct Node {
    T state;
    M modifier;
    bool has_operation{false};
  };
  char pad_[1024];
};