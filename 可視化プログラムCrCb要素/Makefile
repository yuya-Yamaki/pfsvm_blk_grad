#Makefile
makeyuvCrCb:mainCrCb.o yuvCrCb.o
	gcc -Wall -O2 -o makeyuvCrCb mainCrCb.o yuvCrCb.o

mainCrCb.o:mainCrCb.c
	gcc -c mainCrCb.c

yuvCrCb.o:yuvCrCb.c
	gcc -c yuvCrCb.c

clean:
	rm -f makeyuvCrCb mainCrCb.o yuvCrCb.o
