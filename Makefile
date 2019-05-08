build: game.o menu.o 2048.o
	gcc 2048.o game.o menu.o -o 2048 -lncurses -Wall
2048.0:
	gcc -c 2048.c
game.o: game.c
	gcc -c game.c
menu.o: menu.c
	gcc -c menu.c
run: 2048
	./2048
clean:
	rm -f 2048 *.o

.PHONY: all run
