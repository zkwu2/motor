#include "robot.h"
#include "hw_timer.h"

int32_t state_setting(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter();
            break;
        case EVE_STAT_EXIT:
            hw_timer_stop(TIME1);
            robot_state_show_exit();
            break;
        case EVE_UI_EVT1:
            robot_state_back((void *)10);
            break;
        case EVE_UI_EVT2:
            hw_timer_start(TIME1, 500);
            break;
        case EVE_TIMEOUT0:
            break;
        case EVE_TIMEOUT1:
            robot_eve_post(EVE_PWR_OFF);
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEAL;
}