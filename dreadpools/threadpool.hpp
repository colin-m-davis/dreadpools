#pragma once

#include "taskqueue.hpp"
#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <vector>

namespace dreadpools {

class ThreadPool {
public:
  explicit ThreadPool(
      // std::thread::hardware_concurrency can return 0
      const unsigned int count_threads =
          std::max(2u, std::thread::hardware_concurrency()) - 1u) noexcept
      : threads(count_threads){};

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  void start();
  void join();

  template <typename Callable, typename... Args>
  std::future<std::invoke_result_t<Callable, Args...>> submit(Callable &&f,
                                                              Args &&...args) {
    using return_type = std::invoke_result_t<Callable, Args...>;
    std::function<return_type()> func =
        std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
    auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(func);

    _tasks.enqueue([task_ptr]() { (*task_ptr)(); });
    _cv.notify_one();
    return task_ptr->get_future();
  }

  ~ThreadPool();

private:
  std::vector<std::jthread> threads;
  std::condition_variable _cv{};
  std::mutex _cv_m{};
  TaskQueue<std::function<void()>> _tasks{};
  std::stop_source _stop_source;
  friend class ThreadWorker;
};

class ThreadWorker {
public:
  explicit ThreadWorker(ThreadPool &p);
  void operator()() const;

private:
  ThreadPool &_pool;
  std::stop_token _stop_token;
};

} // namespace dreadpools
