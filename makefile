#
# Makefile that builds project
# 

jc: jc.o token.o
	gcc -g -Wall -std=c11 -o jc jc.o token.o

jc.o: jc.c jc.h token.o
	gcc -g -Wall -std=c11 -c jc.c 

token.o: token.c token.h
	gcc -g -Wall -std=c11 -c token.c

clean:
	rm -f *.o jc *~