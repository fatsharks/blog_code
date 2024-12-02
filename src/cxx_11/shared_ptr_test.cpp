#include <pub_inc.h>
#include <memory>
#include <mutex>

using namespace std;

template<typename T>
class share_ptr
{
private:
    T *ptr;
    int *ref_cnt;
    mutex* mt;

    int release_ptr();
    int add_ref_cnt();

public:
    share_ptr(T *p = nullptr) : ptr(p), ref_cnt(new int(1)), mt(new mutex())
    {
        std::cout << "share ptr constructor" << std::endl;
    }
    ~share_ptr();
    share_ptr& operator=(const share_ptr &p);
    share_ptr(const share_ptr &p)
    {
        std::cout << "exec copy" << std::endl;
        ptr     = p.ptr;
        ref_cnt = p.ref_cnt;
        mt      = p.mt;

        add_ref_cnt();
    }
    int get_use_cnt();
    T*  get_origin_ptr();
}; 

template<typename T> share_ptr<T>::~share_ptr()
{
    std::cout << "share ptr deconstructor" << std::endl;
    release_ptr();
}

template<typename T> share_ptr<T>& share_ptr<T>::operator=(const share_ptr<T> &p)
{
    std::cout << "exec equal" << std::endl;
    if(p.ptr != ptr)
    {
        release_ptr(); // 释放旧资源
        ptr     = p.ptr;
        ref_cnt = p.ref_cnt;
        mt      = p.mt;

        add_ref_cnt();
        std::cout << "equal ref cnt: " << *ref_cnt << std::endl;
    }
    return *this;
}

template<typename T> share_ptr<T> make_share(T arg)
{
    share_ptr<T> ret(new T(arg));
    return ret;
}

template<typename T> int share_ptr<T>::release_ptr()
{
    bool release_flag = false;
    mt->lock();

    if(--(*ref_cnt) == 0)
    {
        delete ptr;
        delete ref_cnt;
        release_flag = true;
    }

    mt->unlock();
    if(release_flag)
    {
        delete mt;

        ptr     = nullptr;
        ref_cnt = nullptr;
        mt      = nullptr;

        std::cout << "release share ptr" << std::endl;
    }

    return 0;
}

template<typename T> int share_ptr<T>::add_ref_cnt()
{
    mt->lock();
    ++(*ref_cnt);
    mt->unlock();

    std::cout << "add ref cnt: " << *ref_cnt << std::endl;
    return 0;
}

template<typename T> T* share_ptr<T>::get_origin_ptr()
{
    return ptr;
}

template<typename T> int share_ptr<T>::get_use_cnt()
{
    return *ref_cnt;
}



int shared_ptr_test(void)
{
    int *num = new int(10);
    share_ptr<int> p1(num);

    share_ptr<int> p2(p1);
    share_ptr<int> p3 = p2;
    share_ptr<int> p4 = make_share<int>(10);

    std::cout << "test ref cnt: " << p1.get_use_cnt() << std::endl;
    std::cout << "p4 ref cnt: " << p4.get_use_cnt() << std::endl;

    return 0;
}