#include "BPlusTree.h"

int main()
{
    BPlusTree<int> bpt("test", 5, sizeof(int));
    bpt._insert_BT(5, 5);
    bpt._insert_BT(8, 8);
    bpt._insert_BT(10, 10);
    bpt._insert_BT(15, 15);
    bpt._insert_BT(16, 16);
    bpt._insert_BT(17, 17);
    bpt._insert_BT(18, 18);
    bpt._insert_BT(6, 6);
    bpt._insert_BT(9, 9);
    bpt._insert_BT(19, 19);
    bpt._insert_BT(20, 20);
    bpt._insert_BT(21, 21);
    bpt._insert_BT(22, 22);
    bpt._insert_BT(7, 7);
    bpt._print_leaf();

    bpt._remove_BT(22);
    bpt._remove_BT(15);
    bpt._remove_BT(7);
    bpt._print_leaf();

    return 0;
}