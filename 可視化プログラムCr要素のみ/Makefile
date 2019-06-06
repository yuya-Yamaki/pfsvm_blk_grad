#Makefile
makeyuv:main.o yuv.o
	gcc -Wall -O2 -o makeyuv main.o yuv.o

main.o:main.c
	gcc -c main.c

yuv.o:yuv.c
	gcc -c yuv.c

clean:
	rm -f makeyuv main.o yuv.o
