CC = gcc
OPTIONS = -W -Wall -std=c89 -pedantic -O3

J_controls.o: clean J_controls.c J_controls.h
	$(CC) $(OPTIONS) -I Test -c J_controls.c

test: J_controls.o J_ctr_test.o
	$(CC) $(OPTIONS) J_controls.o J_ctr_test.o -o J_ctr_test

J_ctr_test.o: Test/J_ctr_test.h Test/J_ctr_test.c
	$(CC) $(OPTIONS) -c Test/J_ctr_test.c

clean:
	rm -f *.o *~ J_ctr_test