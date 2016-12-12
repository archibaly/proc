#ifndef _SHM_H_
#define _SHM_H_

#include <stddef.h>

typedef struct {
	size_t size;
	unsigned char *addr;
} shm_t;

int shm_alloc(shm_t *shm);
void shm_free(shm_t *shm);

#endif /* _SHM_H_ */
