CC = gcc
OPTIONS = -W -Wall -std=c89 -pedantic -O3

test: clean J_objects.o obj_test.o
	$(CC) $(OPTIONS) J_objects.o obj_test.o -o obj_test

obj_test.o: Test/obj_test.h Test/obj_test.c
	$(CC) $(OPTIONS) -c Test/obj_test.c

J_objects.o: J_objects.c J_objects.h
	$(CC) $(OPTIONS) -I Test -c J_objects.c

clean:
	rm -f *.o *~ obj_test