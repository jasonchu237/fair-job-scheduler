CC=g++
CFLAGS= -std=c++11 -Wall -Werror

jobscheduler:main.o tree.o heap.o scheduler.o
	$(CC) -o jobscheduler main.o tree.o heap.o scheduler.o


main.o:main.cpp
	$(CC) -c $(CFLAGS) main.cpp -o main.o

tree.o:tree.cpp
	$(CC) -c $(CFLAGS) tree.cpp -o tree.o

heap.o:heap.cpp
	$(CC) -c $(CFLAGS) heap.cpp -o heap.o

scheduler.o:scheduler.cpp
	$(CC) -c $(CFLAGS) scheduler.cpp -o scheduler.o

.PHONY:clean

clean:
	rm -rf *.o jobscheduler
