#include "robot.h"
#include <time.h>
#include "plat_os.h"

int os_thread_sleep(int32_t ms)
{
    usleep(ms * 1000);
    return 0;
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