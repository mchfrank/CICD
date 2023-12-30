CC= gcc -std=c11
CFLAGS= -g -Wall -Wextra -Werror
OS = $(shell uname)
SOURCES= s21_grep.c

ifeq ($(OS), Linux)
 LINUX_LIBS=-D_GNU_SOURCE
endif

all: clean s21_grep test

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES) -lpcre $(LINUX_LIBS) -o s21_grep

test:
	chmod +x t_grep.sh
	bash t_grep.sh

check:
	cp ../../materials/linters/CPPLINT.cfg ./
	python3 ../../materials/linters/cpplint.py --extensions=c ./*.c
	rm -rf *.cfg

clean:
	rm -rf s21_grep 
	rm -rf *.a *.out *.o log.txt *.cfg *.dSYM

rebuild:
	make clean
	make all
	rm -rf  *.o *.a