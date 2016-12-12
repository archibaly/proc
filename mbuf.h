#ifndef _MBUF_H_
#define _MBUF_H_

#include <stddef.h>

#include "shm.h"

typedef struct {
	int head;
	int count;
	shm_t base;
} mbuf_t;

int mbuf_init(size_t size);
int mbuf_get_len(void);
int mbuf_add(unsigned char *data, int size);
int mbuf_write(int fd);
int mbuf_free(void);

#endif /* _MBUF_H_ */
