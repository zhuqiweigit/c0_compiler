#include <vector>
#include "Quaternary.h"
using std::vector;


#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H


typedef class class_actinfo{  //纪录单个活跃变量的活跃位置
public:
	item act_node;
	vector<int> act_location;
}ACTINFO;

typedef ACTINFO* actinfo;


typedef class class_confi_node{  //冲突图元素
public:
	item this_node;
	vector<item> confis;
}CONFI_NODE;

typedef CONFI_NODE *confi_node;

typedef class class_block{  //基本块
public:
	int id;
	vector<class_block*> pre;
	vector<class_block*> next;
	vector <quater> block_quter;
	vector <item> use;
	vector <item> def;
	vector <item> in;
	vector <item> out;

}BLOCK;

typedef BLOCK* block;

typedef class class_record{
public:
	vector <item> in;
	vector <item> out;
}RECORD;
typedef RECORD* record;

typedef class class_fblock{  
public:
	vector<block> myblocks;  //以函数为单位存储基本块
	vector<actinfo> myactinfo;  //活跃节点信息
	vector<confi_node> myconfi_info;  //冲突图
	vector<int> myorder; //移走节点的顺序
}FBLOCK;

typedef FBLOCK* fblock;


extern vector<fblock> fblocks; //存储所有的基本块



void getBlocks();
void active_value();
void printBlocks();
int find(vector<item> &allIt, item &it);

#endif