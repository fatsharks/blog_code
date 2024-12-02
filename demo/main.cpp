#include <iostream>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <future>
#include <functional>
#include <pub_inc.h>
#include <vector>

int main(int argc, char** argv)
{
    // future_test();
    // thread_test();
    // mutex_test();
    // singleton_test();
    // shared_ptr_test();
    // virtual_class_test();

    thread_pool_test();

    // variadic_template_test();

    // ipc_test();
    // v_slam_entr();

    std::cout << "main test end" << std::endl;
    return 0;
}