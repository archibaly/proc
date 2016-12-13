#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "proc.h"
#include "log.h"

int last_proc;
int proc_slot;

proc_t processes[MAX_PROCESSES];

int proc_spawn(spawn_proc_pt proc, void *args, char *name, int respawn)
{
	int s;
	pid_t pid;
	spawn_proc_pt proc_tmp = proc;
	void *args_tmp = args;

	for (s = 0; s < last_proc; s++) {
		if (processes[s].pid == -1) {	/* must exited */
			proc_tmp = processes[s].proc;
			args_tmp = processes[s].args;
			break;
		}
	}

	if (s == MAX_PROCESSES) {
		log_write(LOG_ERRO, "no more than %d processes can be spawned",
				  MAX_PROCESSES);
		return -1;
	}

	proc_slot = s;

	pid = fork();
	if (pid < 0) {
		return -1;
	} else if (pid == 0) {
		proc_tmp(args_tmp);
	}

	processes[s].pid = pid;

	if (respawn)
		return pid;

	processes[s].args = args;
	processes[s].name = name;
	processes[s].proc = proc;

	if (s == last_proc)
		last_proc++;

	return pid;
}

void proc_wait(void)
{
	pid_t pid;
	int i, status;
	char *proc_name;

	for (;;) {
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == 0)
			return;

		if (pid == -1) {
			if (errno == EINTR)
				continue;
			else
				return;
		}

		proc_name = "unknown process";

		for (i = 0; i < last_proc; i++) {
			if (processes[i].pid == pid) {
				processes[i].pid = -1;
				proc_spawn(NULL, NULL, NULL, 1);
				proc_name = processes[i].name;
				break;
			}
		}

		if (WTERMSIG(status)) {
			log_write(LOG_INFO, "%s(%d) exited on signal %d", proc_name, pid,
					  WTERMSIG(status));
		} else {
			log_write(LOG_INFO, "%s(%d) exited with code %d", proc_name, pid,
					  WEXITSTATUS(status));
		}
	}
}
