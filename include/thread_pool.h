#pragma once

#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool
{
public:
    ThreadPool(int threadPoolSize = std::thread::hardware_concurrency()) : isStopped(false)
    {
        for (int i = 0; i < threadPoolSize; ++i)
            threads.emplace_back(std::bind(&ThreadPool::worker, this));
    }

    ~ThreadPool()
    {
        if (!isStopped)
            stop();
    }

    template <typename Fn, typename... Args>
    void post(Fn &&fn, Args &&...args)
    {
        std::lock_guard<std::mutex> lock(mutex);
        workQueue.emplace(std::async(std::launch::deferred,
                                     std::forward<Fn>(fn),
                                     std::forward<Args>(args)...));
    }

    void stop()
    {
        isStopped = true;
        cv.notify_all();

        for (auto &t : threads)
            t.join();
    }

private:
    void worker()
    {
        while(!isStopped)
        {
            std::unique_lock<std::mutex> ul(mutex);
            cv.wait(ul, [this]()
            {
                return !workQueue.empty() || isStopped;
            });

            while(!workQueue.empty())
            {
                auto f = std::move(workQueue.front());
                workQueue.pop();
                ul.unlock();
                f.get();
                if(!ul.owns_lock())
                    ul.lock();  
            }
        }
    }

    std::vector<std::thread> threads;
    std::queue<std::future<void>> workQueue;
    std::atomic_bool isStopped;
    std::mutex mutex;
    std::condition_variable cv;
};