#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shm.h"

int shm_alloc(shm_t *shm)
{
	int id;

	id = shmget(IPC_PRIVATE, shm->size, (SHM_R | SHM_W | IPC_CREAT));

	if (id == -1) {
		fprintf(stderr, "shmget(%uz) failed: %s\n", shm->size, strerror(errno));
		return -1;
	}

	shm->addr = shmat(id, NULL, 0);

	if (shm->addr == (void *)-1)
		fprintf(stderr, "shmat() failed: %s\n", strerror(errno));

	if (shmctl(id, IPC_RMID, NULL) == -1)
		fprintf(stderr, "shmctl(IPC_RMID) failed: %s\n", strerror(errno));

	return (shm->addr == (void *)-1) ? -1 : 0;
}

void shm_free(shm_t *shm)
{
	if (shmdt(shm->addr) == -1)
		fprintf(stderr, "shmdt(%p) failed: %s\n", shm->addr, strerror(errno));
}
