#include "robot.h"
#include "hw_timer.h"
#include <signal.h>
#include <sys/time.h>

static void sig_handler(int sig)
{
    switch(sig)
    {
    case SIGINT:
        robot_eve_post(EVE_UI_EVT1);
        break;
    case SIGQUIT:
        robot_eve_post(EVE_UI_EVT2);
        break;
    case SIGALRM:
        hw_timer_step(5);
        break;
    default:
        break;
    }
}

static void robot_set_before_run(void)
{
    signal(SIGINT,  sig_handler); //Ctr + 'C'
    signal(SIGQUIT, sig_handler); //Ctr + '\'
    signal(SIGALRM, sig_handler); //alarm定时器处理函数

    struct itimerval timer;
    timer.it_value.tv_sec = 0; //秒部分
    timer.it_value.tv_usec = 5000; //微秒部分,第一次5ms后触发定时
    timer.it_interval.tv_sec = 0; //秒部分
    timer.it_interval.tv_usec = 5000; //微秒部分,每5ms触发一次定时
    setitimer(ITIMER_REAL, &timer, NULL); // 设置定时器
}

extern int32_t state_setting(robot_state_t *self, int32_t event);
extern int32_t state_global_eve_deal(robot_state_t *self, int32_t event);
int32_t app_main(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter(); 
            break;
        case EVE_STAT_EXIT:
            hw_timer_stop(TIME0);
            robot_state_show_exit();
            break;
        case EVE_STAT_RESUME:
            {
                void *ret_void = robot_state_retval();
                long ret_val = (long)ret_void;
                logi("[app_main]resume, get back data: %d\n", (int32_t)ret_val);
            }
            break;
        case EVE_UI_EVT1:
            robot_state_call0(state_setting);
            break;
        case EVE_UI_EVT2:
            hw_timer_start(TIME0, 500);
            break;
        case EVE_TIMEOUT0:
            robot_eve_post(EVE_PWR_ON);
            break;
        case EVE_TIMEOUT1:
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEAL;
}

void robot_main(void)
{
    logi("[robot_main]start\n");
    robot_init();
    hw_timer_init();
    robot_set_before_run();
    robot_run(app_main, state_global_eve_deal);
    while(1)
    {
		os_thread_sleep(40);
    }
}

int32_t main(void)
{
    robot_main(); //会一直停在这个函数里面
    return 0;
}