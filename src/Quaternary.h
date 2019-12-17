#include "symbolTable.h"
#include <vector>
using std::vector;


#ifndef _QUATERNARY_H
#define _QUATERNARY_H


typedef enum _opcodeenum{    //要求已经无语义错误
  ADD,  //op1, op2,  obj加法, use, def
  SUB,  //op1, op2, obj 减法, use, def
  MUL,  //op1, op2, obj 乘法, use, def
  DIV,  //op1, op2, ogj 除法, use, def
  NEG, //op1, -, obj 取反, use, def
  ASSIGN, //op1, -, obj 赋值, use, def

  GTR, //op1, op2, obj 逻辑运算，大于(为真时obj赋值为1) use
  LES,  //op1, op2, obj 小于 , use
  EQL,   //op1, op2, obj 等于, use
  GEQ,   //op1, op2, obj 大于等于, use
  LEQ,   //op1, op2, obj 小于等于, use
  NEQ,    //op1, op2, obj 不等于, use
  NEQZ,   //op1, -, obj  op1不等于0则设obj为1, use

  TJMP, //op1,-,obj 为真跳转, use
  FJMP, //op1, -, obj 为假跳转, use
  JMP, //-, -, obj 无条件跳转, 
  TAG, //-， -， obj 标号, 

  HEADER, //-, -, obj 函数头部
  FORPARA, //-, NULL, obj 函数形参声明   
  ACTPARA, //-, -, obj 函数调用的实参声明  use_ 
  RCALL , //op1, - , obj 有返回值函数调用,op1为函数名， obj为调用结果
  CALL, //op1, -, -无返回值函数调用， op1为函数名
  NRETN, // -, -, -无返回值返回
  RETN, //-, -, obj有返回值返回
  FOOT, //-, -, - 函数尾部

  READ, //-, -, obj 读语句，读入一个值到obj, def
  WRITE, //-，-，obj 写语句，输出obj, use

  
  RARRY,  //op1, op2, obj 读数组操作， op1为数组，op2为偏移，obj为临时变量, use
  WARRY  //op1,op2, obj 写数组操作， op1为数组， op2为偏移， obj为要写的值, use_ 


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

//函数定义

void insertQuater(opcode op, item op1, item op2, item obj);
item tagGen();
void insertStrData(item ss);
string printOpCode(opcode opd);
void printQuaternary();
void printAquayer(quater qt); //打印单个四元式



extern vector <quater> quaterList;
extern vector <item> strdata;


#endif // _QUATERNARY_H


