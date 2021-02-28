#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <future>
#include <utility>
#include "queue.h"



class ThreadPool {
private:
    class WorkTask
    {
    private:
        ThreadPool* _pool;

    public:

        WorkTask(ThreadPool* pool)
        {
            _pool = pool;
        }

        void operator()()
        {
            std::function<void()> execFunc;
            while (_pool->_running)
            {
                std::unique_lock<std::mutex> lock(_pool->_mutex);
                if (_pool->_queue.empty())
                {
                    _pool->_cvar.wait(lock);
                }
                execFunc = _pool->_queue.dequeue();
                if (execFunc != NULL)
                {
                    execFunc();
                }
            }
        }
    };

    bool _running;
    std::vector<std::thread> _threads;
    SynchronizedQueue<std::function<void()>> _queue;

    std::condition_variable _cvar;
    std::mutex _mutex;


public:
    ThreadPool(size_t threads)
    {
        _threads = std::vector<std::thread>(threads);
        _running = true;
        for (int i = threads - 1; i >= 0; i--)
        {
            _threads[i] = std::thread(WorkTask(this));
        }
    }

    ~ThreadPool()
    {
        _running = false;
        _cvar.notify_all();
        for (int i = _threads.size() - 1; i >= 0; i--)
        {
            _threads[i].join();
        }
    }


    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
    {
        auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>> (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::function<void()> func = [task]() { (*task)(); }; 
        _queue.enqueue(func);
        _cvar.notify_one();

        return task->get_future();
    }
};

#endif


