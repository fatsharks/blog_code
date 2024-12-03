#ifndef __TIMER_WHEEL_H__
#define __TIMER_WHEEL_H__

#include <list>
#include <functional>
#include <vector>
#include <memory>
#include "stdint.h"

typedef std::shared_ptr<timer_wheel> timer_wheel_ptr_t;
typedef std::function<void()> timer_task_t;
uint64_t get_cur_timestamp_ms();

typedef struct
{
    timer_task_t task;
    uint64_t when_ms;
} timer_event_t;

typedef std::shared_ptr<timer_event_t> timer_event_ptr_t;

class timer_wheel
{
private:
    const char* tw_name_;
    uint32_t current_index_;
    uint32_t slot_nums_;
    uint32_t slot_unit_time_ms_;

    timer_wheel_ptr_t less_tw_ptr_;
    timer_wheel_ptr_t greater_tw_ptr_;

    std::vector<std::list<timer_event_ptr_t>> slots_;


public:
    timer_wheel(const char* tw_name, uint32_t slot_nums, uint32_t slot_unit_time_ms);
    ~timer_wheel();

    void insert_timer_event(timer_event_ptr_t timer_event);
    std::list<timer_event_ptr_t> get_timer_event(uint32_t index);
    uint64_t get_now_slottime_ms();

    void set_less_tw_ptr(timer_wheel_ptr_t tw_ptr);
    void set_greater_tw_ptr(timer_wheel_ptr_t tw_ptr);

    void tick_increase();
};



#endif
