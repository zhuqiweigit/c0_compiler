#include "symbolTable.h"
#include <vector>
using std::vector;


#ifndef _QUATERNARY_H
#define _QUATERNARY_H


typedef enum _opcodeenum{    //Ҫ���Ѿ����������
  ADD,  //op1, op2,  obj�ӷ�, use, def
  SUB,  //op1, op2, obj ����, use, def
  MUL,  //op1, op2, obj �˷�, use, def
  DIV,  //op1, op2, ogj ����, use, def
  NEG, //op1, -, obj ȡ��, use, def
  ASSIGN, //op1, -, obj ��ֵ, use, def

  GTR, //op1, op2, obj �߼����㣬����(Ϊ��ʱobj��ֵΪ1) use
  LES,  //op1, op2, obj С�� , use
  EQL,   //op1, op2, obj ����, use
  GEQ,   //op1, op2, obj ���ڵ���, use
  LEQ,   //op1, op2, obj С�ڵ���, use
  NEQ,    //op1, op2, obj ������, use
  NEQZ,   //op1, -, obj  op1������0����objΪ1, use

  TJMP, //op1,-,obj Ϊ����ת, use
  FJMP, //op1, -, obj Ϊ����ת, use
  JMP, //-, -, obj ��������ת, 
  TAG, //-�� -�� obj ���, 

  HEADER, //-, -, obj ����ͷ��
  FORPARA, //-, NULL, obj �����β�����   
  ACTPARA, //-, -, obj �������õ�ʵ������  use_ 
  RCALL , //op1, - , obj �з���ֵ��������,op1Ϊ�������� objΪ���ý��
  CALL, //op1, -, -�޷���ֵ�������ã� op1Ϊ������
  NRETN, // -, -, -�޷���ֵ����
  RETN, //-, -, obj�з���ֵ����
  FOOT, //-, -, - ����β��

  READ, //-, -, obj ����䣬����һ��ֵ��obj, def
  WRITE, //-��-��obj д��䣬���obj, use

  
  RARRY,  //op1, op2, obj ����������� op1Ϊ���飬op2Ϊƫ�ƣ�objΪ��ʱ����, use
  WARRY  //op1,op2, obj д��������� op1Ϊ���飬 op2Ϊƫ�ƣ� objΪҪд��ֵ, use_ 


}opcode;

typedef class class_quater{
public:
    opcode opc;
    item op1;
    item op2;
    item obj;
	int isBlockB;
	int isBlockE;

} QUATER;

typedef QUATER* quater;

//��������

void insertQuater(opcode op, item op1, item op2, item obj);
item tagGen();
void insertStrData(item ss);
string printOpCode(opcode opd);
void printQuaternary();
void printAquayer(quater qt); //��ӡ������Ԫʽ



extern vector <quater> quaterList;
extern vector <item> strdata;


#endif // _QUATERNARY_H


