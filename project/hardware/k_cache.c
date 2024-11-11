#include "k_cache.h"
#include "robot.h"

/*
* 循环队列状态
*/
enum k_cache_flag
{
	CACHE_INIT	= 0,
	CACHE_END,
	CACHE_ERR,
};

//返回当前cache的剩余数据量大小
static int32_t k_cache_size_data(k_cache_t *cache)
{
	int32_t tmp = cache->in - cache->out;
	if(tmp < 0)
	{
		return tmp + cache->vol;
	}
	return tmp;
}

//返回当前cache剩余空间大小。为了防止buf被写满，返回的empty大小比真实的empty少4字节
static int32_t k_cache_size_empty(k_cache_t *cache)
{
	int32_t tmp = cache->out - cache->in;
	if(tmp <= 0)
	{
		tmp += cache->vol;
	}
	
	tmp = tmp > 4 ? tmp - 4 : 0;
	return tmp;
}

int32_t k_cache_init(k_cache_t *cache, void *buf, int32_t size)
{
	cache->buf 	= buf;
	cache->vol 	= size;
	cache->in  	= 0;
	cache->out  = 0;
	cache->flag = CACHE_INIT;
	
	return 0;
}

/*
* 读取cache里面size字节大小的数据到buf中，如果数据不够的话会一直阻塞在这里
*/
int32_t k_cache_read(k_cache_t *cache, void *buf, int32_t size)
{
    int32_t i, rev;
	uint8_t *dst;
	while(1)
	{
		rev = k_cache_size_data(cache);
		if(rev >= size)
		{
			rev = size; //数据量够，直接返回所需的size字节
			break;
		}
		else if(cache->flag == CACHE_END)
		{
			size = rev; //数据量不够且cache end，返回cache中剩余的数据
			break;
		}
		else if(cache->flag == CACHE_ERR)
		{
			logw("[cache]cache read err\n");
			return -1; //数据量不够且cache error，直接返回错误
		}
		os_thread_sleep(10);
	}
	
	i = cache->out;
	dst = (uint8_t *)buf;
	while(rev > 0)
	{
		*dst++ = cache->buf[i++]; //读取cache里面的数据到dst中
		if(i >= cache->vol){
			i = 0;
		}
		rev--;
	}
	
	cache->out = i;
	return size;
}

/*
* 将buf中的size字节大小数据写入到cache当中
*/
int32_t k_cache_write(k_cache_t *cache, void *buf, int32_t size)
{
	int32_t i, rev;
	uint8_t *src;
	
#if 0 //如果空间不够的话会一直阻塞在这里
	while(1)
	{
		rev = k_cache_size_empty(cache);
		if(rev >= size)
		{
			rev = size; //剩余的空间够，直接返回所需的size字节
			break;
		}
		else if(cache->flag == CACHE_END)
		{
			size = rev; //剩余的空间不够且cache end，返回cache中剩余的空间
			break;
		}
		else if(cache->flag == CACHE_ERR)
		{
			logw("[cache]cache write err\n");
			return -1; //剩余的空间不够且cache error，直接返回错误
		}
		os_thread_sleep(10);
	}
#else //如果空间不够的话直接返回0
	rev = k_cache_size_empty(cache);
	if(rev >= size)
	{
		rev = size; 
	}
	else if(cache->flag == CACHE_END)
	{
		return 0;
	}
	else if(cache->flag == CACHE_ERR)
	{
		logw("[cache]cache write err\n");
		return -1;
	}
	else
	{
		return 0;
	}
#endif
	
	i = cache->in;
	src = (uint8_t *)buf;
	while(rev > 0)
	{
		cache->buf[i++] = *src++; //将src数据写入到cache当中
		if(i >= cache->vol){
			i = 0;
		}
		rev--;
	}
	
	cache->in = i;
	return size;
}

void k_cache_set_end(k_cache_t *cache)
{
	cache->flag = CACHE_END;
}

void k_cache_set_err(k_cache_t *cache)
{
	cache->flag = CACHE_ERR;
}

int32_t k_cache_flag_get(k_cache_t *cache)
{
    return cache->flag;
}


#if 0
k_cache_t cache = {0};
void main(void)
{
	uint8_t *buf = (uint8_t *)malloc(1024);
	if(buf == NULL)
		return;
	k_cache_init(&cache, buf, 1024); //开辟一个大小为1k的ring buffer
	
	uint8_t *write_buf = (uint8_t *)malloc(64); //每次往cache里面写64字节数据
	if(write_buf == NULL)
		return;
	memset(write_buf, 0x55, 64);
	
	uint8_t *read_buf = (uint8_t *)malloc(64); //每次从cache里面读64字节数据
	if(read_buf == NULL)
		return;
	
	while(1)
	{
		k_cache_write(&cache, write_buf ,64);
		k_cache_read(&cache, read_buf ,64);
		os_thread_sleep(10);
	}
	
	return;
}
#endif