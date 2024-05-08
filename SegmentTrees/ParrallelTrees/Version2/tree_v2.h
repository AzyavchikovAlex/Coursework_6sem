#pragma once

#pragma once

#include <cassert>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#include "Policies/abstract_mass_policy.h"
#include "SegmentTrees/abstract_tree.h"
#include "SegmentTrees/ClassicTree/classic_segment_tree.h"
#include "Utils/buffered_channel.h"

template<typename T, typename M, typename P, size_t ThreadsCount = 8>
class ParallelSegmentTree_V2 : public AbstractTree<T, M, P> {
 private:
  struct Query;
  struct Response;
 public:
  template<class Iter>
  explicit ParallelSegmentTree_V2(Iter begin, Iter end, P policy)
      : AbstractTree<T, M, P>(std::move(policy)) {
    if (std::distance(begin, end) < ThreadsCount) {
      throw std::runtime_error("too small array");
    }
    size_t chunk_size = std::distance(begin, end) / ThreadsCount;
    for (size_t index = 0; index < ThreadsCount; ++index) {
      size_t l_border = index * chunk_size;
      size_t r_border =
          index + 1 == ThreadsCount ? std::distance(begin, end) : (index + 1)
              * chunk_size;
      auto tree = std::make_shared<Tree<T, M, P>>(std::next(begin,
                                                                      l_border),
                                                            std::next(begin,
                                                                      r_border),
                                                            this->policy_);
      channels_[index] = std::make_shared<BufferedChannel<Query>>(kBufferSize);
      threads_pool_.emplace_back(&ParallelSegmentTree_V2::RunThread, this, tree, channels_[index], l_border, r_border);
    }
  }

  virtual  ~ParallelSegmentTree_V2() {
    for (size_t index = 0; index < ThreadsCount; ++index) {
      channels_[index]->Close();
    }
    for (size_t index = 0; index < ThreadsCount; ++index) {
      threads_pool_[index].join();
    }
    threads_pool_.clear();
  }

  void ModifyTree(size_t l, size_t r, M modifier) override {
    Query q = {modifier, l, r};
    for (size_t index = 0; index < ThreadsCount; ++index) {
      channels_[index]->Send(q);
    }
  }

  std::future<T> GetFutureTreeState(size_t l, size_t r) override {
    Query q = {std::nullopt, l, r, Response{std::make_shared<std::promise<T>>(),
                                            std::make_shared<std::atomic<T>>(this->GetNullState()),
                                            std::make_shared<std::atomic<int>>(0)}};
    for (size_t index = 0; index < ThreadsCount; ++index) {
      channels_[index]->Send(q);
    }
    return q.response.result->get_future();
  }

  T GetTreeState(size_t l, size_t r) override {
    return GetFutureTreeState(l, r).get();
  }

 private:
  struct Query {
    std::optional<M> modifier{std::nullopt};
    size_t l{0}, r{0};
    Response response{};
  };
  struct Response {
    std::shared_ptr<std::promise<T>> result{nullptr};
    std::shared_ptr<std::atomic<T>> state{nullptr};
    std::shared_ptr<std::atomic<int>> counter{nullptr};
  };
  std::vector<std::thread> threads_pool_;
  static constexpr size_t kBufferSize = 1024;
  std::array<std::shared_ptr<BufferedChannel<Query>>, ThreadsCount> channels_;

  void FinishTask(Response& response) {
    auto value = response.counter->fetch_add(1) + 1;
    if (value == ThreadsCount) {
      response.result->set_value(response.state->load());
    }
  }

  void FinishTask(Response& response, T sub_state) {
    for (auto old_state = response.state->load();;) {
      auto new_state = this->GetState(old_state, sub_state);
      if (response.state->compare_exchange_strong(old_state, new_state)) {
        break;
      }
    }
    FinishTask(response);
  }

  void RunThread(std::shared_ptr<Tree<T, M, P>> tree,
                 std::shared_ptr<BufferedChannel<Query>> channel,
                 size_t l_border,
                 size_t r_border) {
    for (;;) {
      auto query = channel->Recv();
      if (!query.has_value()) {
        break;
      }
      auto l = std::max(query->l, l_border);
      auto r = std::min(query->r, r_border);
      if (l >= r) {
        if (!query->modifier.has_value()) {
          FinishTask(query->response);
        }
        continue;
      }
      l -= l_border;
      r -= l_border;
      if (query->modifier.has_value()) {
        tree->ModifyTree(l, r, query->modifier.value());
        continue;
      }

      auto sub_state = tree->GetTreeState(l, r);
      FinishTask(query->response, sub_state);
    }
  }

};