#include <mutex>
#include <condition_variable>
#include <chrono>
#include <pub_inc.h>
#include <queue>
#include <unistd.h>
#include <stdio.h>
using namespace std;

static int cnt = 0;
static mutex mt;
static condition_variable cv;
static bool notify_flag = false;
static bool thread_run_flag = false;
static queue<int> que;
static pthread_mutex_t pth_mt = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;  // 缓冲区不空

void *add_func(void *argv)
{
    for(uint8_t i = 0; i < 100; i++)
    {
        lock_guard<mutex> lock(mt);
        cnt ++;
    }
    return nullptr;
}

void *producer(void *argv)
{
    for(int i = 0; i < 10; i++)
    {
        usleep(900);
        pthread_mutex_lock(&pth_mt);
        que.push(i);
        cout << "producer i: " << i << endl;
        pthread_cond_signal(&not_empty);
        notify_flag = true;
        pthread_mutex_unlock(&pth_mt);
    }
    pthread_exit(NULL);
}

void *consumer(void *argv)
{
    while(thread_run_flag)
    {
        pthread_mutex_lock(&pth_mt);

        while(!notify_flag)
        {
            pthread_cond_wait(&not_empty, &pth_mt);
        }
        usleep(1000);
        while(!que.empty())
        {
            cout << "consumer i: " << que.front() << endl;
            que.pop();
        }
        notify_flag = false;
        pthread_mutex_unlock(&pth_mt);
    }
    pthread_exit(NULL);
}


int mutex_test(void)
{
    thread th[10];
    for(int i = 0; i < 10; i++)
    {
        th[i] = thread(add_func, nullptr);
    }

    for(uint8_t i = 0; i < 10; i++)
    {
        th[i].join();
    }

    std::cout << "cnt: " << cnt << std::endl;

    thread_run_flag = true;

    // auto producer = [&](){
    //     for(int i = 0; i < 10; i++)
    //     {
    //         this_thread::sleep_for(chrono::milliseconds(900));
    //         unique_lock<mutex> lock(mt);
    //         que.push(i);
    //         cout << "produce i: " << i  << endl;
    //         notify_flag = true;
    //         cv.notify_all();
    //     }
    // };

    // auto consumer = [&](){
    //     while(thread_run_flag)
    //     {
    //         unique_lock<mutex> lock(mt);
    //         // cv.wait(lock);
    //         cv.wait(lock, [&](){return notify_flag;});
    //         lock.unlock();

    //         this_thread::sleep_for(chrono::milliseconds(1000));

    //         lock.lock();
            
    //         while(!que.empty())
    //         {
    //             cout << "comsumer i: " << que.front() << endl;
    //             que.pop();
    //         }
    //         notify_flag = false;

    //     }
    // };

    // thread p_th(producer);

    // thread c_th[3];
    // for(int i = 0; i < 3; i++)
    // {
    //     c_th[i] = thread(consumer);
    // }
    // p_th.join();

    // thread_run_flag = false;
    // for(int i = 0; i < 3; i++)
    // {
    //     c_th[i].join();
    // }

    pthread_t prod, cons[3];

    int prod_ret = 0;
    int cons_ret[3] = {};

    prod_ret = pthread_create(&prod, NULL, &producer, NULL);
    for(int i = 0; i < 3; i++)
    {
        cons_ret[i] = pthread_create(&cons[i], NULL, &consumer, NULL);
    }

    // thread_run_flag = false;
    prod_ret = pthread_join(prod, NULL);

    for(int i = 0; i < 3; i++)
    {
        cons_ret[i] = pthread_join(cons[i], NULL);
    }

    return 0;
}
