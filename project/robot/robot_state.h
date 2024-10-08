#ifndef __ROBOT_STATE_H__
#define __ROBOT_STATE_H__

#include "robot.h"

typedef enum
{
	ROBOT_STATE_REV_DEAL = 0,   //收到的事件在这个场景里面处理掉了
	ROBOT_STATE_REV_SKIP,       //收到的事件在这个场景里面被忽略掉
	ROBOT_STATE_REV_GOTO,       //状态发生跳转
	ROBOT_STATE_REV_BACK,       //状态返回
	ROBOT_STATE_REV_ERR,        //状态跳转或者返回时出错，目前只有在没有上级状态的情况下调用状态返回时会出现
}robot_state_rev_t;

typedef struct robot_state robot_state_t;
typedef int32_t (*state_fun_t)(robot_state_t *self, int32_t eve);

#define PARAM0          0
#define PARAM1          1
#define PARAM2          2
#define PARAM3          3
#define PARAM4          4
#define STAT_PARM_NUM	5

#define STAT_NAME_SIZE  32
#define STATE_NUM		10

typedef struct robot_state
{
    int8_t name[STAT_NAME_SIZE];
	void* parm[STAT_PARM_NUM];
	state_fun_t fun;
}robot_state_t;

typedef struct robot_global_struct
{
    state_fun_t fun;    //global的处理函数
    robot_state_t *self; //设置global的时候可以设置一个状态机
}robot_global_t;

typedef struct robot_struct
{
	int32_t index;
	void* ret_val;
	robot_state_t *first;
	robot_state_t *end;
	robot_state_t state_buf[STATE_NUM];
    state_fun_t global_fun;
}robot_t;

extern robot_t robot_one;

#define robot_state_retval()   (robot_one.ret_val)

void robot_state_init(state_fun_t start_fun, state_fun_t global_fun);

int32_t robot_state_run(void);

int32_t _robot_state_call(state_fun_t fun, void *p0, void *p1, void *p2, void *p3, void *p4);

void _robot_state_goto(state_fun_t fun, void *p0, void *p1, void *p2, void *p3, void *p4);

int32_t _robot_state_back(void *bak_val);

#define robot_state_call(fun, p0, p1, p2, p3, p4)       \
do{                                                     \
	int32_t _rev; 									    \
	logi("[CALL][%s]\n", #fun); 						\
	_rev = _robot_state_call((fun), (void *)(p0),       \
                      (void *)(p1), (void *)(p2),       \
                      (void *)(p3), (void *)(p4));      \
    if(_rev != ROBOT_STATE_REV_BACK)                    \
	{ 												    \
		return _rev; 						            \
	} 													\
	else if(_rev == -1) 							    \
	{ 												    \
		logi("[CALL][%s]failed!\n", #fun); 	  		    \
	} 												    \
}while(0)

#define robot_state_call0(fun)   robot_state_call(fun, NULL, NULL, NULL, NULL, NULL)
#define robot_state_call1(fun, p0)   robot_state_call(fun, p0, NULL, NULL, NULL, NULL)
#define robot_state_call2(fun, p0, p1)   robot_state_call(fun, p0, p1, NULL, NULL, NULL)
#define robot_state_call3(fun, p0, p1, p2)  robot_state_call(fun, p0, p1, p2, NULL, NULL)
#define robot_state_call4(fun, p0, p1, p2, p3)  robot_state_call(fun, p0, p1, p2, p3, NULL)

#define robot_state_goto(fun, p0, p1, p2, p3, p4)       \
do{                                                     \
	logi("[GOTO][%s]\n", #fun);                         \
	_robot_state_goto((fun), (void *)(p0),              \
				(void *)(p1), (void *)(p2),             \
				(void *)(p3), (void *)(p4));            \
	return ROBOT_STATE_REV_GOTO; 						\
}while(0)                                     

#define robot_state_goto0(fun)   robot_state_goto(fun, NULL, NULL, NULL, NULL, NULL)
#define robot_state_goto1(fun, p0)   robot_state_goto(fun, p0, NULL, NULL, NULL, NULL)
#define robot_state_goto2(fun, p0, p1)   robot_state_goto(fun, p0, p1, NULL, NULL, NULL)
#define robot_state_goto3(fun, p0, p1, p2)  robot_state_goto(fun, p0, p1, p2, NULL, NULL)
#define robot_state_goto4(fun, p0, p1, p2, p3)  robot_state_goto(fun, p0, p1, p2, p3, NULL)

#define robot_state_back(val)                           \
do{ 		                                            \
	int32_t _rev; 						                \
	_rev = _robot_state_back((void *)val);              \
	if(_rev == ROBOT_STATE_REV_BACK) 	                \
 	{ 									                \
		logi("[BACK]\n"); 			                    \
	} 									                \
	return _rev; 	                                    \
}while(0)

#endif