#ifndef __PLAT_OS_H__
#define __PLAT_OS_H__

#include "stdint.h"

int os_thread_sleep(int32_t ticks);

uint32_t os_ticks_get(void);

void *os_mem_alloc(uint32_t size);

void os_mem_free(void *ptr);

#endif