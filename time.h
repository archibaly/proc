#ifndef _TIME_H_
#define _TIME_H_

#include <time.h>

struct time {
	int year;
	int mon;
	int mday;
	int wday;
	int hour;
	int min;
	int sec;
};

int time_get(struct time *t);
int time_fmt(char *s, size_t size, const char *fmt);
time_t time_sec(void);

#endif /* _TIME_H_ */
