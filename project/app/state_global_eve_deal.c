#include "robot.h"

int32_t state_global_eve_deal(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_PWR_ON:
            logi("[global]SYSTEM POWER ON!!!\n");
            break;
        case EVE_PWR_OFF:
            logi("[global]SYSTEM POWER OFF!!!\n");
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEAL;
}