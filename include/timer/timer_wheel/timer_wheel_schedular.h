#ifndef __TIMER_WHEEL_SCHEDULAR_H__
#define __TIMER_WHEEL_SCHEDULAR_H__

#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include "stdint.h"
#include "timer_wheel.h"

class timer_wheel_schedular
{
private:
    timer_wheel_ptr_t tw_ptr_;
    bool stop_;
public:
    timer_wheel_schedular();
    ~timer_wheel_schedular();

    void append_timer_wheel(const char *tw_name, uint32_t slots_num, uint32_t slot_unittime_ms);
    void stop_timer_wheel_schedular();
    void start_timer_wheel_schedular();
    void destroy_timer_wheel_schedular();
};


#endif