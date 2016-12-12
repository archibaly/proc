#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "mbuf.h"
#include "proc.h"

void proc_write(void *args)
{
	int ret;
	unsigned char data[] = {'A', 'B', 'C', 'D', '\n'};

	for (;;) {
		ret = mbuf_add(data, sizeof(data));
		printf("mbuf_add ret = %d\n", ret);
		sleep(1);
	}
}

void proc_read(void *args)
{
	int ret;

	for (;;) {
		ret = mbuf_write(fileno(stdout));
		printf("mbuf_write ret = %d\n", ret);
		sleep(3);
	}
}

void signal_handler(int signo)
{
}

int main()
{
	if (mbuf_init(4096) < 0)
		exit(-1);

	signal(SIGCHLD, signal_handler);

	proc_spawn(proc_write, NULL, "proc_write", 0);
	proc_spawn(proc_read, NULL, "proc_read", 0);

	for (;;) {
		pause();
		proc_wait();
	}

	return 0;
}
