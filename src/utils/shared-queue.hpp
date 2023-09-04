#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>

template<typename T>
struct SharedQueue
{
    auto empty() const -> bool{
        std::lock_guard lock(mutex);
        return data.empty();
    }

    auto size() const -> size_t {
        std::lock_guard lock(mutex);
        return data.size();
    }

    auto push(const T & element) -> void{
        std::lock_guard lock(mutex);
        data.push_back(element);
        data_condition.notify_one();
    }

    auto wait_and_pop() -> T {
        std::unique_lock lock(mutex);

        while (data.empty()) {
            data_condition.wait(lock);
        }

        T result = std::move(data.front());
        data.pop_front();
        return result;
    }

private:
    std::deque<T> data;
    mutable std::mutex mutex;
    std::condition_variable data_condition;
};
