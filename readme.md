### BPlusTree Template Class

this template class `BPlusTree.h` realise almost functions of bplustree

it's suitable for any order, any data type, and any case.

`test.cpp` offers a simple test for `BPlusTree.h`

the method to compile and run the code:
(open your ternimal firstly)
~~~cpp
# you need to enter
bash exec.sh 
# then exec.sh will give you a hint:
please enter the executable file name:
# you need to enter any file name you like to generate execuable file, such as test
test
# then result will be given, test sample:
-------------------------insert(0-49)--------------------------
18   
24 30 36 42   
20 22   26 28   32 34   38 40   44 46   
18 19  20 21  22 23  24 25  26 27  28 29  30 31  32 33  34 35  36 37  38 39  40 41  42 43  44 45  46 47 48 49  
-------------------------delete(0-19)--------------------------
20   
30 36 42   
22 24 26 28   32 34   38 40   44 46   
20 21  22 23  24 25  26 27  28 29  30 31  32 33  34 35  36 37  38 39  40 41  42 43  44 45  46 47 48 49 
~~~

useful website: https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html
