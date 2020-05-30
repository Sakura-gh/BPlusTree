/////////////////////////////////////////////////////////////////////////
// basis.h
// created by gehao on 2020/5/29
/////////////////////////////////////////////////////////////////////////

#ifndef _BASIS_H_
#define _BASIS_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 每一页的大小为4KB
#define PAGESIZE 4096
// 最大页数为100
#define MAXFRAMESIZE 100

enum
{
    INT = -1,
    FLOAT = 0
};

//一个struct存放它的一条信息的类型和值
//用一个strunt嵌套一个union实现多种类型的转换
//type的类型：-1：int,0:float,1-255:string(数值为字符串的长度+1),注意使用时对Value的选择！
struct Data
{
    int type;
    int datai;
    float dataf;
    string datas;
};

//在确定类型时，慎用str.size()+1来决定str的type的值，一张表最多32个attribute
struct Attribute
{
    int num;            //存放table的属性数
    string name[32];    //存放每个属性的名字
    short type[32];     //存放每个属性的类型，-1：int,0:float,1~255:string的长度+1
    bool unique[32];    //判断每个属性是否unique，是为true
    short primary_key;  //判断是否存在主键,-1为不存在，其他则为主键的所在位置
    bool has_index[32]; //判断是否存在索引
};

// 用于将含有特殊符号的字符串分隔成字符串vector
// input: str, symbol; output: vector new_str[]
vector<string> split(const string &s, const string &seperator);

#endif
