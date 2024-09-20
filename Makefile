main.out : main.c shell_utils.h stringhandling.c launch.c
	gcc main.c stringhandling.c launch.c -g -o main.out
