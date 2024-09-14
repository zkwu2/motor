#include "robot.h"

const char *eve_name_buf[]=
{
    "STAT_ENTER",
	"STAT_EXIT",
	"STAT_BACK",
	"STAT_RESUME",

    "PWR_ON",
    "PWR_OFF",

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
// static os_semaphore_t sem_eve;
char eve_map_buf[EVE_TOTAL_NUM];

static void robot_eve_reset_all(void)
{	
    // robot_enter_critical();
	memset(eve_map_buf, 0, sizeof(eve_map_buf));
    // robot_exit_critical();
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
	// os_semaphore_create(&sem_eve, "eve", 0);
}

void robot_eve_post(int32_t eve)
{
    // robot_enter_critical();
	eve_map_buf[eve] = 1;
    // robot_exit_critical();
	// os_semaphore_put(&sem_eve);
}

static int32_t robot_eve_fetch(void)
{
	int eve = -1;
	for(int i = 0; i < EVE_TOTAL_NUM; i++)
	{
		if(eve_map_buf[i] == 1)
		{
			eve = i;
            // robot_enter_critical();
			eve_map_buf[i] = 0;
            // robot_exit_critical();
			robot_eve_get_name(eve);
			break;
		}
	}
	return eve;
}

int32_t robot_eve_wait_fetch(int32_t wait)
{
	// int rev = 0;
	if(wait <= 0)
	{
		return robot_eve_fetch();
	}
	// rev = os_semaphore_get(&sem_eve, wait);
	return robot_eve_fetch();
}

int32_t robot_eve_chk(int32_t eve)
{
    return eve_map_buf[eve];
}

void robot_eve_clr(int32_t eve)
{
    // robot_enter_critical();
    eve_map_buf[eve] = 0;
    // robot_exit_critical();
}