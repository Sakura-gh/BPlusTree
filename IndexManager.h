///////////////////////////////////////////////////////////////
// IndexManager.h
// created by gehao on 2020/5/29
///////////////////////////////////////////////////////////////

#ifndef _INDEXMANAGE_H
#define _INDEXMANAGE_H

#include "BPlusTree.h"
#include "basis.h"
#include <map>

typedef map<string, BPlusTree<int> *> map_int;
typedef map<string, BPlusTree<string> *> map_string;
typedef map<string, BPlusTree<float> *> map_float;

// index for each table
class IndexManager
{
public:
    // constructor
    IndexManager(string tableName);
    // destructor
    ~IndexManager();
    // create index in bplustree, index store in filePath
    void _create_index(string filePath, int type);
    // drop index
    void _drop_index(string filePath, int type);
    // find index
    int _find_by_index(string filePath, Data data);
    // insert index
    void _insert_by_index(string filePath, Data data, int blockID);
    // remove index key
    void _remove_by_index(string filePath, Data data);
    // search key in range
    void _search_key_range(string filePath, Data data1, Data data2, vector<int> &values);
    // get key size
    int _get_key_size(int type);
    // get suitable order
    int _get_order(int type);

private:
    map_int intIndex;
    map_string stringIndex;
    map_float floatIndex;
};

#endif
