#include <iostream>

class singleton
{
private:
    singleton()
    {
        std::cout << "singleton constructor called" << std::endl;
    }
    ~singleton() = default;
    
public:
    static singleton& get_instance()
    {
        static singleton instance;
        return instance;
    }
    singleton(const singleton& single) = delete;
    singleton& operator=(const singleton& single) = delete;
};



int singleton_test(void)
{
    singleton& single1 = singleton::get_instance();
    singleton& single2 = singleton::get_instance();

    std::cout << "singleton test" << std::endl;

    return 0;
}