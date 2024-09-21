main.out : main.c ./utils/shell_utils.h ./utils/string_handling.c ./utils/launch.c ./utils/history.c ./utils/signal_handler.c ./utils/launch_pipe.c ./utils/error_handling.c
	gcc main.c ./utils/string_handling.c ./utils/launch.c ./utils/history.c ./utils/signal_handler.c ./utils/launch_pipe.c ./utils/error_handling.c -g -o main.out

make clean:
	rm main.out