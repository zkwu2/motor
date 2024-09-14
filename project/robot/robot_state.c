#include "robot.h"

#define DEFAULT_EVE_TIMEOUT  5

robot_t robot_one;

static int32_t state_fun_start(robot_state_t *self, int32_t event)
{
	switch(event)
	{
		default:
			loge("[robot_state]default stat skip\n");
			break;
	}
	return ROBOT_STATE_REV_DEEL;
}

void robot_state_init(state_fun_t start_fun)
{
	robot_state_t *cur;
	robot_t *robot = &robot_one;
	memset(robot, 0, sizeof(robot_t));
	cur = &robot->state_buf[0];
	if(start_fun == NULL)
	{
		cur->fun = state_fun_start;
	}
	else
	{
		cur->fun = start_fun;
	}
	robot->first	 = &robot->state_buf[0];
	robot->end		 = &robot->state_buf[STATE_NUM - 1];
	robot->index 	 = 0;
    robot->eve_timeout = DEFAULT_EVE_TIMEOUT;
	robot_eve_post(EVE_STAT_ENTER);
}

int32_t robot_state_global_deel(int32_t eve, robot_global_t *global, robot_state_t *cur)
{
    int32_t i, rev;
    for(i = 0; i < ROBOT_PRIO_NUM; i++, global++)
    {
        if(global->fun == NULL)
            continue;
        rev = global->fun(global->self == NULL ? cur : global->self, eve);
        if(rev != ROBOT_STATE_REV_SKIP)
        {
            return rev;
        }
    }
    return ROBOT_STATE_REV_SKIP;
}

int32_t robot_state_run(void)
{
	int32_t eve, rev;
	robot_t *robot = &robot_one;
	robot_state_t *cur;
	
	while(1)
	{
        cur = &robot->state_buf[robot->index];
		eve = robot_eve_wait_fetch(robot->eve_timeout);
		if(eve < 0 || cur->fun == NULL)
		{
			continue;
		}

		logi("<<%s>>\n", robot_eve_get_name(eve));
		rev = cur->fun(cur, eve); //先在当前场景来处理事件
        if(rev == ROBOT_STATE_REV_SKIP) //如果事件没有被当前场景处理,看全局状态会不会处理
        {
            rev = robot_state_global_deel(eve, robot->global, cur);
        }

		switch(rev)
		{
			case ROBOT_STATE_REV_DEEL:
				break;
			case ROBOT_STATE_REV_SKIP:
				logi("<<Negative>>\n");
				break;
			default:
				logi("[state]run ERR rev:%d\n", rev);
		}
	}
}