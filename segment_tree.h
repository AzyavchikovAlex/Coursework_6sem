#pragma once

#include <cassert>
#include <vector>

template<typename T>
class Tree {
 private:
  struct Node;
 public:
  explicit Tree(const std::vector<T>& values) {
    size_ = ProperSize(values.size());
    tree_.resize(2 * size_);
    Build(values, 0, 0, size_);
  }

  void Assign(size_t l, size_t r, T value) {
    assert(l < r);
    Modify(l, r, value, 0, 0, size_);
  }

  T Sum(size_t l, size_t r) {
    return Calc(l, r, 0, 0, size_);
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
    if (!tree_[pos].has_operation) {
      return;
    }
    if (r - l == 1) {
      tree_[pos].sum = tree_[pos].modify;
      tree_[pos].has_operation = false;
      return;
    }
    assert(pos * 2 + 2 < tree_.size());
    size_t mid = (r + l) >> 1;
    tree_[pos * 2 + 1].sum = tree_[pos].modify * (mid - l);
    tree_[pos * 2 + 1].modify = tree_[pos].modify;
    tree_[pos * 2 + 1].has_operation = true;

    tree_[pos * 2 + 2].sum = tree_[pos].modify * (r - mid);
    tree_[pos * 2 + 2].modify = tree_[pos].modify;
    tree_[pos * 2 + 2].has_operation = true;
    tree_[pos].sum = tree_[pos * 2 + 1].sum + tree_[pos * 2 + 2].sum;
    tree_[pos].has_operation = false;
  }

  void Modify(size_t searching_l, size_t searching_r, T value,
              size_t pos, size_t l, size_t r) {
    assert(l < r);
    assert(searching_l >= l && searching_r <= r);
    if (searching_l == l && searching_r == r) {
      tree_[pos].sum = value * (r - l);
      tree_[pos].modify = value;
      tree_[pos].has_operation = true;
      return;
    }
    Propagate(pos, l, r);
    size_t m = (l + r) >> 1;
    if (searching_r <= m) {
      Modify(searching_l, searching_r, value, pos * 2 + 1, l, m);
    } else if (searching_l >= m) {
      Modify(searching_l, searching_r, value, pos * 2 + 2, m, r);
    } else {
      Modify(searching_l, m, value, pos * 2 + 1, l, m);
      Modify(m, searching_r, value, pos * 2 + 2, m, r);
    }
    tree_[pos].sum = tree_[pos * 2 + 1].sum + tree_[pos * 2 + 2].sum;
  }

  T Calc(size_t searching_l, size_t searching_r,
         size_t pos, size_t l, size_t r) {
    assert(l < r);
    assert(searching_l >= l && searching_r <= r);
    Propagate(pos, l, r);
    if (searching_l == l && searching_r == r) {
      return tree_[pos].sum;
    }
    size_t mid = (l + r) >> 1;
    if (searching_r <= mid) {
      return Calc(searching_l, searching_r, pos * 2 + 1, l, mid);
    }
    if (searching_l >= mid) {
      return Calc(searching_l, searching_r, pos * 2 + 2, mid, r);
    }
    T sum1 = Calc(searching_l, mid, pos * 2 + 1, l, mid);
    T sum2 = Calc(mid, searching_r, pos * 2 + 2, mid, r);
    return sum1 + sum2;
  }

  void Build(const std::vector<T>& values, size_t pos, size_t l, size_t r) {
    assert (l < r);
    if (r - l == 1) {
      if (l < values.size()) {
        tree_[pos].sum = values[l];
      }
      return;
    }
    size_t mid = (l + r) >> 1;
    size_t left = pos * 2 + 1;
    size_t right = left + 1;
    Build(values, left, l, mid);
    Build(values, right, mid, r);
    tree_[pos].sum = tree_[left].sum + tree_[right].sum;
  }

  std::vector<Node> tree_;
  size_t size_;
  struct Node {
    bool has_operation{false};
    T modify{0};
    T sum{0};
  };
};