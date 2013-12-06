CC = gcc -g
FLAGS = `pkg-config --cflags --libs allegro-5.0 allegro_primitives-5.0 allegro_image-5.0`

all: rpg

rpg: main.c runtime.o file.o logic.o event.o graphics.o
	$(CC) -o $@ $^ $(FLAGS)

graphics.o: graphics.c runtime.o
	$(CC) -c $<

event.o: event.c runtime.o
	$(CC) -c $<

logic.o: logic.c runtime.o
	$(CC) -c $<

file.o: file.c runtime.o
	$(CC) -c $<

runtime.o: runtime.c
	$(CC) -c $<

clean:
	rm -f rpg *.o
