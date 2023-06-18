#include "threadpool.hpp"

namespace dreadpools {

void ThreadPool::start() {
    for (auto& t : threads) {
        t = std::jthread(ThreadWorker(*this));
    }
}

void ThreadPool::join() {
    // adds a dummy task to end of queue
    auto fut = submit([]{});
    fut.get();
    // all tasks that were in the queue when join() was called have been picked up by a thread
    {
        std::unique_lock lk(_cv_m);
        _stop_source.request_stop();
        _cv.notify_all();
    }
    // wait for threads to complete their tasks as necessary
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock lk(_cv_m);
    _stop_source.request_stop();
    _cv.notify_all();
}

ThreadWorker::ThreadWorker(ThreadPool& p) :
    _pool(p),
    _stop_token(p._stop_source.get_token())
{}

void ThreadWorker::operator()() {
    while (!_stop_token.stop_requested()) {
        std::function<void()> task;
        bool has_task = false;
        {
            std::unique_lock lk(_pool._cv_m);
            while (!_stop_token.stop_requested() && _pool._tasks.empty()) {
                // unlock mutex and add itself to list of waiters
                _pool._cv.wait(lk);
            }
            has_task = _pool._tasks.dequeue(task);
        }
        if (has_task) {
            task();  // perform task
        }
    }
}

}
