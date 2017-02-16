Simple Huffman
=====

Make Commands
-----

``` bash
make build
```
or
``` bash
make
```
Compile the program as $(EXECUTABLE_NAME).out (EXECUTABLE_NAME may be set as follows ```make build EXECUTABLE_NAME=your_name_here``` and by default it is "program")

```bash
make test
```
Compiles and test the program using the unit tests in tests/test_suite.py. The test/test.conf configuration file can be used to set the logfile name to allow for capturing statistics such as the size of the compressed and decompressed files and the time taken for compression and decompression

-----

#### Note on Tests

There are 3 types of test run and they are detailed below. The Cyclical and Random test types are run on three different file sizes, while the Ordered test are only run two different sizes due to the time it takes to run the larger alphabet compression present in this test type paticularly in larger files.
###### Ordered
Tests the performance of a file with values going incrementally from 1 to N (Where N is the total number of symbols in the file). This tests the performance of compression/decompression on a message with a large symbol alphabet.
###### Cyclical
Tests the performance on a file going cyclically over a period of 1 to I (Where I is a value that meets the criteria, I < N, I=1024 was used in most of the tests in the test suite) this creates a largely balanced situation (where all symbol probabilities for the input alphabet should be comparable)
###### Random
As the name suggests this tests the performance of the compression on a random file generated with unmanaged symbol distributions hopefully leading to a more varied model structure. (Those the random function is in all cases specified to select only values in the range [0, I) to ensure the slow performance of large alphabets isn't encountered)

Program input
-----
Currently the program only accepts files formatted as a series of "%d\n" values.


Once compiled the program should be called as such for compression:
```
./program.out -c inputfile outputfile
./program.out --compression inputfile outputfile
```
Or for decompression as such:
```
./program.out -d inputfile outputfile
./program.out --decompression inputfile outputfile
```
A verbose flag can also be set to print information about model and huffman tree as the file is being processed:
```
./program.out -c -v inputfile outputfile
./program.out -c -verbose inputfile outputfile
```

Environment
-----
This program was built and tested on an Ubuntu 16.04 VirtualBox running with 2 GB of RAM

The software used for the creation were pinned at the following versions
* GCC: gcc (Ubuntu 6.2.0-5ubuntu12) 6.2.0 20161005
* Make: GNU Make 4.1
* Python: Python 2.7.12+


