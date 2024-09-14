#include "robot.h"
#include <time.h>
#include "plat_os.h"

int os_thread_sleep(int32_t ms)
{
    usleep(ms * 1000);
    return 0;
}

uint32_t os_ticks_get(void)
{
    struct timespec ts;
    clock_gettime(1, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void *os_mem_alloc(uint32_t size)
{
    uint8_t *buf = malloc(size);
    if(buf)
    {
        memset(buf, 0, size);
    }
    return buf;
}

void os_mem_free(void *buf)
{
   free(buf);
}