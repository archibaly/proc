#ifndef _PROC_H_
#define _PROC_H_

#include <unistd.h>

#define MAX_PROCESSES	128

typedef void (*spawn_proc_pt) (void *args);

typedef struct {
	pid_t pid;
	void *args;
	char *name;
	int status;
	spawn_proc_pt proc;
} proc_t;

int proc_spawn(spawn_proc_pt proc, void *args, char *name, int respawn);
void proc_wait(void);

extern int last_proc;
extern int proc_slot;
extern proc_t processes[];

#endif /* _PROC_H_ */
