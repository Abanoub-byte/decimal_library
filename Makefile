CC = gcc 
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage 
CHECK_FLAGS = $(shell pkg-config --cflags --libs check) -lm

SRC = s21_helpers.c s21_comparison.c s21_converters.c s21_other.c 
OBJ = $(SRC:.c=.o) 
TEST_SRC = tests/test_main.c

all : s21_decimal.a 

s21_decimal.a : $(OBJ)
	ar rcs s21_decimal.a $(OBJ)

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

test : s21_decimal.a
		$(CC) $(CFLAGS) $(TEST_SRC) s21_decimal.a $(CHECK_FLAGS) -o test_runner 
		./test_runner

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(CHECK_FLAGS) -o test_gcov
	./test_gcov
	lcov -t "s21_decimal" -o coverage.info -c -d . --no-external
	genhtml coverage.info -o report 
	@echo "Open report/index.html to view coverage"

clean:
	rm -f *.o *.a *.gcno *.gcda *.info test_runner test_gcov
	rm -rf report

.PHONY: all test gcov_report clean