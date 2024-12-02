#include <pub_inc.h>
#include <pthread.h>

typedef struct
{
    int left;
    int right;
} num_struct_t;


void* times_num(void *argv)
{
    num_struct_t *num =  static_cast<num_struct_t*>(argv);

    std::cout << "times pthread result: " << num -> left * num -> right << std::endl;

    return NULL;

}

int thread_test(void)
{
    auto times = [](int left, int right){return left * right;};
    std::function<int(int)> times_func = std::bind(times, std::placeholders::_1, 2);
    
    std::thread times_func_thread(std::move([&](int num){std::cout << "times func thread result: " << times_func(num) << std::endl;}), 4);
    times_func_thread.join();

    pthread_attr_t times_thread_attr = {};
    pthread_t times_thread = {};

    num_struct_t num = {3, 3};
    int ret = pthread_create(&times_thread, &times_thread_attr, *times_num, &num);


    ret = pthread_join(times_thread, NULL);

    return ret;
}