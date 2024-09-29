#include "robot.h"

robot_t robot_one;

static int32_t state_fun_start(robot_state_t *self, int32_t event)
{
	switch(event)
	{
		default:
			loge("[robot_state]default stat skip\n");
			break;
	}
	return ROBOT_STATE_REV_DEAL;
}

void robot_state_init(state_fun_t start_fun, state_fun_t global_fun)
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
	robot->global_fun = global_fun;
	robot_eve_post(EVE_STAT_ENTER);
}

int32_t robot_state_run(void)
{
	int32_t eve, rev;
	robot_t *robot = &robot_one;
	robot_state_t *cur;
	
	while(1)
	{
        cur = &robot->state_buf[robot->index];
		eve = robot_eve_fetch();
		if(eve < 0 || cur->fun == NULL)
		{
			continue;
		}

		logi("<<%s>>\n", robot_eve_get_name(eve));
		rev = cur->fun(cur, eve); //先在当前场景来处理事件
        if(rev == ROBOT_STATE_REV_SKIP) //如果事件没有被当前场景处理,看全局状态会不会处理
        {
            if(robot->global_fun != NULL)
			{
				rev = robot->global_fun(cur, eve);
			}
        }

		switch(rev)
		{
			case ROBOT_STATE_REV_DEAL:
				break;
			case ROBOT_STATE_REV_BACK:
				return rev;
			case ROBOT_STATE_REV_GOTO:
				break;
			case ROBOT_STATE_REV_SKIP:
				logi("<<Negative>>\n");
				break;
			default:
				logi("[state]run ERR rev:%d\n", rev);
		}
	}
}

//call场景退出后，会先执行上一个场景中EVE_STAT_RESUME中的操作，再执行上一个场景中调用robot_state_call()函数后的操作
int32_t _robot_state_call(state_fun_t fun, void *p0, void *p1, void *p2, void *p3, void *p4)
{
	int32_t rev;
	robot_t *robot = &robot_one;
	robot_state_t *cur, *state;
	
    cur = &robot->state_buf[robot->index];
	if(cur == robot->end)
	{
		robot->ret_val = (void *)-1;
		loge("[state err]current state is the last state!!!\n");
		return -1;
	}
    
	robot->index++;
	state = &robot->state_buf[robot->index];
    memset(state, 0, sizeof(robot_state_t));
	state->fun = fun;
	state->parm[0] = p0;
	state->parm[1] = p1;
	state->parm[2] = p2;
	state->parm[3] = p3;
	state->parm[4] = p4;
	robot_eve_post(EVE_STAT_ENTER);
	rev = robot_state_run();
    if(rev == ROBOT_STATE_REV_BACK)
    { 
        logi("<<%s>>\n", robot_eve_get_name(EVE_STAT_RESUME));
		cur->fun(cur, EVE_STAT_RESUME);
    }
	return rev;
} 

//调用robot_state_goto()函数后，当前场景中的其他代码就不执行了，并且依次执行历史场景中的EVE_STAT_EXIT
void _robot_state_goto(state_fun_t fun, void *p0, void *p1, void *p2, void *p3, void *p4)
{
	robot_t *robot = &robot_one;
	robot_state_t *state;
	for(int32_t i = robot->index; i >= 0; i--)
	{
		state = &robot->state_buf[i];
	    logi("<<%s>>\n", robot_eve_get_name(EVE_STAT_EXIT));
		state->fun(state, EVE_STAT_EXIT);
	}
	robot->index = 0;
	state = &robot->state_buf[0];
    memset(state, 0, sizeof(robot_state_t));
	state->fun = fun;
	state->parm[0] = p0;
	state->parm[1] = p1;
	state->parm[2] = p2;
	state->parm[3] = p3;
	state->parm[4] = p4;
	robot_eve_post(EVE_STAT_ENTER);
}

int32_t _robot_state_back(void *bak_val)
{
	robot_t *robot = &robot_one;
	robot_state_t *cur = &robot->state_buf[robot->index];
	if(cur == robot->first)
	{
        return ROBOT_STATE_REV_ERR;
	}
	robot->ret_val = bak_val;
	robot->index--;
	logi("<<%s>>\n", robot_eve_get_name(EVE_STAT_EXIT));
	cur->fun(cur, EVE_STAT_EXIT);

	return ROBOT_STATE_REV_BACK;
}