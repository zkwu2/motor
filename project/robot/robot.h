#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "stdint.h"
#include "robot_event.h"
#include "robot_state.h"
#include "plat_os.h"

#define robot_enter_critical()	    1
#define robot_exit_critical()       0

typedef enum {
    ROBOT_LOG_LEVEL_DEBUG = 0,
    ROBOT_LOG_LEVEL_INFO,
    ROBOT_LOG_LEVEL_WARNING,
    ROBOT_LOG_LEVEL_ERROR,
}loglevel;

void log_show(loglevel level, const char *format, ...);

#define LOG_LEVEL_SET  ROBOT_LOG_LEVEL_INFO
#define logd(...)   log_show(ROBOT_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define logi(...)   log_show(ROBOT_LOG_LEVEL_INFO, __VA_ARGS__)
#define logw(...)   log_show(ROBOT_LOG_LEVEL_WARNING, __VA_ARGS__)
#define loge(...)   log_show(ROBOT_LOG_LEVEL_ERROR, __VA_ARGS__)

void robot_init(void);
void *robot_run(void *parm);

#endif
