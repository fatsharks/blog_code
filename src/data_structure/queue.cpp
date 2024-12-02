#ifndef __DATA_STRUCTURE_QUEUE_H__
#define __DATA_STRUCTURE_QUEUE_H__

#include <iostream>

#define QUEUE_DEFAULT_SIZE 32

template<class T> class queue_list
{
private:
    T *arr;
    int cnt;
    int arr_capacity;
public:
    queue_list(/* args */);
    ~queue_list();
    int push(const T&& arg);
    void pop();
    T &front();
    int empty();
    int size();
    template<class Arg>int emplace_back(const Arg&& arg...);
};

template<class T> queue_list<T>::queue_list()
{
    arr_capacity = QUEUE_DEFAULT_SIZE;
    arr = new T(arr_capacity);
    cnt = 0;
    if(arr == nullptr)
    {
        std::cout << "queue list malloc err" << std::endl;
    }
}

template<class T> queue_list<T>::~queue_list()
{
    if(arr != nullptr)
    {
        for(int i = 0; i < cnt; i++)
        {
            delete arr[i];
            arr[i] = nullptr;
        }
    }
}

template<class T> int queue_list<T>::empty()
{
    return cnt == 0;
}

template<class T> int queue_list<T>::size()
{
    return cnt;
}

template<class T> T &queue_list<T>::front()
{
    return arr[0];
}

template<class T> int queue_list<T>::push(const T&& arg)
{
    if(cnt < arr_capacity)
    {
        arr[cnt++] = arg;
        return 0;
    }
    else
    {
        //TODO：queue需要进行扩容
        return 0;
    }
}

template<class T> template<class Arg> int queue_list<T>::emplace_back(const Arg&& arg...)
{
    if(cnt < arr_capacity)
    {
        arr[cnt++] = new T(arg);
        return 0;
    }
    else
    {
        return 0;
    }
}

template<class T> void queue_list<T>::pop()
{
    if(cnt == 0)
    {
        return;
    }
    else
    {
        T ret = arr[0];
        for(uint8_t i = 1; i < cnt; i++)
        {
            arr[i - 1] = arr[i];
        }
        cnt--;
        return ;
    }
}



int queue_test(void)
{
    return 0;
}



#endif