# Victor Forbes - 9293394

all:
	@gcc src/*.c -I include/ -o main -g -Wall
run:
	@./main
fullrun:
	@valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./main