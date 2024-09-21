main.out : main.c shell_utils.h string_handling.c launch.c history.c signal_handler.c launch_pipe.c error_handling.c
	gcc main.c string_handling.c launch.c history.c signal_handler.c launch_pipe.c error_handling.c -g -o main.out
