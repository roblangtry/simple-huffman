import unittest, sys, ConfigParser
import logging, os, time, subprocess, filecmp
from test_lib.files import generate_random_int_file, \
    generate_ordered_int_file, generate_cyclical_int_file

PROGRAM_NAME = None
LOGGING_LEVELS = {
    'critical': logging.CRITICAL,
    'error': logging.ERROR,
    'warning': logging.WARNING,
    'info': logging.INFO,
    'debug': logging.DEBUG,
    'notset': logging.NOTSET
}


def assert_same_file(file1, file2):
    """assert that file1 and file2 have the same content"""
    assert(filecmp.cmp(file1, file2, shallow=False))


def do_compression(filename):
    """Run the program in compression mode on the given file"""
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
    """Run the program in decompression mode on the given file"""
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


def delete_if_exists(filename):
    """Delete a file if it exists"""
    if os.path.isfile(filename) is True:
        os.remove(filename)


class TestRandomFiles(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        generate_random_int_file('small.temp', 0, 102, 1024)
        generate_random_int_file('medium.temp', 0, 1024, 1024 * 100)
        generate_random_int_file('large.temp', 0, 1024, 1024 * 1000 * 10)

    @classmethod
    def tearDownClass(self):
        delete_if_exists('small.temp')
        delete_if_exists('medium.temp')
        delete_if_exists('large.temp')
        delete_if_exists('c_small.temp')
        delete_if_exists('c_medium.temp')
        delete_if_exists('c_large.temp')
        delete_if_exists('d_c_small.temp')
        delete_if_exists('d_c_medium.temp')
        delete_if_exists('d_c_large.temp')

    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Random.small')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Random.medium')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Random.large')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
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
        delete_if_exists('small.temp')
        delete_if_exists('medium.temp')
        delete_if_exists('large.temp')
        delete_if_exists('c_small.temp')
        delete_if_exists('c_medium.temp')
        delete_if_exists('c_large.temp')
        delete_if_exists('d_c_small.temp')
        delete_if_exists('d_c_medium.temp')
        delete_if_exists('d_c_large.temp')

    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Ordered.small')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Ordered.medium')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Ordered.large')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
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
        delete_if_exists('small.temp')
        delete_if_exists('medium.temp')
        delete_if_exists('large.temp')
        delete_if_exists('c_small.temp')
        delete_if_exists('c_medium.temp')
        delete_if_exists('c_large.temp')
        delete_if_exists('d_c_small.temp')
        delete_if_exists('d_c_medium.temp')
        delete_if_exists('d_c_large.temp')

    def test_small(self):
        filename = 'small.temp'
        log=logging.getLogger('Cyclical.small')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_medium(self):
        filename = 'medium.temp'
        log=logging.getLogger('Cyclical.medium')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)

    def test_large(self):
        filename = 'large.temp'
        log=logging.getLogger('Cyclical.large')
        # get the uncompressed size
        uncompressed_size = os.path.getsize(filename)
        # do the compression and time it for statistics
        compression_before_time = time.time()
        compressed_filename = do_compression(filename)
        compression_after_time = time.time()
        # calculate compression duration and compressed size
        compression_duration = '%0.3f ms' % ((compression_after_time - compression_before_time) * 1000.0)
        compressed_size = os.path.getsize(compressed_filename)
        # do the decompression and time it for statistics
        decompression_before_time = time.time()
        decompressed_filename = do_decompression(compressed_filename)
        decompression_after_time = time.time()
        # calculate decompression duration and decompressed size
        decompression_duration = '%0.3f ms' % ((decompression_after_time - decompression_before_time) * 1000.0)
        decompressed_size = os.path.getsize(decompressed_filename)
        # assert that the decompressed file and the uncompressed file
        # have the same content
        assert_same_file(filename, decompressed_filename)
        # Now log the statistics of the compression/decompression
        log.debug('Uncompressed size -> %d B' % uncompressed_size)
        log.debug('Compressed size -> %d B' % compressed_size)
        log.debug('Compression time -> %s' % compression_duration)
        log.debug('Decompressed size -> %d B' % decompressed_size)
        log.debug('Decompression time -> %s' % decompression_duration)


if __name__ == '__main__':
    config_file = open('tests/test.conf')
    config = ConfigParser.RawConfigParser(allow_no_value=True)
    config.readfp(config_file)
    log_file = config.get("testing", "log_file")
    # get the log filename from the config file (test.conf)
    log_level_text = config.get("testing", "log_level")
    log_level = LOGGING_LEVELS.get(log_level_text.lower(), logging.CRITICAL)
    # parse the log_level set in the config or if it isn't a correct logging
    # level just set the system to only log critical messages
    logging.basicConfig(filename=log_file,level=log_level)
    temp_file = open('tests/test.temp', 'r')
    PROGRAM_NAME = temp_file.readline().replace('\n', '')
    unittest.main()