#include "robot.h"

void log_show(loglevel level, const char *format, ...) 
{
    if (level >= LOG_LEVEL_SET)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    } 
}

void robot_init(void)
{
    robot_eve_init();
}

void *robot_run(void *parm0, void *parm1)
{
	robot_state_init(parm0, parm1);
	robot_state_run();
    return NULL;
}