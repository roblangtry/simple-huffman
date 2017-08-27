EXECUTABLE_NAME = program
CFLAGS = -Wall -pedantic -O3 -ansi

prepare_tests:
	echo "$(EXECUTABLE_NAME).out" > tests/test.temp

run_tests:
	python tests/test_suite.py

cleanup_tests:
	rm tests/test.temp

do_tests: prepare_tests run_tests cleanup_tests

build:
	gcc $(CFLAGS) src/*.c -lm -o $(EXECUTABLE_NAME).out

build_debug:
	gcc $(CFLAGS) -g src/*.c -lm -o $(EXECUTABLE_NAME).out

test: build do_tests

new_build:
	gcc $(CFLAGS) src/*.c -lm -o simple-huffman

new_test:
	bash scripts/new_test.sh

new: new_build new_test

default:
	build
