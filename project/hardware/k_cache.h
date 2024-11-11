#include "stdint.h"

/*
* 循环队列，单位为字节
* out == in的时候ring buf为空
* 往cache里面写数据时,直接往in处写数据，写完后in++
* 从cache里面读数据时,直接读out处的数据，读完后out++
*/
typedef struct k_cache_struct
{
	uint8_t *buf;
	int32_t vol;
	int32_t in;
	int32_t out;
	int32_t flag;
}k_cache_t;

int32_t k_cache_init(k_cache_t *cache, void *buf, int32_t size);

int32_t k_cache_read(k_cache_t *cache, void *buf, int32_t size);

int32_t k_cache_write(k_cache_t *cache, void *buf, int32_t size);

void k_cache_set_end(k_cache_t *cache);

void k_cache_set_err(k_cache_t *cache);

int32_t k_cache_flag_get(k_cache_t *cache);