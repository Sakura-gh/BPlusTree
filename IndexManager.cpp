///////////////////////////////////////////////////////////////
// IndexManager.cpp
// created by gehao on 2020/5/29
///////////////////////////////////////////////////////////////

#include "IndexManager.h"
#include "basis.h"
#include "BPlusTree.h"

// just a test CatalogManager class
class CatalogManager
{
public:
    CatalogManager(){};
    ~CatalogManager(){};
    Attribute getAttribute(string)
    {
        Attribute test;
        test.num = 0;
    };
};

// for each table, there will be indexes on different attributes
IndexManager::IndexManager(string tableName)
{
    // read attributes and index on it from CatalogManager
    CatalogManager catalog;
    Attribute attrs = catalog.getAttribute(tableName);

    // for each attribute which has index, create index
    for (int i = 0; i < attrs.num; i++)
    {
        if (attrs.has_index[i])
            _create_index("INDEX_" + tableName + "_" + attrs.name[i], attrs.type[i]);
    }
}

// write back blocks
IndexManager::~IndexManager()
{
    for (auto iter_int = intIndex.begin(); iter_int != intIndex.end(); iter_int++)
    {
        if (iter_int->second)
        {
            iter_int->second->_write_disk();
            delete iter_int->second;
        }
    }

    for (auto iter_str = stringIndex.begin(); iter_str != stringIndex.end(); iter_str++)
    {
        if (iter_str->second)
        {
            iter_str->second->_write_disk();
            delete iter_str->second;
        }
    }

    for (auto iter_float = floatIndex.begin(); iter_float != floatIndex.end(); iter_float++)
    {
        if (iter_float->second)
        {
            iter_float->second->_write_disk();
            delete iter_float->second;
        }
    }
}

// create index and bplustree, insert to index map
void IndexManager::_create_index(string filePath, int type)
{
    int keySize = _get_key_size(type);
    int order = _get_order(type);

    if (type == INT)
    {
        BPlusTree<int> *bplustree = new BPlusTree<int>(filePath, type, order);
        intIndex.insert(map_int::value_type(filePath, bplustree));
    }
    else if (type == FLOAT)
    {
        BPlusTree<float> *bplustree = new BPlusTree<float>(filePath, type, order);
        floatIndex.insert(map_float::value_type(filePath, bplustree));
    }
    else
    {
        BPlusTree<string> *bplustree = new BPlusTree<string>(filePath, type, order);
        stringIndex.insert(map_string::value_type(filePath, bplustree));
    }
}

// drop index from index map and delete bplustree
void IndexManager::_drop_index(string filePath, int type)
{
    if (type == INT)
    {
        auto iter_int = intIndex.find(filePath);
        if (iter_int == intIndex.end())
        {
            cout << "Error:in drop index, no index " << filePath << " exits" << endl;
            return;
        }
        else
        {
            delete iter_int->second;
            intIndex.erase(iter_int);
        }
    }
    else if (type == FLOAT)
    {
        auto iter_float = floatIndex.find(filePath);
        if (iter_float == floatIndex.end())
        {
            cout << "Error:in drop index, no index " << filePath << " exits" << endl;
            return;
        }
        else
        {
            delete iter_float->second;
            floatIndex.erase(iter_float);
        }
    }
    else
    {
        auto iter_str = stringIndex.find(filePath);
        if (iter_str == stringIndex.end())
        {
            cout << "Error:in drop index, no index " << filePath << " exits" << endl;
            return;
        }
        else
        {
            delete iter_str->second;
            stringIndex.erase(iter_str);
        }
    }
    return;
}

// if index exists, find the address of key by this index
int IndexManager::_find_by_index(string filePath, Data data)
{
    if (data.type == INT)
    {
        auto iter_int = intIndex.find(filePath);
        if (iter_int == intIndex.end())
        {
            cout << "Error:in search index, no index " << filePath << " exits" << endl;
            return -1;
        }
        else
            return iter_int->second->_find_BT(data.datai);
    }
    else if (data.type == FLOAT)
    {
        auto iter_float = floatIndex.find(filePath);
        if (iter_float == floatIndex.end())
        {
            cout << "Error:in search index, no index " << filePath << " exits" << endl;
            return -1;
        }
        else
            return iter_float->second->_find_BT(data.dataf);
    }
    else
    {
        auto iter_str = stringIndex.find(filePath);
        if (iter_str == stringIndex.end())
        {
            cout << "Error:in search index, no index " << filePath << " exits" << endl;
            return -1;
        }
        else
            return iter_str->second->_find_BT(data.datas);
    }
}

// if index on filePath exists, insert {key:address} by this index
void IndexManager::_insert_by_index(string filePath, Data data, int blockID)
{
    if (data.type == INT)
    {
        auto iter_int = intIndex.find(filePath);
        if (iter_int == intIndex.end())
        {
            cout << "Error:in insert index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_int->second->_insert_BT(data.datai, blockID);
    }
    else if (data.type == FLOAT)
    {
        auto iter_float = floatIndex.find(filePath);
        if (iter_float == floatIndex.end())
        {
            cout << "Error:in insert index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_float->second->_insert_BT(data.dataf, blockID);
    }
    else
    {
        auto iter_str = stringIndex.find(filePath);
        if (iter_str == stringIndex.end())
        {
            cout << "Error:in insert index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_str->second->_insert_BT(data.datas, blockID);
    }
}

// if index on filePath exists, remove {key:address} by this index
void IndexManager::_remove_by_index(string filePath, Data data)
{
    if (data.type == INT)
    {
        auto iter_int = intIndex.find(filePath);
        if (iter_int == intIndex.end())
        {
            cout << "Error:in remove index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_int->second->_remove_BT(data.datai);
    }
    else if (data.type == FLOAT)
    {
        auto iter_float = floatIndex.find(filePath);
        if (iter_float == floatIndex.end())
        {
            cout << "Error:in remove index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_float->second->_remove_BT(data.dataf);
    }
    else
    {
        auto iter_str = stringIndex.find(filePath);
        if (iter_str == stringIndex.end())
        {
            cout << "Error:in remove index, no index " << filePath << " exits" << endl;
            return;
        }
        else
            iter_str->second->_remove_BT(data.datas);
    }
}

int IndexManager::_get_order(int type)
{
    return (PAGESIZE - sizeof(int)) / (_get_key_size(type) + sizeof(int));
}

// type=-1: int; type=0: float; type=1:255: char[]/string
int IndexManager::_get_key_size(int type)
{
    if (type == INT)
        return sizeof(int);
    else if (type == FLOAT)
        return sizeof(float);
    else if (type > 0 && type <= 255)
        return type;
    else
        cout << "ERROR: in get key size: invalid type" << endl;
}
