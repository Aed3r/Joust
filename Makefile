CC = gcc
OPTIONS = -W -Wall -std=c89 -pedantic -O3 -I
all:
	+$(MAKE) -C Modules/Objects
	+$(MAKE) -C Modules/Controls
	+$(MAKE) -C Modules/Screen
test : Modules/Objects/J_objects.o Modules/Controls/J_controls.o Modules/Screen/J_screen.o test.c
clean :
	rm -f *.o test 