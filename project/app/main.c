
#include "robot.h"
#include <signal.h>

static void sig_handler(int sig)
{
    logi("\n");
    switch(sig)
    {
    case SIGINT:
        robot_eve_post(EVE_UI_EVT1);
        break;
    case SIGQUIT:
        robot_eve_post(EVE_UI_EVT2);
        break;
    default:
        break;
    }
}

static void robot_set_before_run(void)
{
    signal(SIGINT,  sig_handler); //Ctr + 'C'
    signal(SIGQUIT, sig_handler); //Ctr + '\'
}

int32_t app_main(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter(); 
            break;
        case EVE_STAT_EXIT:
            robot_state_show_exit();
            break;
        case EVE_STAT_BACK:
            break;
        case EVE_UI_EVT1:
            break;
        case EVE_UI_EVT2:
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEEL;
}

void robot_main(void)
{
    logi("[robot_main]start\n");
    robot_init();
    robot_set_before_run();
    robot_run(app_main);
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