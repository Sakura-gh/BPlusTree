#include "BPlusTree.h"

int main()
{
    cout << "-------------------------insert(0-19)--------------------------" << endl;
    BPlusTree<int> bpt("test", 5, sizeof(int));
    for (int i = 0; i < 20; i++)
        bpt._insert_BT(i, i);
    // bpt._print_leaf();
    bpt._print_every_level();

    cout << "-------------------------delete(0--9)--------------------------" << endl;

    for (int i = 0; i < 10; i++)
        bpt._remove_BT(i);
    // bpt._print_leaf();
    bpt._print_every_level();

    return 0;
}