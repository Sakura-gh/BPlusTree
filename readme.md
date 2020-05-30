### BPlusTree Template Class

##### introduction

This template class `BPlusTree.h` implemented most functions of bplustree. It's suitable for any order and any data type.

Now I have used `BPlusTree.h` to Implement function interface for minisql with file management:

- `IndexManager.h` and `IndexManager.cpp`：the interface for minisql
- `BPlusTree.h`：implement the bplustree and store {key: value} in file named `./index/`
- `basis.h` and `basis.cpp`：give some basis enum, struct, const and function

##### attention!

this code runs in Linux, and for other os, you need to modify a little in `BPlusTree.h`：

- for `#include` in linux：

~~~cpp
#include <unistd.h> 
#include <sys/stat.h> 
~~~

change to (in other os):

~~~cpp
#include <io.h>
#include <direct.h>
~~~


- for `void BPlusTree<T>::_init_from_disk()` in Linux

~~~cpp
if (access(_fileName.c_str(), 0) != -1){...}
~~~

change to (in other os):

~~~cpp
if (_access(_fileName.c_str(), 0) == -1){...}
~~~

- for `void BPlusTree<T>::_write_disk()` in Linux

~~~cpp
// if dir not exists, mkdir
if (access(_dir.c_str(), 0) == -1)
    mkdir(_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
~~~

change to (in other os):

~~~cpp
if (_access(_dir.c_str(), 0) == -1)
    _mkdir(_dir.c_str());
~~~

##### compile and test

for Linux/ macos/ unix:

`exec.sh` can automatically find all the `.cpp` files and compile them into `.o`  executable file, run finally

`test.cpp` gives a simple test for `BPlusTree.h`

- if `INDEX_test` file in dir `./index/` not exists , it will create bplustree with insert 0-49 firstly and delete 0-19 later, finally it will store the {key:value} in `./index/INDEX_test`
- if `INDEX_test` file in dir `./index/` exists, it will read the {key: value} from this file and build bplustree automatically, after that, it wll print node's key in all tree level. 

the method to compile and run the code in `exec.sh`:
(open your ternimal firstly)

~~~bash
$ bash exec.sh 
please enter the executable file name: test # you need to enter test or other file name you like to generate execuable file
g++ basis.cpp IndexManager.cpp test.cpp -std=c++11 -o test.o # some information
compile...
./test.o
execute...
# if ./index/INDEX_test not exists
-------------------------insert(0-49)--------------------------
18   
6 12   24 30 36 42   
2 4   8 10   14 16   20 22   26 28   32 34   38 40   44 46   
0 1  2 3  4 5  6 7  8 9  10 11  12 13  14 15  16 17  18 19  20 21  22 23  24 25  26 27  28 29  30 31  32 33  34 35  36 37  38 39  40 41  42 43  44 45  46 47 48 49  
-------------------------delete(0-19)--------------------------
30 36 42   
22 24 26 28   32 34   38 40   44 46   
20 21  22 23  24 25  26 27  28 29  30 31  32 33  34 35  36 37  38 39  40 41  42 43  44 45  46 47 48 49 

# if ./index/INDEX_test exists
# just print the tree stored in file
26 32 38   
22 24   28 30   34 36   40 42 44 46   
20 21  22 23  24 25  26 27  28 29  30 31  32 33  34 35  36 37  38 39  40 41  42 43  44 45  46 47 48 49  
~~~

for windows (modify `BPlusTree.h` mentioned above firstly !):

you need to enter in cmd manually

~~~bash
g++ basis.cpp IndexManager.cpp test.cpp -std=c++11 -o test.o
./test.o
~~~

##### others

useful website: https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html