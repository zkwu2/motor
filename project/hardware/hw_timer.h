#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__

#include "stdint.h"

#define TIMER_STAT_STOP         0
#define TIMER_STAT_RUNNING      1
#define TIMER_STAT_TIMEOUT      2

#define TIMER_ONCE           0
#define TIMER_INTER         (1 << 0)
#define TIMER_AUTO_START    (1 << 1)

#define TIME0   0
#define TIME1   1

typedef void(*hw_timer_cb)(int32_t data);

typedef struct hw_timer_struct
{
    int32_t stat;
    int32_t flag;
    uint32_t perio;
    uint32_t cnt;       //定时器计时，我们用自增的方式
    hw_timer_cb cb;
    int32_t data;
}hw_timer_t;

int32_t hw_timer_init(void);

int32_t hw_timer_start(int32_t id, uint32_t ms);

#endif