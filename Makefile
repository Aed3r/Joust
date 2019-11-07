CC = gcc
OPTIONS = -W -Wall -std=c89 -pedantic -O3 -I

test : test.c J_objects.o J_screen.o J_controls.o

J_screen.o : Modules/Screen/J_screen.c J_objects.o Modules/Screen/J_screen.h

J_controls.o : Modules/Controls/J_controls.c J_objects.o Modules/Controls/J_controls.h

J_objects.o : Modules/Objects/J_objects.c Modules/Objects/J_objects.h

clean :
	rm -f *.o test 