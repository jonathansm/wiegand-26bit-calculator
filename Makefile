wiegand-calculator: wiegand-calculator.c Makefile
	gcc -pthread -g -Wextra -Wall -std=c99 -o wiegand-calculator wiegand-calculator.c -lm
