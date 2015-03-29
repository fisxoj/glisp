FILES = types.c util.c environment.c function.c primitives.c eval.c print.c glisp.c

default:
	gcc $(FILES) -o glisp -I. -std=c11 -g
