

normal : normal.o boxmuller.o
	gcc  normal.o boxmuller.o -lm -o normal

normal.o : normal.c boxmuller.h
	gcc -c normal.c -o normal.o

boxmuller.o : boxmuller.c boxmuller.h
	gcc -c boxmuller.c -o boxmuller.o

clean :
	rm *.o normal
