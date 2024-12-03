#include "timer.h"
#include "timer_wheel_schedular.h"
#include <chrono>

timer::timer(timer_task_t task, uint64_t when_ms) : task_(task), when_ms_(when_ms), time_interval_(0), timer_type_(timer_type_e::TIME_AT), timer_name_(nullptr)
{}

timer::timer(timer_task_t task, uint64_t when_ms, uint64_t time_interval) : task_(task), when_ms_(when_ms), time_interval_(time_interval), timer_type_(timer_type_e::TIME_AT_LOOP), timer_name_(nullptr)
{}

timer::timer(timer_task_t task, uint64_t cnt_down_ms) : task_(task), time_interval_(0), timer_type_(timer_type_e::TIME_AFTER), timer_name_(nullptr)
{
    when_ms_ = cnt_down_ms + get_cur_timestamp_ms();
}

timer::timer(timer_task_t task, uint64_t cnt_down_ms, uint64_t time_interval) : task_(task), time_interval_(time_interval), timer_type_(timer_type_e::TIME_AFTER_LOOP), timer_name_(nullptr)
{
    when_ms_ = cnt_down_ms + get_cur_timestamp_ms();
}

uint64_t timer::when_ms()
{
    return when_ms_; 
}

void timer::update_when_time_ms()
{
    when_ms_ += time_interval_;
}

void timer::shut_down()
{
    timer_sch_.stop_timer_wheel_schedular();
}

void timer::start_up()
{
    timer_sch_.start_timer_wheel_schedular();
}

void timer::cancel()
{
    timer_sch_.destroy_timer_wheel_schedular();
}