gcc: main.c functions.c functions.h
	gcc main.c functions.c -O2 -o Hx

clang: main.c functions.c functions.h
	clang main.c functions.c -O2 -o Hx
