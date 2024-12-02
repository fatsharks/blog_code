#include <iostream>

template<class T> class father
{
private:
    /* data */
public:
    father(/* args */);
    ~father();
    virtual father& say();
    void say_again();
};

template<class T> father<T>::father()
{
}

template<class T> father<T>::~father()
{
}

template<class T> father<T>& father<T>::say()
{
    std::cout << "father say" << std::endl;
    return *this;
}

template<class T> void father<T>::say_again()
{
    std::cout << "father say again" << std::endl;
}

template<class T> class son : public father<T>
{
private:
    /* data */
public:
    son(/* args */);
    ~son();
    son& say();
    void say_again();
};

template<class T> son<T>::son()
{
}

template<class T> son<T>::~son()
{
}

template<class T> son<T>& son<T>::say()
{
    std::cout << "son say" << std::endl;
    return *this;
}

template<class T> void son<T>::say_again()
{
    std::cout << "son say again" << std::endl;
}

template<class T> class empty_father
{
private:
    /* data */
public:
    empty_father
(/* args */);
    ~empty_father
();
};

template<class T> class empty_son : public empty_father<T>
{
private:
    /* data */
public:
    empty_son(/* args */);
    ~empty_son();
};



int virtual_class_test(void)
{
    father<int> fa;
    fa.say();
    fa.say_again();
    son<int> sn;
    sn.say();
    sn.say_again();

    father<int> *p_fa = new son<int>;
    p_fa->say();

    std::cout << "size of empty father" << sizeof(empty_father<int>) << std::endl;
    std::cout << "size of empty son" << sizeof(empty_son<int>) << std::endl;
    return 0;
}