CC = gcc
CFLAGS = -Wall
EXE = proc

$(EXE): main.o mbuf.o proc.o shm.o time.o writen.o log.o time.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	-rm -f tags *.o $(EXE)
