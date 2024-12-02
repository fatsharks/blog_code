#ifndef __CXX_THREAD_POOL_H__
#define __CXX_THREAD_POOL_H__

#include <iostream>
#include <mutex>
#include <queue>
#include <functional>
#include <thread>
#include <condition_variable>
#include <future>
#include <random>
#include <pthread.h>
#include <chrono>

template <class T>
class safe_queue
{
private:
    /* data */
    std::queue<T> que;
    std::mutex mt;

public:
    safe_queue(/* args */) = default;
    ~safe_queue() = default;
    bool empty();
    int size();
    void enqueue(T &arg);
    bool dequeue();
    T front();
};

template <class T>
bool safe_queue<T>::empty()
{
    std::lock_guard<std::mutex> lock(mt);
    return que.empty();
}

template <class T>
int safe_queue<T>::size()
{
    std::lock_guard<std::mutex> lock(mt);
    return que.size();
}

template <class T>
void safe_queue<T>::enqueue(T &arg)
{
    std::lock_guard<std::mutex> lock(mt);
    que.push(arg);
    return;
}

template <class T>
bool safe_queue<T>::dequeue()
{
    std::lock_guard<std::mutex> lock(mt);
    if (que.empty())
    {
        return false;
    }
    else
    {
        // t = std::move(que.front());
        que.pop();
        return true;
    }
}

template <class T>
T safe_queue<T>::front()
{
    return que.front();
}

class thread_pool
{
private:
    /* data */
    bool shutdown_flag;
    safe_queue<std::function<void()>> safe_que;

    std::vector<std::thread> thread_list;
    std::mutex cond_mt;
    std::condition_variable cv;

    class thread_worker
    {
    private:
        /* data */
        int th_id;
        thread_pool *pool;

    public:
        thread_worker(thread_pool *pl, const int id) : pool(pl), th_id(id) {}
        ~thread_worker() = default;

        void operator()()
        {
            std::function<void()> func;
            bool dequeue_flag;
            while (!pool->shutdown_flag)
            {
                std::unique_lock<std::mutex> lock(pool->cond_mt);
                while (pool->safe_que.empty())
                {
                    if (!pool->shutdown_flag)
                    {
                        pool->cv.wait(lock);
                    }
                    else
                    {
                        break;
                    }
                }
                func = pool->safe_que.front();
                dequeue_flag = pool->safe_que.dequeue();
                if (dequeue_flag)
                {
                    func();
                }
            }
        }
    };

public:
    thread_pool(const int &thread_num) : thread_list(std::vector<std::thread>(thread_num)), shutdown_flag(false) {}
    ~thread_pool() = default;

    thread_pool(const thread_pool &pool) = delete;
    thread_pool &operator=(const thread_pool &pool) = delete;
    thread_pool(thread_pool &&pool) = delete;
    thread_pool &operator=(thread_pool &&pool) = delete;

    void startup();
    void shutdown();

    template <class F, class... Args>
    auto submit_task(F &&f, Args &&...args) -> std::future<decltype(f(args...))>;
};

void thread_pool::startup()
{
    for (int i = 0; i < thread_list.size(); i++)
    {
        thread_list[i] = std::thread(thread_worker(this, i));
    }
    std::cout << "pool start up" << std::endl;
    return;
}

void thread_pool::shutdown()
{
    shutdown_flag = true;
    cv.notify_all();
    for (int i = 0; i < thread_list.size(); i++)
    {
        if (thread_list[i].joinable())
        {
            thread_list[i].join();
        }
    }
    std::cout << "pool shutdown" << std::endl;
    return;
}

template <class F, class... Args>
auto thread_pool::submit_task(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
{
    std::function<decltype(f(args...))()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(task);

    std::function<void()> task_wrapper = [task_ptr]()
    {
        (*task_ptr)();
    };

    safe_que.enqueue(task_wrapper);
    cv.notify_one();

    return task_ptr->get_future();
}


std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);

auto rnd = std::bind(dist, mt);

// 设置线程睡眠时间

void simulate_hard_computation()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10 + rnd()));
}

// 添加两个数字的简单函数并打印结果

void multiply(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}

// 添加并输出结果

void multiply_output(int &out, const int a, const int b)
{
    simulate_hard_computation();
    out = a * b;
    std::cout << a << " * " << b << " = " << out << std::endl;
}

// 结果返回

int multiply_return(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
    return res;
}

int thread_pool_test()
{
    thread_pool pool(3);

    pool.startup();

    for (int i = 1; i < 3; ++i)
    {
        for (int j = 1; j < 10; ++j)
        {
            pool.submit_task(multiply, i, j);
        }
    }

    int ret = 0;
    auto resul = pool.submit_task(multiply_output, std::ref(ret), 3, 8);
    resul.get();
    std::cout << "Last operation result is equals to " << ret << std::endl;

    auto result = pool.submit_task(multiply_return, 3, 6);
    std::cout << "last times task result: " << result.get() << std::endl;
    pool.shutdown();
    return 0;
}

#endif