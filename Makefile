
main: src/main.c src/imageprocessing.c src/cronometro.c
	gcc -omain src/main.c src/imageprocessing.c  src/cronometro.c -lfreeimage -lpthread
