#include <vector>
#include "Quaternary.h"
using std::vector;


#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H


typedef class class_actinfo{  //��¼������Ծ�����Ļ�Ծλ��
public:
	item act_node;
	vector<int> act_location;
}ACTINFO;

typedef ACTINFO* actinfo;


typedef class class_confi_node{  //��ͻͼԪ��
public:
	item this_node;
	vector<item> confis;
}CONFI_NODE;

typedef CONFI_NODE *confi_node;

typedef class class_block{  //������
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
	vector<block> myblocks;  //�Ժ���Ϊ��λ�洢������
	vector<actinfo> myactinfo;  //��Ծ�ڵ���Ϣ
	vector<confi_node> myconfi_info;  //��ͻͼ
	vector<int> myorder; //���߽ڵ��˳��
}FBLOCK;

typedef FBLOCK* fblock;


extern vector<fblock> fblocks; //�洢���еĻ�����



void getBlocks();
void active_value();
void printBlocks();
int find(vector<item> &allIt, item &it);

#endif