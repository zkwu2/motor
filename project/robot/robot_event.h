#ifndef __ROBOT_EVENT_H__
#define __ROBOT_EVENT_H__

#include "stdint.h"

#define robot_state_show_enter()	logi("[Enter][%s]\n", (char *)__func__)
#define robot_state_show_exit()		logi("[Exit][%s]\n", (char *)__func__)

typedef enum
{
	EVE_SYS_START = 0,
	
    EVE_STAT_ENTER = EVE_SYS_START,
    EVE_STAT_EXIT,
	EVE_STAT_RESUME,

    EVE_PWR_ON,
    EVE_PWR_OFF,

	EVE_KEYD01,                    
	EVE_KEYU01,
	EVE_KEYS01,
	EVE_KEYL01,

	EVE_UI_EVT1,
    EVE_UI_EVT2,
    EVE_UI_EVT3,
    EVE_UI_EVT4,
	
	EVE_SYS_END,
}event_t;

void robot_eve_init(void);
char *robot_eve_get_name(int32_t eve);
void robot_eve_post(int32_t eve);
int32_t robot_eve_fetch(void);
int32_t robot_eve_chk(int32_t eve);
void robot_eve_clr(int32_t eve);

#endif