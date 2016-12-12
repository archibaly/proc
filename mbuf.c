#include <string.h>

#include "mbuf.h"
#include "writen.h"

static shm_t mbuf;

int mbuf_init(size_t size)
{
	mbuf.size = sizeof(mbuf_t);

	if (shm_alloc(&mbuf) < 0)
		return -1;

	mbuf_t *pm = (mbuf_t *)mbuf.addr;
	pm->base.size = size;
	if (shm_alloc(&pm->base) < 0) {
		shm_free(&mbuf);
		return -1;
	}

	return 0;
}

int mbuf_get_len(void)
{
	mbuf_t *pm = (mbuf_t *)mbuf.addr;
	if (!pm)
		return -1;

	return pm->count;
}

int mbuf_add(unsigned char *data, int size)
{
	int tail;
	mbuf_t *pm = (mbuf_t *)mbuf.addr;
	if (!pm)
		return -1;

	if (pm->count < pm->base.size) {
		tail = (pm->head + pm->count) % pm->base.size;
		memcpy(pm->base.addr + tail, data, size);
		pm->count += size;
		return 0;
	} else {
		return -1;
	}
}

int mbuf_write(int fd)
{
	int len, n;
	mbuf_t *pm = (mbuf_t *)mbuf.addr;
	if (!pm)
		return -1;

	len = mbuf_get_len();
	if (pm->count > 0) {
		n = writen(fd, pm->base.addr + pm->head, len);
		pm->head += n;
		pm->head %= pm->base.size;
		pm->count -= n;
		return n;
	} else {
		return -1;
	}
}

int mbuf_free(void)
{
	mbuf_t *pm = (mbuf_t *)mbuf.addr;
	if (!pm)
		return -1;

	if (pm->base.addr)
		shm_free(&pm->base);

	if (mbuf.addr)
		shm_free(&mbuf);

	return 0;
}
