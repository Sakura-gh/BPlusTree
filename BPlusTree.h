/////////////////////////////////////////////////////////////////////////
// BPlusTree.h
// template class
// created by gehao on 2020/5/23
/////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// decalare
template <typename T>
class BPlusTree;

/////////////////////////////////TreeNode///////////////////////////////
template <typename T>
class TreeNode
{
public:
    // constructor
    // input: order, isLeaf(default=false)
    TreeNode(int order, bool isLeaf = false);
    // destructor
    ~TreeNode();
    // is root or not
    bool isRoot();
    // is full or not
    bool isFull();
    // find key, return index
    int _find(const T &key);
    // insert key to inner node, return index
    // _pointer is left pointer for first key, pointer is right pointer for all keys in inner node
    int _insert(const T &key, TreeNode<T> *pointer, TreeNode<T> *_pointer = NULL);
    // insert key to leaf node
    int _insert(const T &key, const int &address);
    // remove key by index
    bool _remove(const int &index);
    // split the node into two nodes, return the new node's address
    TreeNode *_split(T &key);
    // next node
    TreeNode *nextNode();
    // update minKeyNum
    void updateMinKeyNum();

private:
    // order of tree
    int order;
    // minimum key number of leaf node or inner node
    int minKeyNum;
    // if leaf node
    bool isLeaf;
    // pointer to parent
    TreeNode *parent;
    // pointer to next node
    TreeNode *next;
    // key vector
    vector<T> keys;
    // address, store in leaf node
    vector<int> address;
    // pointer to children, store in inner node
    vector<TreeNode *> pointers;

    //friend class
    friend class BPlusTree<T>;
};

template <typename T>
inline TreeNode<T>::TreeNode(int order, bool isLeaf) : order(order), isLeaf(isLeaf)
{
    this->parent = NULL;
    this->next = NULL;
    updateMinKeyNum();
}

template <typename T>
inline TreeNode<T>::~TreeNode() {}

// update minimun key number of node when node's isLeaf changed
template <typename T>
inline void TreeNode<T>::updateMinKeyNum()
{
    if (this->isLeaf) // leaf node
        this->minKeyNum = this->order / 2;
    else // inner node
    {
        if (this->order % 2 == 1) // order is odd
            this->minKeyNum = this->order / 2;
        else // order is even
            this->minKeyNum = this->order / 2 - 1;
    }
}

template <typename T>
inline bool TreeNode<T>::isRoot()
{
    if (this->parent == NULL)
        return true;
    else
        return false;
}

template <typename T>
inline bool TreeNode<T>::isFull()
{
    // keys number <= order-1
    if (this->keys.size() > this->order - 1)
        return true;
    else
        return false;
}

// input: key
// output: the index of key; if key not exsits, return -1
template <typename T>
int TreeNode<T>::_find(const T &key)
{
    // if (keys.size() == 0)
    //     return -1;
    // if (keys.front() > key)
    //     return -1;
    // if (keys.back() < key)
    //     return -1;

    auto iter = find(this->keys.begin(), this->keys.end(), key);
    if (iter == this->keys.end())
        return -1;
    else
        return distance(this->keys.begin(), iter);
}

// inner node insert
// insert key, return index, if fail, return -1
template <typename T>
int TreeNode<T>::_insert(const T &key, TreeNode<T> *pointer, TreeNode<T> *_pointer)
{
    TreeNode<T> *pLeftNode = _pointer;
    TreeNode<T> *pRightNode = pointer;
    // first key in inner node, for initial case
    if (this->keys.size() == 0)
    {
        this->keys.push_back(key);
        this->pointers.push_back(NULL);
        this->pointers[this->pointers.size() - 1] = pLeftNode;
        this->pointers.push_back(NULL);
        this->pointers[this->pointers.size() - 1] = pRightNode;
        return 0;
    }
    // if key not exists, insert
    else if (_find(key) == -1)
    {
        // insert into front
        if (key < this->keys.front())
        {
            this->keys.insert(this->keys.begin(), key);
            this->pointers.insert(this->pointers.begin() + 1, NULL);
            this->pointers[1] = pRightNode;
            return 0;
        }
        else if (key > this->keys.back())
        {
            this->keys.push_back(key);
            this->pointers.push_back(NULL);
            this->pointers[this->pointers.size() - 1] = pRightNode;
            return this->keys.size() - 1;
        }
        else
        {
            // find the first element bigger than key, insert key at this position
            auto index = upper_bound(this->keys.begin(), this->keys.end(), key);
            this->keys.insert(index, key);

            // insert pointer
            this->pointers.insert(this->pointers.begin() + (index - this->keys.begin()) + 1, NULL);
            this->pointers[index - this->keys.begin() + 1] = pRightNode;
            return index - this->keys.begin();
        }
    }

    return -1;
}

// leaf node insert
// insert key and address of record, return index, if not leaf, return -1
template <typename T>
int TreeNode<T>::_insert(const T &key, const int &address)
{
    if (!isLeaf)
        return -1;
    if (this->keys.size() == 0)
    {
        this->keys.push_back(key);
        this->address.push_back(address);
        return 0;
    }
    else if (_find(key) == -1)
    {
        // insert key
        auto iter = upper_bound(this->keys.begin(), this->keys.end(), key);
        int offset = iter - this->keys.begin();
        this->keys.insert(iter, key);
        // insert address
        this->address.insert(this->address.begin() + offset, address);
        return offset;
    }
    return -1;
}

template <typename T>
bool TreeNode<T>::_remove(const int &index)
{
    if (index < 0 || index >= this->keys.size())
        return false;
    if (isLeaf)
    {
        this->keys.erase(this->keys.begin() + index);
        this->address.erase(this->address.begin() + index);
        return true;
    }
    else
    {
        this->keys.erase(this->keys.begin() + index);
        this->pointers.erase(this->pointers.begin() + index + 1);
        // // if there only one key to be remove, then delete two pointers
        // if (this->pointers.size() == 1)
        //     this->pointers.clear();
        return true;
    }
    return false;
}

// split the node, return the pointer of new node
// reference key will be changed into the first key of new node
template <typename T>
TreeNode<T> *TreeNode<T>::_split(T &key)
{
    // minimun key number of node
    int minKeyNum;
    if (this->isLeaf) // leaf node
        minKeyNum = this->order / 2;
    else // inner node
    {
        if (this->order % 2 == 1) // order is odd
            minKeyNum = this->order / 2;
        else // order is even
            minKeyNum = this->order / 2 - 1;
    }

    // create new node
    TreeNode *newNode = new TreeNode(this->order, this->isLeaf);

    if (isLeaf) // leaf node
    {
        // for leaf node:{key:address}
        key = this->keys[minKeyNum];
        // iterator of first element in new node
        auto iter1 = this->keys.begin() + minKeyNum;
        // copy keys[minKeyNum, order-1] of this node to new node keys
        newNode->keys.assign(iter1, this->keys.end());
        // erase keys[minKeyNum, order-1] from this node
        this->keys.erase(iter1, this->keys.end());

        // address for leaf nodes
        auto iter2 = this->address.begin() + minKeyNum;
        newNode->address.assign(iter2, this->address.end());
        this->address.erase(iter2, this->address.end());

        // insert the new node into this node's next
        newNode->next = this->next;
        // this node's next will be newNode
        this->next = newNode;
        // new node's parent is this node's parent
        newNode->parent = this->parent;
    }
    else // inner node
    {
        // 0-minKeyNum-1 keys和0-minKeyNum pointers给第一个node
        // 相当于给第一个key一前一后两个pointer，其余key都只给跟在它后面的那个pointer(这个pointer指向的node的第一个元素的值与该key值相等)
        key = this->keys[minKeyNum];
        // keys split
        auto iter_key = this->keys.begin() + minKeyNum;
        newNode->keys.assign(iter_key, this->keys.end());
        this->keys.erase(iter_key, this->keys.end());
        // pointers split
        // the first key in new node will be upper level later
        auto iter_p = this->pointers.begin() + minKeyNum + 1;
        newNode->pointers.assign(iter_p, this->pointers.end());
        // must update newNode's child's parent!
        for (int i = 0; i < newNode->pointers.size(); i++)
            newNode->pointers[i]->parent = newNode;
        this->pointers.erase(iter_p, this->pointers.end());

        newNode->next = this->next;
        this->next = newNode;
        newNode->parent = this->parent;
    }

    return newNode;
}

// next node
template <typename T>
TreeNode<T> *TreeNode<T>::nextNode()
{
    return next;
}

////////////////////////////////BPlusTree////////////////////////

template <typename T>
class BPlusTree
{
public:
    // constructor
    BPlusTree(string fileName, int order, int keySize);
    // destructor
    ~BPlusTree();
    // find the address of record with key
    int _find_BT(const T &key);
    // insert address with key into bplustree
    bool _insert_BT(const T &key, const int &address);
    // remove key from bplustree
    bool _remove_BT(const T &key);
    // drop all the tree
    void _drop_BT(TreeNode<T> *pNode);
    // read data from disk
    void _read_disk();
    // write data to disk
    void _write_disk();
    // print leaf nodes
    void _print_leaf();
    // print every level nodes
    void _print_every_level();
    // detial operation
    TreeNode<T> *_find_detial(TreeNode<T> *pNode, const T &key, int &value);
    void _insert_detail(const T &key, TreeNode<T> *pNodeLeft, TreeNode<T> *pNodeRight, TreeNode<T> *&obj);
    bool _remove_detial(TreeNode<T> *&pNode);

private:
    // file to read/write
    string fileName;
    // pointer to root of the tree
    TreeNode<T> *root;
    // pointer to leaf level of the tree
    TreeNode<T> *leaf;
    // order of BPlusTree
    int order;
    // size of key
    int keySize;
};

template <typename T>
BPlusTree<T>::BPlusTree(string fileName, int order, int keySize) : fileName(fileName), order(order), keySize(keySize)
{
    this->root = new TreeNode<T>(order, true);
    this->leaf = this->root;
    // _read_disk();
}

template <typename T>
void BPlusTree<T>::_read_disk()
{
    // string fname = './source/index/' + this->fileName;
}

template <typename T>
BPlusTree<T>::~BPlusTree()
{
    _drop_BT(this->root);
}

template <typename T>
int BPlusTree<T>::_find_BT(const T &key)
{
    int value;
    _find_detial(this->root, key, value);

    return value;
}

// return pNode : is the leaf node where key in or will be in
template <typename T>
TreeNode<T> *BPlusTree<T>::_find_detial(TreeNode<T> *pNode, const T &key, int &value)
{
    int index = pNode->_find(key);
    // if find key
    if (index != -1)
    {
        // if pNode is leaf node, then find it
        if (pNode->isLeaf)
        {
            value = pNode->address[index];
            return pNode;
        }
        // if pNode is not leaf node, then find the leaf node
        else
        {
            // right subtree of pNode
            pNode = pNode->pointers[index + 1];
            // find the least key in right subtree
            while (!pNode->isLeaf)
                pNode = pNode->pointers[0];
            // get the leaf node
            value = pNode->address[0];
            return pNode;
        }
    }
    // if not find key in this node
    else
    {
        // if in leaf and not find key, return value=-1
        if (pNode->isLeaf)
        {
            value = -1;
            return pNode;
        }
        // recursively find next level
        else
        {
            if (key < pNode->keys.front())
                return _find_detial(pNode->pointers.front(), key, value);
            else if (key > pNode->keys.back())
                return _find_detial(pNode->pointers.back(), key, value);
            else
            {
                auto iter = upper_bound(pNode->keys.begin(), pNode->keys.end(), key);
                return _find_detial(pNode->pointers[iter - pNode->keys.begin()], key, value);
            }
        }
    }
}

template <typename T>
bool BPlusTree<T>::_insert_BT(const T &key, const int &address)
{
    int value;
    TreeNode<T> *pNode = _find_detial(this->root, key, value);
    // if key exists, return false
    if (value != -1)
        return false;
    // if key not exists, then insert into leaf node where pNode point to
    else
    {
        // leaf node insert
        pNode->_insert(key, address);
        // if node is full, split and get new node's first key newKey and pointer pNewNode
        if (pNode->isFull())
        {
            T newKey;
            TreeNode<T> *pNewNode = pNode->_split(newKey);
            _insert_detail(newKey, pNode, pNewNode, pNode->parent);
        }
        return true;
    }
}

template <typename T>
void BPlusTree<T>::_insert_detail(const T &key, TreeNode<T> *pNodeLeft, TreeNode<T> *pNodeRight, TreeNode<T> *&obj)
{
    // if leaf node
    if (pNodeLeft->isLeaf)
    {
        // if obj is null
        if (obj == NULL)
        {
            obj = new TreeNode<T>(this->order, false);
            // update right's parent
            pNodeRight->parent = obj;
            // insert left and right to parent
            obj->_insert(key, pNodeRight, pNodeLeft);
            this->root = obj;
            return;
        }
        // if obj is not null
        else
        {
            obj->_insert(key, pNodeRight, NULL);
            if (obj->isFull())
            {
                T newKey;
                // use pOldNode store origin obj
                // 很奇怪的现象，obj->_split(newKey)之后，obj本身也发生了变化，变成了pNewNode，但_split函数并不会对obj本身做修改，而调试的时候在_split函数内部obj地址也没变，一旦出了这个函数，obj地址立马就变了，很疑惑...暂时找不到原因，因此用pOldNode保存obj原先的值来暂时处理
                TreeNode<T> *pOldNode = obj;
                TreeNode<T> *pNewNode = obj->_split(newKey);
                // for inner node, first key in new node will be upper insert
                pNewNode->keys.erase(pNewNode->keys.begin());
                _insert_detail(newKey, pOldNode, pNewNode, pOldNode->parent);
            }
            return;
        }
    }
    // not leaf node, the same as leaf node
    else
    {
        // if obj=null, then create new root node
        if (obj == NULL)
        {
            obj = new TreeNode<T>(this->order, false);
            // update right's parent
            pNodeRight->parent = obj;
            obj->_insert(key, pNodeRight, pNodeLeft);
            this->root = obj;
            return;
        }
        // obj is not null
        else
        {
            obj->_insert(key, pNodeRight, NULL);
            if (obj->isFull())
            {
                T newKey;
                //使用理由同上
                TreeNode<T> *pOldNode = obj;
                TreeNode<T> *pNewNode = obj->_split(newKey);
                // for inner node, first key in new node will be upper insert
                pNewNode->keys.erase(pNewNode->keys.begin());
                _insert_detail(newKey, pOldNode, pNewNode, pOldNode->parent);
            }
            return;
        }
    }
}

template <typename T>
bool BPlusTree<T>::_remove_BT(const T &key)
{
    int value;
    TreeNode<T> *pNode = _find_detial(this->root, key, value);
    // if key not in tree
    if (value == -1)
        return false;

    // if key in tree, get the key index in leaf node
    int leaf_index = pNode->_find(key);

    // if leaf node is root
    if (pNode->isRoot())
    {
        pNode->_remove(leaf_index);
        return _remove_detial(pNode);
    }
    // need to update inner node
    else
    {
        // first key of node, but not first node in leaf
        if (leaf_index == 0 && this->leaf != pNode)
        {
            TreeNode<T> *tmp_parent = pNode->parent;
            int tmp_index = tmp_parent->_find(key);
            // find key location in inner node
            while (tmp_index == -1)
            {
                if (tmp_parent->parent)
                    tmp_parent = tmp_parent->parent;
                else
                    break;
                tmp_index = tmp_parent->_find(key);
            }
            // update inner node which has key
            tmp_parent->keys[tmp_index] = pNode->keys[1];
            // delete key from leaf node
            pNode->_remove(leaf_index);
            return _remove_detial(pNode);
        }
        // not need to update inner node
        else
        {
            pNode->_remove(leaf_index);
            return _remove_detial(pNode);
        }
    }

    return false;
}

template <typename T>
bool BPlusTree<T>::_remove_detial(TreeNode<T> *&pNode)
{
    if (pNode->keys.size() >= pNode->minKeyNum)
        return true;

    // if pNode is root
    if (pNode->isRoot())
    {
        // if keys num>0, no update
        if (pNode->keys.size() > 0)
            return true;
        // if root's key num<=0
        else
        {
            // if tree will be null
            if (this->root->isLeaf)
            {
                delete pNode;
                this->root = NULL;
                this->leaf = NULL;
            }
            // root will be first left child
            else
            {
                this->root = pNode->pointers[0];
                this->root->parent = NULL;
                delete pNode;
            }
        }
    }
    // if pNode is not root
    else
    {
        TreeNode<T> *parent = pNode->parent;
        TreeNode<T> *brother = NULL;
        if (pNode->isLeaf)
        {
            int index = parent->_find(pNode->keys[0]);
            //choose the left brother to merge or replace
            if (parent->pointers[0] != pNode && index + 1 == parent->keys.size())
            {
                // get left brother of pNode
                brother = parent->pointers[index];
                // borrow last key from left brother
                if (brother->keys.size() > brother->minKeyNum)
                {
                    pNode->_insert(brother->keys.back(), brother->address.back());
                    brother->_remove(brother->keys.size() - 1);
                    // update parent's key
                    parent->keys[index] = pNode->keys[0];
                    return true;
                }
                // merge the node with its brother
                else
                {
                    parent->_remove(index);
                    for (int i = 0; i < pNode->keys.size(); i++)
                        brother->_insert(pNode->keys[i], pNode->address[i]);
                    brother->next = pNode->next;
                    delete pNode;

                    return _remove_detial(parent);
                }
            }
            // borrow or merge with the right brother
            else
            {
                if (parent->pointers[0] == pNode)
                    brother = parent->pointers[1];
                else
                    brother = parent->pointers[index + 2];
                // borrow the most left key of brother to add to the right hand of the node
                if (brother->keys.size() > brother->minKeyNum)
                {
                    pNode->_insert(brother->keys.front(), brother->address.front());
                    brother->_remove(0);

                    // update parent's key
                    if (parent->pointers[0] == pNode)
                        parent->keys[0] = brother->keys[0];
                    else
                        parent->keys[index + 1] = brother->keys[0];
                    return true;
                }
                // merge the node with brother
                else
                {
                    for (int i = 0; i < brother->keys.size(); i++)
                        pNode->_insert(brother->keys[i], brother->address[i]);
                    if (pNode == parent->pointers[0])
                        parent->_remove(0);
                    else
                        parent->_remove(index + 1);
                    pNode->next = brother->next;
                    delete brother;

                    return _remove_detial(parent);
                }
            }
        }
        // inner node
        else
        {
            // int index = parent->_find(pNode->pointers[0]->keys[0]);
            auto iter = upper_bound(parent->keys.begin(), parent->keys.end(), pNode->keys.back());
            int index = iter - parent->keys.begin();
            // last pointer of parent
            if (pNode->keys[0] > parent->keys.back())
                index = parent->keys.size() - 1;

            if (parent->pointers[0] != pNode && index + 1 == parent->keys.size() && pNode->keys[0] > parent->keys.back())
            {
                brother = parent->pointers[index];
                // borrow the most right key and child to add to the left hand of the pnode
                if (brother->keys.size() > brother->minKeyNum)
                {
                    pNode->keys.insert(pNode->keys.begin(), parent->keys[index]);
                    pNode->pointers.insert(pNode->pointers.begin(), brother->pointers.back());
                    parent->keys[index] = brother->keys.back();
                    // update brother and child
                    if (brother->pointers.back())
                        brother->pointers.back()->parent = pNode;
                    brother->_remove(brother->keys.size() - 1);

                    return true;
                }
                // merge the node to its left brother
                else
                {
                    brother->keys.push_back(parent->keys[index]);
                    parent->_remove(index);
                    for (int i = 0; i < pNode->keys.size(); i++)
                    {
                        brother->pointers.push_back(pNode->pointers[i]);
                        pNode->pointers[i]->parent = brother;
                        brother->keys.push_back(pNode->keys[i]);
                    }
                    brother->pointers.push_back(pNode->pointers.back());
                    brother->pointers.back()->parent = brother;

                    brother->next = pNode->next;
                    delete pNode;
                    return _remove_detial(parent);
                }
            }
            // borrow or merge with right brother
            else
            {
                // find the right brother
                brother = parent->pointers[index + 1];

                // borrow left key of right brother to pNode
                if (brother->keys.size() > brother->minKeyNum)
                {
                    pNode->keys.push_back(parent->keys[index]);

                    pNode->pointers.push_back(brother->pointers[0]);
                    pNode->pointers.back()->parent = pNode;

                    parent->keys[index] = brother->keys[0];

                    brother->pointers[0] = brother->pointers[1];
                    brother->_remove(0);

                    return true;
                }
                // merge with right brother
                else
                {
                    pNode->keys.push_back(parent->keys[index]);
                    parent->_remove(index);
                    for (int i = 0; i < brother->keys.size(); i++)
                    {
                        pNode->pointers.push_back(brother->pointers[i]);
                        brother->pointers[i]->parent = pNode;
                        pNode->keys.push_back(brother->keys[i]);
                    }
                    pNode->pointers.push_back(brother->pointers.back());
                    brother->pointers.back()->parent = pNode;

                    // don't forget to update next
                    pNode->next = brother->next;
                    delete brother;

                    return _remove_detial(parent);
                }
            }
        }
    }

    return false;
}

template <typename T>
void BPlusTree<T>::_drop_BT(TreeNode<T> *pNode)
{
    // if (pNode == NULL)
    //     return;
    // if (!pNode->isLeaf)
    // {
    //     for (int i = 0; i < pNode->pointers.size(); i++)
    //     {
    //         _drop_BT(pNode->pointers[i]);
    //         pNode->pointers[i] = NULL;
    //     }
    //     delete pNode;
    // }
    // else
    // {
    //     delete pNode;
    // }
    return;
}

template <typename T>
void BPlusTree<T>::_print_leaf()
{
    TreeNode<T> *pLeaf = this->leaf;
    while (pLeaf)
    {
        for (auto iter = pLeaf->keys.begin(); iter != pLeaf->keys.end(); iter++)
            cout << *iter << " ";
        cout << endl;
        pLeaf = pLeaf->next;
    }
}

template <typename T>
void BPlusTree<T>::_print_every_level()
{
    TreeNode<T> *pNextLevel = this->root->pointers[0];
    TreeNode<T> *nowLevel = this->root;
    do
    {
        while (nowLevel)
        {
            for (int i = 0; i < nowLevel->keys.size(); i++)
                cout << nowLevel->keys[i] << " ";
            cout << "  ";
            nowLevel = nowLevel->next;
        }
        cout << endl;
        nowLevel = pNextLevel;
        if (!pNextLevel->isLeaf)
            pNextLevel = pNextLevel->pointers[0];
    } while (!nowLevel->isLeaf);
    // for leaf
    while (nowLevel)
    {
        for (int i = 0; i < nowLevel->keys.size(); i++)
            cout << nowLevel->keys[i] << " ";
        cout << " ";
        nowLevel = nowLevel->next;
    }
    cout << endl;
}