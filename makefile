EXECUTABLE_NAME = program
CFLAGS = -Wall -pedantic -O3

prepare_tests:
	echo "$(EXECUTABLE_NAME).out" > tests/test.temp

run_tests:
	python tests/test_suite.py

cleanup_tests:
	rm tests/test.temp

do_tests: prepare_tests run_tests cleanup_tests

build:
	gcc $(CFLAGS) src/*.c -lm -o $(EXECUTABLE_NAME).out

test: build do_tests

default:
	build
