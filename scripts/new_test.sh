echo 'small.data test'
echo '  COMPRESS'
time ./simple-huffman --new=encode < small.data > small.stk
echo '  DECOMPRESS'
time ./simple-huffman --new=decode < small.stk > small.out
cmp small.data small.out
ls -l small.stk
rm small.stk
rm small.out
echo 'OLD wsj.data test'
time ./simple-huffman -c test.data test.stk -I
time ./simple-huffman -d test.stk test.out -I
cmp test.data test.out
ls -l test.stk
rm test.stk
rm test.out
echo 'NEW wsj.data test'
time cat test.data | ./simple-huffman --new=encode > test.stk
time cat test.stk | ./simple-huffman --new=decode > test.out
cmp test.data test.out
ls -l test.stk
rm test.stk
rm test.out