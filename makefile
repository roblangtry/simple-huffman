EXECUTABLE_NAME = program
CFLAGS = -Wall

prepare_tests:
	echo "$(EXECUTABLE_NAME).out" > tests/test.temp

run_tests:
	python tests/test_suite.py

cleanup_tests:
	rm tests/test.conf

do_tests: prepare_tests run_tests cleanup_tests

build:
	gcc $(CFLAGS) src/*.c -o $(EXECUTABLE_NAME).out

test: build do_tests

default:
	build