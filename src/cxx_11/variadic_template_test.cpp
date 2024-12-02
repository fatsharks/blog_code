#include <iostream>

template<class... Args> auto func(Args... args)
{
    std::cout << "paramter num: " << sizeof...(args) << std::endl;
    return 0;
}

template<class T> T sum(T left, T right)
{
    return left + right;
}

template<class T, class... Args> T sum(T left, Args... rights)
{
    return left + sum<T>(rights...);
}



int variadic_template_test()
{
    func(2, 3);
    std::cout << "sum result: " << sum(2, 3) << std::endl;
    std::cout << "sum result: " << sum(2, 3, 4) << std::endl;
    std::cout << "sum result: " << sum(2, 3, 4, 5) << std::endl;


    return 0;
}