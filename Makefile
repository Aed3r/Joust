CC = gcc
CFLAGS = -DDEBUG -g -W -Wall -std=c89 -pedantic -O3 -I `pkg-config --cflags MLV`
LDFLAGS = `pkg-config --libs-only-other --libs-only-L MLV`
LDLIBS=`pkg-config --libs-only-l MLV`

all: clean
	+$(MAKE) -C Modules/Objects
	+$(MAKE) -C Modules/Controls
	+$(MAKE) -C Modules/Screen
	+$(MAKE) -C Modules/IA
test : Modules/Objects/J_objects.o Modules/Controls/J_controls.o Modules/Screen/J_screen.o Modules/IA/J_ia.o test.c
clean :
	rm -f *.o test 
	+$(MAKE) clean -C Modules/Objects
	+$(MAKE) clean -C Modules/Controls
	+$(MAKE) clean -C Modules/Screen
	+$(MAKE) clean -C Modules/IA