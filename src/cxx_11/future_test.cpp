#include <pub_inc.h>
#include <future>
#include <thread>

// lambda捕获只能捕获 lambda表达式所在作用域的 非静态局部 变量

// void promise_add(std::promise<int>&& p)
// {
//     p.set_value(7);
// }

int future_test(void)
{
    auto add = [](int left, int right){return left + right;};
    std::function<int(int)> add_func = std::bind(add, std::placeholders::_1, 2);


    /*******************    packaged task         ******************************/
    std::packaged_task<int()> add_task([&](){std::cout << "package task execute" << std::endl; return add_func(3);});
    // std::packaged_task<int()> add_task(std::move([&](){return 5;}));

    std::future<int> add_result = add_task.get_future();

    std::thread add_thread(std::move(add_task));
    add_thread.detach();

    add_result.wait();
    std::cout << "packaged task add result: " << add_result.get() << std::endl;


    /******************     async                        ********************/
    std::future<int> async_add_result = std::async([&](){std::cout << "async task execute" << std::endl; return add_func(4);});

    std::cout << "async task add result: " << async_add_result.get() << std::endl;

    /******************     promise              ******************************/
    std::promise<int> pr;
    auto promise_add = [](std::promise<int> pr, int left, int right){pr.set_value(left + right);};
    std::future<int> promise_add_result = pr.get_future();
    std::thread th(std::move(promise_add), std::move(pr), 2, 5);
    th.join();

    std::cout << "promise add result: " << promise_add_result.get() << std::endl;

    return 0;
}