#include "BPlusTree.h"

int main()
{
    string _fileName = "INDEX_test";
    BPlusTree<int> bpt(_fileName, 5, -1);
    // if index file exists, then print directly, else insert data and write to disk
    string _dir = "./index/" + _fileName;
    if (access(_dir.c_str(), 0) == -1)
    {
        cout << "-------------------------insert(0-49)--------------------------" << endl;

        for (int i = 0; i < 50; i++)
            bpt._insert_BT(i, i);
        // bpt._print_leaf();
        bpt._print_every_level();

        cout << "-------------------------delete(0-19)--------------------------" << endl;

        for (int i = 0; i < 20; i++)
            bpt._remove_BT(i);
        // bpt._print_leaf();
        bpt._print_every_level();
    }
    else
        bpt._print_every_level();

    return 0;
}
