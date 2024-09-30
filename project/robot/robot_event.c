#include "robot.h"

const char *eve_name_buf[]=
{
    "STAT_ENTER",
	"STAT_EXIT",
	"STAT_RESUME",

    "PWR_ON",
    "PWR_OFF",

	"TIMEOUT0",
	"TIMEOUT1",

	"KEYD01",
	"KEYU01",
	"KEYS01",
	"KEYL01",

	"UI_EVT1",
	"UI_EVT2",
	"UI_EVT3",
	"UI_EVT4",

    "SYS_END",
};

#define EVE_TOTAL_NUM (EVE_SYS_END - EVE_SYS_START)
static char eve_map_buf[EVE_TOTAL_NUM];

static void robot_eve_reset_all(void)
{	
	memset(eve_map_buf, 0, sizeof(eve_map_buf));
}

char *robot_eve_get_name(int32_t eve)
{
	static char eve_name[32];
    sprintf(eve_name, "%s", eve_name_buf[eve]);
    return eve_name;
}

void robot_eve_init(void)
{
	robot_eve_reset_all();
}

void robot_eve_post(int32_t eve)
{
	eve_map_buf[eve] = 1;
}

int32_t robot_eve_fetch(void)
{
	int eve = -1;
	for(int i = 0; i < EVE_TOTAL_NUM; i++)
	{
		if(eve_map_buf[i] == 1)
		{
			eve = i;
			eve_map_buf[i] = 0;
			robot_eve_get_name(eve);
			break;
		}
	}
	return eve;
}

int32_t robot_eve_chk(int32_t eve)
{
    return eve_map_buf[eve];
}

void robot_eve_clr(int32_t eve)
{
    eve_map_buf[eve] = 0;
}