CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -I.
LDFLAGS = -lcheck -lsubunit -lrt -lpthread -lm
WRAP_FLAGS = -Wl,--wrap=calloc \
             -Wl,--wrap=s21_calc_complements \
             -Wl,--wrap=s21_transpose \
             -Wl,--wrap=s21_mult_number \
             -Wl,--wrap=s21_determinant \
             -Wl,--wrap=s21_create_matrix \
             -Wl,--wrap=s21_get_minor

FUNCDIR = s21_matrix
TESTDIR = s21_test
BUILD_DIR = build
REPORTDIR = gcov.report

LIB = s21_matrix.a
TEST_BIN = $(BUILD_DIR)/s21_test.out

LIB_SRC = $(wildcard $(FUNCDIR)/*.c)
LIB_OBJ = $(LIB_SRC:.c=.o)

TEST_SRC = $(wildcard $(TESTDIR)/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)

GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TEST_DEFS = -DS21_TESTING

.PHONY: all clean test gcov_report

all: $(LIB)

$(LIB): $(LIB_OBJ)
	$(AR) rcs $@ $^

$(FUNCDIR)/%.o: $(FUNCDIR)/%.c s21_matrix.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTDIR)/%.o: $(TESTDIR)/%.c s21_matrix.h
	$(CC) $(CFLAGS) $(TEST_DEFS) -c $< -o $@

$(TEST_BIN): $(TEST_OBJ) $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_OBJ) -o $@ $(LIB) $(LDFLAGS) $(WRAP_FLAGS) -ldl

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: clean $(BUILD_DIR)
	$(MAKE) CFLAGS="$(CFLAGS) $(GCOV_FLAGS) $(TEST_DEFS)" LDFLAGS="$(LDFLAGS) $(WRAP_FLAGS) -ldl" $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: test
	mkdir -p $(REPORTDIR)
	gcov -b $(FUNCDIR)/*.c
	lcov -c -d . -b . --no-external \
		--exclude '*/s21_test/*' \
		--exclude '*/build/*' \
		--ignore-errors unused \
		-o coverage.info
	genhtml coverage.info -o $(REPORTDIR)
	@echo "Report: $(REPORTDIR)/index.html"

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LIB_OBJ) $(TEST_OBJ) $(LIB) $(TEST_BIN)
	rm -f $(FUNCDIR)/*.gcno $(FUNCDIR)/*.gcda $(FUNCDIR)/*.gcov
	rm -f $(TESTDIR)/*.gcno $(TESTDIR)/*.gcda $(TESTDIR)/*.gcov
	rm -f *.gcno *.gcda *.gcov
	rm -f coverage.info
	rm -rf $(REPORTDIR)

install: all
	cp $(LIB) /usr/local/lib/
	cp s21_matrix.h /usr/local/include/

uninstall:
	rm -f /usr/local/lib/$(LIB)
	rm -f /usr/local/include/s21_matrix.h