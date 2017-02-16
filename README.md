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
Compiles and test the program using the unittests in tests/test_suite.py. The test/test.conf configuration file can be used to set the logfile name to allow for capturing statistics such as the size of the compressed and decompressed files and the time taken for compression and decompression

-----

#### Note on Tests

There are 3 types of test run and each (with the exclusion of ordered which only runs on 2 due to time considerations) are run on 3 different sized files.
###### Ordered
Tests the performance of a file going from 1 to N tests to see how compression and decompression function on a message with a large symbol alphabet
###### Cyclical
Tests the performance on a file going cyclically over a period of 1 to I (Where I < N) this creates a largely balanced situation (all frequencies for the input alphabet should be comparable)
###### Random
As the name suggests this tests the performance of the compression on a random file generated with unmanaged symbol distributions (those these distributions are still kept lower than N to keep the tests running fast)

Environment
-----
This program was built and tested on an Ubuntu 16.04 VirtualBox running with 2 GB of RAM

The software used for the creation were pinned at the following versions
* GCC: gcc (Ubuntu 6.2.0-5ubuntu12) 6.2.0 20161005
* Make: GNU Make 4.1
* Python: Python 2.7.12+


