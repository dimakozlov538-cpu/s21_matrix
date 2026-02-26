CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I.
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

SRC_DIR = matrix
TEST_DIR = tests
BUILD_DIR = build
LIB_NAME = s21_matrix.a

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_EXECUTABLES = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.out)

.PHONY: all clean test gcov_report

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

test: $(LIB_NAME) $(TEST_EXECUTABLES)
	@echo "Запуск всех тестов..."
	@cd $(BUILD_DIR) && \
	for test in *.out; do \
		echo ">>> Запуск $$test"; \
		./$$test || exit 1; \
	done

$(BUILD_DIR)/%.out: $(TEST_DIR)/%.c $(LIB_NAME)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ $< $(LIB_NAME) -lcheck -lsubunit -lm -lpthread -ldl

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) *.gcda *.gcno *.gcov gcov_report coverage.info

gcov_report: test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report