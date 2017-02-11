import os.path
from random import randint

def generate_random_int_file(filename, lower_bound, upper_bound, file_length):
    assert(os.path.isfile(filename) is False)
    file = open(filename, 'w')
    length = 0
    while length < file_length:
        value = randint(lower_bound, upper_bound)
        file.write(str(value) + '\n')
        length += 1
    file.close()

def generate_ordered_int_file(filename, start, period, file_length):
    assert(os.path.isfile(filename) is False)
    file = open(filename, 'w')
    length = 0
    value = start
    while length < file_length:
        file.write(str(value) + '\n')
        value += period
        length += 1
    file.close()

def generate_cyclical_int_file(filename, lower_bound, upper_bound, file_length):
    assert(os.path.isfile(filename) is False)
    file = open(filename, 'w')
    length = 0
    value = lower_bound
    while length < file_length:
        file.write(str(value) + '\n')
        value += 1
        if value > upper_bound:
            value = lower_bound
        length += 1
    file.close()
