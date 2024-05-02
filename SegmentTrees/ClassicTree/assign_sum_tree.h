#pragma once

#include <vector>
#include <cstdio>

class ASTree {
 private:
  struct Node;
 public:
  explicit ASTree(const std::vector<int>& values) {
    size_ = ProperSize(values.size());
    tree_.resize(size_ << 1);
    Build(values, 0, 0, size_);
  }

  void ModifyTree(size_t l, size_t r, int modifier) {
    Modify(l, r, modifier, 0, 0, size_);
  }

  int GetTreeState(size_t l, size_t r) {
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
    auto& node = tree_[pos];
    if (!node.has_operation) {
      return;
    }
    if (r - l == 1) {
      node.state = node.modifier;
      node.has_operation = false;
      return;
    }
    size_t mid = node.mid;
    size_t left = node.left;
    size_t right = node.right;
    auto& left_node = tree_[left];
    auto& right_node = tree_[right];
    left_node.state = node.modifier * (int) (mid - l);
    left_node.modifier = node.modifier;
    left_node.has_operation = true;

    right_node.state = node.modifier * (int) (r - mid);
    right_node.modifier = node.modifier;
    right_node.has_operation = true;

    node.state = left_node.state + right_node.state;
    node.has_operation = false;
  }

  void Modify(size_t searching_l, size_t searching_r, int modifier,
              size_t pos, size_t l, size_t r) {
    if (searching_l == l && searching_r == r) {
      tree_[pos].state = modifier * (int) (r - l);
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
    tree_[pos].state = tree_[left].state + tree_[right].state;
  }

  int GetState(size_t searching_l, size_t searching_r,
             size_t pos, size_t l, size_t r) {
    Propagate(pos, l, r);
    if (searching_l == l && searching_r == r) {
      return tree_[pos].state;
    }
    size_t mid = tree_[pos].mid;
    size_t left = tree_[pos].left;
    size_t right = tree_[pos].right;
    if (searching_r <= mid) {
      return GetState(searching_l, searching_r, left, l, mid);
    }
    if (searching_l >= mid) {
      return GetState(searching_l, searching_r, right, mid, r);
    }
    return GetState(searching_l, mid, left, l, mid) +
        GetState(mid, searching_r, right, mid, r);
  }

  void Build(const std::vector<int>& values, size_t pos, size_t l, size_t r) {
    if (r - l == 1) {
      if (l < values.size()) {
        tree_[pos].state = values[l];
      }
      return;
    }
    auto& node = tree_[pos];
    node.mid = (l + r) >> 1;
    node.left = (pos << 1) + 1;
    node.right = node.left + 1;
    Build(values, node.left, l, node.mid);
    Build(values, node.right, node.mid, r);
    node.state = tree_[node.left].state + tree_[node.right].state;
  }

  std::vector<Node> tree_;
  size_t size_;
  struct Node {
    int state{0};
    int modifier{0};
    size_t mid{0}, left{0}, right{0};
    bool has_operation{false};
  };
};