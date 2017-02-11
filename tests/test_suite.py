import unittest, sys
from test_lib.files import generate_random_int_file, \
    generate_ordered_int_file, generate_cyclical_int_file

LOG_FILE = 'debug.log'
PROGRAM_NAME = None
import logging, os, time, subprocess, filecmp


def assert_same_file(file1, file2):
    assert(filecmp.cmp(file1, file2, shallow=False))

def do_compression(filename):
    compressed_filename = 'c_' + filename
    command = './%s -c %s %s' % (PROGRAM_NAME, filename, compressed_filename)
    status = os.system(command)
    if status > 255:
        # this is a weird quirk between unix and windows
        # on windows os.system returns the status code returned
        # by the program as is but unix encodes something fancy
        # in the low order byte and puts the status code in the
        # second byte so assume if its over 255 its unix status
        # in the 2nd byte
        status = status >> 8

    assert(status == 1)
    return compressed_filename
def do_decompression(filename):
    decompressed_filename = 'd_' + filename
    command = './%s -d %s %s' % (PROGRAM_NAME, filename, decompressed_filename)
    status = os.system(command)
    if status > 255:
        # this is a weird quirk between unix and windows
        # on windows os.system returns the status code returned
        # by the program as is but unix encodes something fancy
        # in the low order byte and puts the status code in the
        # second byte so assume if its over 255 its unix status
        # in the 2nd byte
        status = status >> 8
    assert(status == 1)
    return decompressed_filename


class TestRandomFiles(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        generate_random_int_file('small.temp', 0, 102, 1024)
        generate_random_int_file('medium.temp', 0, 1024, 1024 * 100)
        generate_random_int_file('large.temp', 0, 1024, 1024 * 1000 * 10)
    @classmethod
    def tearDownClass(self):
        if os.path.isfile('small.temp') is True:
            os.remove('small.temp')
        if os.path.isfile('medium.temp') is True:
            os.remove('medium.temp')
        if os.path.isfile('large.temp') is True:
            os.remove('large.temp')
        if os.path.isfile('c_small.temp') is True:
            os.remove('c_small.temp')
        if os.path.isfile('c_medium.temp') is True:
            os.remove('c_medium.temp')
        if os.path.isfile('c_large.temp') is True:
            os.remove('c_large.temp')
        if os.path.isfile('d_c_small.temp') is True:
            os.remove('d_c_small.temp')
        if os.path.isfile('d_c_medium.temp') is True:
            os.remove('d_c_medium.temp')
        if os.path.isfile('d_c_large.temp') is True:
            os.remove('d_c_large.temp')
    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Random.small')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Random.medium')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Random.large')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)


class TestOrderedFiles(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        generate_ordered_int_file('small.temp', 0, 1, 1024)
        generate_ordered_int_file('medium.temp', 0, 1, 1024 * 100)
        generate_ordered_int_file('large.temp', 0, 1, 1024 * 1000 * 10)
    @classmethod
    def tearDownClass(self):
        if os.path.isfile('small.temp') is True:
            os.remove('small.temp')
        if os.path.isfile('medium.temp') is True:
            os.remove('medium.temp')
        if os.path.isfile('large.temp') is True:
            os.remove('large.temp')
        if os.path.isfile('c_small.temp') is True:
            os.remove('c_small.temp')
        if os.path.isfile('c_medium.temp') is True:
            os.remove('c_medium.temp')
        if os.path.isfile('c_large.temp') is True:
            os.remove('c_large.temp')
        if os.path.isfile('d_c_small.temp') is True:
            os.remove('d_c_small.temp')
        if os.path.isfile('d_c_medium.temp') is True:
            os.remove('d_c_medium.temp')
        if os.path.isfile('d_c_large.temp') is True:
            os.remove('d_c_large.temp')
    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Ordered.small')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Ordered.medium')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Ordered.large')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)


class TestCyclicalFiles(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        generate_cyclical_int_file('small.temp', 0, 102, 1024)
        generate_cyclical_int_file('medium.temp', 0, 1024, 1024 * 100)
        generate_cyclical_int_file('large.temp', 0, 1024, 1024 * 1000 * 10)
    @classmethod
    def tearDownClass(self):
        if os.path.isfile('small.temp') is True:
            os.remove('small.temp')
        if os.path.isfile('medium.temp') is True:
            os.remove('medium.temp')
        if os.path.isfile('large.temp') is True:
            os.remove('large.temp')
        if os.path.isfile('c_small.temp') is True:
            os.remove('c_small.temp')
        if os.path.isfile('c_medium.temp') is True:
            os.remove('c_medium.temp')
        if os.path.isfile('c_large.temp') is True:
            os.remove('c_large.temp')
        if os.path.isfile('d_c_small.temp') is True:
            os.remove('d_c_small.temp')
        if os.path.isfile('d_c_medium.temp') is True:
            os.remove('d_c_medium.temp')
        if os.path.isfile('d_c_large.temp') is True:
            os.remove('d_c_large.temp')
    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Cyclical.small')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Cyclical.medium')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)
    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Cyclical.large')
        uncompressed_size = os.path.getsize(filename)
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        assert_same_file(filename, decompressed_filename)
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)


if __name__ == '__main__':
    logging.basicConfig(filename=LOG_FILE,level=logging.DEBUG)
    temp_file = open('tests/test.temp', 'r')
    PROGRAM_NAME = temp_file.readline().replace('\n', '')
    unittest.main()