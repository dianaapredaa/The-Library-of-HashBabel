# Copyright 2021 - 2022 Preda Diana 314CA

build:
	gcc -g main.c hashmap.c book_commands.c def_commands.c user_commands.c top.c exit_commands.c -lm -Wall -Wextra -std=c99 -o main

run:
	valgrind --leak-check=full --track-origins=yes ./main
clean:
	rm -f main

