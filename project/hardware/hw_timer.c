#include "robot.h"
#include "hw_timer.h"

#define TIMER_TOTAL_CNT  3

hw_timer_t *timer_buf[TIMER_TOTAL_CNT];

void hw_timer_step(int32_t ms)
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

//按键检测
static void mod_key_timer_cb(int32_t ms)
{
    
}

int32_t hw_timer_init(void)
{
    hw_timer_t *timer = NULL;

    timer = hw_timer_create(1000, mod_timer_cb, TIME0,  TIMER_ONCE); //TIME0,创建后默认关闭并且只跑一次
    timer_buf[TIME0] = timer;

    timer = hw_timer_create(1000, mod_timer_cb, TIME1,  TIMER_ONCE); //TIME1,创建后默认关闭并且只跑一次
    timer_buf[TIME1] = timer;

    timer = hw_timer_create(15, mod_key_timer_cb, 15,  TIMER_AUTO_START | TIMER_INTER); //KEY_TIM,创建后默认打开并且循环执行,15ms一次
    timer_buf[KEY_TIM] = timer;

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

int32_t hw_timer_stop(int32_t id)
{
    hw_timer_t *timer;
    if(id >= TIMER_TOTAL_CNT)
    {
        logi("[hw_timer]stop err id:%d\n", id);
        return -1;
    }

    timer = timer_buf[id];
    if(timer->stat != TIMER_STAT_TIMEOUT)
    {
        timer->stat = TIMER_STAT_STOP;
    }
    robot_eve_clr(id + EVE_TIMEOUT0);

    return 0;
}