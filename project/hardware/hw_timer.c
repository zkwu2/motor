#include "robot.h"
#include "hw_timer.h"

#define TIMER_TOTAL_CNT  2

hw_timer_t *timer_buf[TIMER_TOTAL_CNT];

static void hw_timer_step(int32_t ms)
{
    int32_t stat, flag;
    hw_timer_t *timer;
    
    for(int32_t i = 0; i < TIMER_TOTAL_CNT; i++)
    {
        timer = timer_buf[i];
        stat = timer->stat;
        if(stat == TIMER_STAT_STOP || stat == TIMER_STAT_TIMEOUT)
        {
            continue;
        }
        timer->cnt += ms;
        if(timer->cnt < timer->perio)
        {
            continue;
        }
        
        flag = timer->flag;
        if(flag & TIMER_INTER) //是重复的定时器
        {
            timer->cnt = 0;
            timer->cb(timer->data);
        }
        else //没有设置重复
        {
            timer->stat = TIMER_STAT_TIMEOUT;
            timer->cnt = 0;
            timer->cb(timer->data);
        }
    }
}

static hw_timer_t *hw_timer_create(uint32_t ms, hw_timer_cb cb, int32_t data, int32_t flag)
{
    hw_timer_t *timer;
    timer = os_mem_alloc(sizeof(hw_timer_t));
    memset(timer, 0, sizeof(hw_timer_t));
    timer->perio = ms;
    timer->cb   = cb;
    timer->data = data;
    timer->flag = flag;
    timer->cnt  = 0;

    if(timer->flag & TIMER_AUTO_START)
    {
        timer->stat = TIMER_STAT_RUNNING;
    }
    else
    {
        timer->stat = TIMER_STAT_STOP;
    }

    return timer;
}

static void mod_timer_cb(int32_t id)
{
    robot_eve_post(id + EVE_TIMEOUT0);
}

int32_t hw_timer_init(void)
{
    hw_timer_t *timer = NULL;
    for(int32_t i = 0; i < TIMER_TOTAL_CNT; i++)
    {
        timer = hw_timer_create(5, mod_timer_cb, i,  TIMER_ONCE); //创建后默认关闭并且只跑一次
        timer_buf[i] = timer;
    }
    hw_timer_start(TIME0, 5);
    hw_timer_step(10); //注册step函数为硬件定时器回调函数

    return 0;
}

int32_t hw_timer_start(int32_t id, uint32_t ms)
{
    hw_timer_t *timer;
    if(id >= TIMER_TOTAL_CNT)
    {
        logi("[hw_timer]start err id:%d\n", id);
        return -1;
    }
    timer = timer_buf[id];
    timer->perio = ms;
    timer->cnt = 0;
    timer->stat = TIMER_STAT_RUNNING;

    return 0;
} 