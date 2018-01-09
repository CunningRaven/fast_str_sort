all: fast_str_sort.o

test: fast_str_sort.o test.c
	gcc -Wall -std=c99 -lm $^ -o $@

%.o: %.c
	gcc -c -Wall -std=c99 $< -o $@

clean: 
	-@rm fast_str_sort.o
	-@rm test

clear: clean

.PHONY: clean clear
