#pragma once

#include <queue>
#include <mutex>
#include <utility>

namespace dreadpools {

template <typename T>
class TaskQueue {
public:
    [[nodiscard]] bool empty() {
        std::lock_guard lock(_mutex);
        return _queue.empty();
    }

    template <typename ... Args>
    void enqueue(Args&& ... args) {
        std::lock_guard lock(_mutex);
        _queue.emplace(std::forward<T>(args)...);
    }
    
    // caller is responsible for providing out param
    bool dequeue(T& out) {
        std::lock_guard lock(_mutex);
        if (_queue.empty()) {
            return false;
        }
        out = std::move(_queue.front());
        _queue.pop();
        return true;
    }

private:
    std::queue<T> _queue{};
    std::mutex _mutex{};
};

}
