#ifndef __TIMER_H__
#define __TIMER_H__

#include "stdint.h"
#include <functional>
#include "timer_wheel_schedular.h"

enum class timer_type_e
{
    TIME_AT = 0,
    TIME_AFTER,
    TIME_AT_LOOP,
    TIME_AFTER_LOOP,
};

class timer
{
private:
    uint64_t when_ms_;
    uint64_t time_interval_;
    timer_task_t task_;
    timer_type_e timer_type_;
    timer_wheel_schedular timer_sch_;

    const char *timer_name_;

public:
    timer(timer_task_t task, uint64_t when_ms);
    timer(timer_task_t task, uint64_t when_ms, uint64_t time_interval);
    timer(timer_task_t task, uint64_t cnt_down_ms);
    timer(timer_task_t task, uint64_t cnt_down_ms, uint64_t time_interval);
    ~timer();
    uint64_t when_ms();
    void update_when_time_ms();
    void start_up();
    void shut_down();
    void cancel();
};



#endif
