#include "robot.h"

int32_t state_setting(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter();
            break;
        case EVE_STAT_EXIT:
            robot_state_show_exit();
            break;
        case EVE_UI_EVT1:
            // robot_state_back((void *)10);
            robot_state_call0(state_setting);
            break;
        case EVE_UI_EVT2:
            robot_state_goto0(state_setting);
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEAL;
}