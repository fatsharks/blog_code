#include "timer_wheel_schedular.h"

#define MS_WHEEL_SLOT_NUM 1000
#define SEC_SLOT_NUM 60
#define MIN_SLOT_NUM 60
#define HOUR_SLOT_NUM 24

#define MS_SLOT_TIME 1
#define SEC_SLOT_TIME 1000
#define MIN_SLOT_TIME (1000 * 60)
#define HOUR_SLOT_TIME (1000 * 60 * 60)



timer_wheel_schedular::timer_wheel_schedular()
{
    tw_ptr_ = nullptr;

    append_timer_wheel("ms_timer_wheel", MS_WHEEL_SLOT_NUM, MS_SLOT_TIME);
    append_timer_wheel("sec_timer_wheel", SEC_SLOT_NUM, SEC_SLOT_TIME);
    append_timer_wheel("min_timer_wheel", MIN_SLOT_NUM, MIN_SLOT_TIME);
    append_timer_wheel("hour_timer_wheel", HOUR_SLOT_NUM, HOUR_SLOT_TIME);
}

void timer_wheel_schedular::append_timer_wheel(const char *name, uint32_t slot_num, uint32_t slot_time_ms)
{
    auto timer_wheel_ptr = std::make_shared<timer_wheel>(timer_wheel(name, slot_num, slot_time_ms));
    if(tw_ptr_ == nullptr)
    {
        tw_ptr_ = timer_wheel_ptr;
    }
    else
    {
        tw_ptr_->set_greater_tw_ptr(timer_wheel_ptr);
        timer_wheel_ptr->set_less_tw_ptr(timer_wheel_ptr);
    }
    return ;
}