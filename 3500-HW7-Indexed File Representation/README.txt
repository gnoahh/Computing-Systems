How to run:

g++ -std=c++11 myNode.cpp -o myNode
.out./myNode.outinput_file1.txt access_trace1.txt


The maximum of blocks this file can store is 12 + 100 + 100 * 100 = 10112

directBlock: direct blocks
singleIndirect: single indirect blocks
doubleIndirect: double indirect blocks
singleArr: array that stores all single indirect blocks
doubleArr: array that stores all double indirect blocks
outerName: the name of the directory
