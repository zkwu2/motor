#include "robot.h"
#include "hw_timer.h"

typedef struct{
    int32_t id;
    int32_t flag;
}state_setting_t;

int32_t state_setting(robot_state_t *self, int32_t event)
{
    state_setting_t *state = self->parm[STAT_PARM_NUM - 1];
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter();
            state = os_mem_alloc(sizeof(state_setting_t));
            self->parm[STAT_PARM_NUM - 1] = state;
            state->id = 1001;
            state->flag = 1;
            break;
        case EVE_STAT_EXIT:
            hw_timer_stop(TIME1);
            os_mem_free(state);
            robot_state_show_exit();
            break;
        case EVE_UI_EVT1:
            logi("id = %d, flag = %d\n", state->id, state->flag);
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