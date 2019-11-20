CC = gcc
OPTIONS = -W -Wall -std=c89 -pedantic -O3

J_objects.o: clean J_objects.c J_objects.h
	$(CC) $(OPTIONS) -I Test -c J_objects.c

test: J_objects.o J_obj_test.o
	$(CC) $(OPTIONS) J_objects.o J_obj_test.o -o J_obj_test

J_obj_test.o: Test/J_obj_test.h Test/J_obj_test.c
	$(CC) $(OPTIONS) -c Test/J_obj_test.c

clean:
	rm -f *.o *~ J_obj_test