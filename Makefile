
main: src/main.c
	gcc -omain src/main.c src/imageprocessing.c -lfreeimage -lpthread
