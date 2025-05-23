CC = gcc
CFLAGS = -Wall -lncursesw

OBJS = main.o game.o game5x5.o ui.o

2048: $(OBJS)
	$(CC) -o 2048 $(OBJS) $(CFLAGS)

clean:
	rm -f *.o 2048
