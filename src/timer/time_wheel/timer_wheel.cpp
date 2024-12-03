#include "timer_wheel.h"
#include <chrono>


timer_wheel::timer_wheel(const char* tw_name, uint32_t slot_nums, uint32_t slot_unit_time_ms) : tw_name_(tw_name), slot_nums_(slot_nums), slot_unit_time_ms_(slot_unit_time_ms), less_tw_ptr_(nullptr), greater_tw_ptr_(nullptr), slots_(slot_nums)
{
}


void timer_wheel::insert_timer_event(timer_event_ptr_t timer_event)
{
    uint64_t less_tw_cur_time = 0;

    if(less_tw_ptr_)
    {
        less_tw_cur_time = less_tw_ptr_->get_now_slottime_ms();
    }

    int64_t time_diff = timer_event->when_ms + less_tw_cur_time - get_cur_timestamp_ms();

    if(time_diff >= slot_unit_time_ms_)
    {
        uint32_t target_slot_index = (current_index_ + time_diff / slot_unit_time_ms_) % slot_nums_;
        slots_[target_slot_index].push_back(timer_event); 
        return;
    }

    if(less_tw_ptr_)
    {
        less_tw_ptr_->insert_timer_event(timer_event);
        return;
    }

    slots_[current_index_].push_back(timer_event);

    return;
}

std::list<timer_event_ptr_t> timer_wheel::get_timer_event(uint32_t index)
{
    return std::move(slots_[index]);
}

uint64_t timer_wheel::get_now_slottime_ms()
{
    uint64_t time = current_index_ * slot_unit_time_ms_;
    if(less_tw_ptr_)
    {
        time += less_tw_ptr_->get_now_slottime_ms();
    }
    return time;
}

void timer_wheel::tick_increase()
{
    current_index_++;

    if(current_index_ < slot_nums_)
    {
        return;
    }
    else
    {
        current_index_ = current_index_ % slot_nums_;

        if(greater_tw_ptr_)
        {
            greater_tw_ptr_->tick_increase();
            std::list<timer_event_ptr_t> event_list = std::move(greater_tw_ptr_->get_timer_event(greater_tw_ptr_->current_index_));

            for(auto event : event_list)
            {
                insert_timer_event(event);
            }
        }
        return;
    }
}

void timer_wheel::set_less_tw_ptr(timer_wheel_ptr_t tw_ptr)
{
    less_tw_ptr_ = tw_ptr;
}

void timer_wheel::set_greater_tw_ptr(timer_wheel_ptr_t tw_ptr)
{
    greater_tw_ptr_ = tw_ptr;
}

uint64_t get_cur_timestamp_ms()
{
    std::chrono::milliseconds timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return timestamp_ms.count();
}