CC = gcc
FLAGS = -std=c11 -g -Wall -Werror -Wextra -D_POSIX_C_SOURCE=199309L
GCOV_FLAGS = --coverage

SOURCE_TEST_C = $(wildcard ./tests/*.c)
SOURCE_TEST_H = $(wildcard ./tests/*.h)

LIB = s21_tetris
LIBNAME = $(LIB).a
PROJECT = tetris
TEST = tetris_test
LIB_DEST = brick_game/tetris/game_logic.c
GUI_DEST = gui/cli/cli.c
LIB_DIR = lib
BUILD_DIR = ../build
DIST_DIR = dist
DOCS_DIR = docs
TESTS_DIR = tests
GCOV_REPORT_DIR := report
SYSTEM := $(shell uname -s)

ifeq ($(SYSTEM), Linux)
    # Код для компиляции под Linux
    OPEN_CMD = xdg-open
    ADD_LIB = -lncurses -lcheck -lsubunit -lm
else ifeq ($(SYSTEM), Darwin)
    # Код для компиляции под macOS
    OPEN_CMD = open
    ADD_LIB = -lcheck -lncurses
    BREW := ${HOME}/homebrew
else
    $(error Unsupported system: $(SYSTEM))
endif

PATH := $(BREW)/bin:$(PATH)

all: gcov_report install dvi dist

install: clean build_dir $(LIBNAME)
	$(CC) $(FLAGS) brick_game/tetris/main.c brick_game/tetris/fsm.c $(GUI_DEST) $(LIB_DIR)/$(LIBNAME) -o $(LIB_DIR)/$(PROJECT) ${ADD_LIB}
	mv $(LIB_DIR)/$(PROJECT) $(BUILD_DIR)

$(LIBNAME): lib_dir
	$(CC) $(FLAGS) -c $(LIB_DEST) -o $(LIB_DIR)/$(LIB).o
	ar rc $(LIB_DIR)/$(LIB).a $(LIB_DIR)/$(LIB).o
	ranlib $(LIB_DIR)/$(LIB).a

uninstall:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(DOCS_DIR)/latex $(DOCS_DIR)/html $(DOCS_DIR)/*.dvi
	rm -rf /tmp/highscore.txt
dvi:
	doxygen Doxyfile

dist: dist_dir
	cp -rf brick_game/ $(DIST_DIR)/
	cp -rf gui/ $(DIST_DIR)/
	cp -rf docs/fsm_diagram.jpg $(DIST_DIR)/
	cp -rf Makefile $(DIST_DIR)/
	cp -rf Doxyfile ${DIST_DIR}/
	tar -czf tetris_v1.0.tar.gz ${DIST_DIR}/
	rm -rf $(DIST_DIR)/*
	mv tetris_v1.0.tar.gz $(DIST_DIR)/

test: clean $(LIBNAME)
	$(CC) $(FLAGS) ${SOURCE_TEST_C} $(LIB_DIR)/$(LIBNAME) -o $(TESTS_DIR)/$(TEST) ${ADD_LIB}
	./$(TESTS_DIR)/$(TEST)

# test_compile: $(LIBNAME)
# 	$(CC) $(FLAGS) $(SOURCE_TEST_C) $(LIB_DIR)/$(LIBNAME) -o $(TESTS_DIR)/$(TEST) $(ADD_LIB)
# 	./$(TESTS_DIR)/$(TEST)

gcov_$(LIBNAME):
	$(CC) $(FLAGS) $(GCOV_FLAGS) -c $(LIB_DEST) -o $(GCOV_REPORT_DIR)/$(LIB).o
	ar rc $(GCOV_REPORT_DIR)/$(LIB).a $(GCOV_REPORT_DIR)/$(LIB).o
	ranlib $(GCOV_REPORT_DIR)/$(LIB).a
	rm -rf $(GCOV_REPORT_DIR)/$(LIB).o

test_gcov: gcov_$(LIBNAME)
	$(CC) $(FLAGS) $(GCOV_FLAGS) ${SOURCE_TEST_C} $(GCOV_REPORT_DIR)/$(LIBNAME) -o $(TESTS_DIR)/$(TEST) ${ADD_LIB}
	./$(TESTS_DIR)/$(TEST)

gcov_report: clean gcov_dir test_gcov
	lcov -c -d . -o $(GCOV_REPORT_DIR)/coverage.info --no-external  # Capture coverage data
	lcov --remove $(GCOV_REPORT_DIR)/coverage.info 'tests/*' -o $(GCOV_REPORT_DIR)/coverage_filtered.info  # Filter out test directories
	genhtml $(GCOV_REPORT_DIR)/coverage_filtered.info -o $(GCOV_REPORT_DIR)/html  # Generate HTML report
	open $(GCOV_REPORT_DIR)/html/index.html
	rm -rf $(TESTS_DIR)/*.dSYM $(TESTS_DIR)/*.gcda $(TESTS_DIR)/*.gcno $(TESTS_DIR)/*.o

lib_dir:
	mkdir $(LIB_DIR)

build_dir:
	mkdir $(BUILD_DIR)

dist_dir:
	mkdir $(DIST_DIR)

gcov_dir:
	mkdir $(GCOV_REPORT_DIR)

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(DOCS_DIR)/latex $(DOCS_DIR)/html $(DOCS_DIR)/*.dvi $(DIST_DIR) $(GCOV_REPORT_DIR) $(TESTS_DIR)/$(TEST) score
	rm -rf $(TESTS_DIR)/*.gcda $(TESTS_DIR)/*.gcno $(TESTS_DIR)/*.o $(TESTS_DIR)/*.dSYM
	rm -rf *.gcno *.gcda *.txt
	rm -rf /tmp/highscore.txt

leaks: clean test
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes ./$(TESTS_DIR)/$(TEST)

check:
	clang-format -style=google -i $(LIB_DEST) $(GUI_DEST) brick_game/tetris/game_logic.h gui/cli/cli.h brick_game/tetris/main.c brick_game/tetris/fsm.c ${SOURCE_TEST_C} ${SOURCE_TEST_H}
	# cppcheck enable=all missingIncludeSystem ${SOURCE_TEST_C}
	cppcheck enable=all missingIncludeSystem $(LIB_DEST) $(GUI_DEST) brick_game/tetris/main.c brick_game/tetris/fsm.c --check-level=exhaustive

pcheck:
	clang-format -style=google -n $(LIB_DEST) $(GUI_DEST) brick_game/tetris/game_logic.h gui/cli/cli.h brick_game/tetris/main.c ${SOURCE_TEST_C} ${SOURCE_TEST_H}

rebuild: clean all
