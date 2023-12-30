CC = gcc
CFLAGS = -Wall -Werror -Wextra
TEST_LIBS = -lcheck -lm -lpthread
GCOVFLAGS = -fprofile-arcs -ftest-coverage

all: s21_cat

s21_cat.o: s21_cat.c
	${CC} ${CFLAGS} -c s21_cat.c -o s21_cat.o

s21_cat: s21_cat.o
	${CC} s21_cat.o -o s21_cat

test: s21_cat
	bash t_cat.sh

clean:
	rm -rf s21_cat.o
	rm -rf s21_cat
	rm -rf log.txt

rebuild: clean all

check:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c