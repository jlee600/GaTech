# Project 4 - CS 2110 Fall 2024
# DO NOT MODIFY FILE
# GCC flags from the syllabus (each flag described for the curious minds!)
# Flag info credit: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

CFLAGS = -std=c99                          # Using the C99 standard
CFLAGS += -Wall                            # This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
CFLAGS += -pedantic                        # Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++
CFLAGS += -Wextra                          # This enables some extra warning flags that are not enabled by -Wall
CFLAGS += -Werror                          # Make all warnings into errors
CFLAGS += -Wstrict-prototypes              # Warn if a function is declared or defined without specifying the argument types
CFLAGS += -Wold-style-definition           # Warn if an old-style function definition is used. A warning is given even if there is a previous prototype
CFLAGS += -Werror=vla                      # Generate an error if variable-length arrays (bad practice in C!) are used

CFILES = spotify.c

ARCH=$(shell uname -m)

TEST_FILES := $(wildcard suites/*.c) suites/$(ARCH)/utils.o

ifdef TEST
	SUITE = $(shell echo $(TEST) | awk -F'::' '{print $$1}')
	TEST_NAME = $(shell echo $(TEST) | awk -F'::' '{print $$2}')
	BREAKPOINT = -ex 'b $(SUITE)_$(TEST_NAME)_impl'
	FILTER = --filter $(SUITE)/$(TEST_NAME)
endif

.PHONY: run-tests
run-tests: tests
	@ ./tests $(FILTER)

.PHONY: run-gdb
run-gdb: tests
	@ gdb -q -ex 'set follow-fork-mode child' $(BREAKPOINT) -ex 'python gdb.execute("handle SIGSTOP nostop noprint")' -ex 'python gdb.execute("handle SIGCONT nostop noprint")' --args ./tests $(FILTER)
	
.PHONY: tests
tests: clean
	@ gcc $(CFILES) -DNO_FAKE_MALLOC -o /dev/null -nostartfiles -Wl,-e,play_next > /dev/null
	@ gcc -fno-asm $(CFLAGS) $(CFILES) $(TEST_FILES) -lcriterion -o tests -g

.PHONY: run-valgrind
run-valgrind: tests
	@ valgrind --quiet --suppressions=suites/valgrind.supp --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all --undef-value-errors=no --trace-children=yes ./tests $(FILTER)

.PHONY: clean
clean:
	@ rm -f *.o tests
