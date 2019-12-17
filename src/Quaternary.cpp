#include <stdlib.h>
#include <iostream>
#include <strstream>
#include "Quaternary.h"
#include "lexicon.h"
#include "parser.h"
#include "symbolTable.h"
using namespace std;
vector <quater> quaterList;  //存储所有生成的四元式
vector <item> strdata;

long tagNum = 0;
//目前仅在四元式里存储的有：tag、数组元素、实参、字符串字面量、数字字面量、char字面量


void insertQuater(opcode op, item op1, item op2, item obj){  //生成一个新的四元式，并插入四元式列表
   quater temp1 =new QUATER();
   temp1->opc = op;
   temp1->op1 = op1;
   temp1->op2 = op2;
   temp1->obj = obj;
   temp1->isBlockB = 0;
   temp1->isBlockE = 0;
   quaterList.push_back(temp1);

}

item tagGen(){  //tag生成器，每次调用都生成一个独一无二的tag
    strstream ss;
    string s, s2("tag");
    item tempi = new ITEM();
    itemInit(tempi);
    ss << tagNum;
	ss >> s;
    s = s2 + s;
    tagNum++;
    tempi->svalue = s;
    return tempi;
}

void insertStrData(item ss){   //存储字符串常量，便于data段统一分配内存
    strdata.push_back(ss);

}


void printQuaternary(){  //打印四元式
    unsigned int i;
	item op1, op2, obj;
	cout << "_________________四元式______________"<<endl;
    for(i = 0; i < quaterList.size(); i++){
		op1 = quaterList[i]->op1;
		op2 = quaterList[i]->op2;
		obj = quaterList[i]->obj;

        cout << "式子：" << i <<"____________________"<<endl;
		cout << printOpCode(quaterList[i]->opc) << "操作:"<<endl;
        cout << " ____op1 " << "item" << ":"<<endl;
		if(op1 != NULL){
        cout << "	name: " << op1->name << endl;
        cout << "	kind: " << toStringKind(op1->kind) <<endl;
        cout << "	type: " << toStringType(op1->type) <<endl;
		cout << "	level:" << op1->level <<endl;
        cout << "	isTemp:" << op1->isTemp <<endl;
        cout << "	addr:" << op1->addr <<endl;
        cout << "	paranum:" << op1->paranum <<endl;
        cout << "	returnV:" << toStringType(op1->returnV) <<endl;
        cout << "	length:" << op1->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< op1->valueInt<<endl;
        cout << "	valueChar:" << op1->valueChar<<endl;
        cout << "	svalue:" << op1->svalue<<endl;
		cout << "	funlen:" << op1->funlen <<endl;
		cout << "	parath:" << op1->parath <<endl;
		cout << "	isisOptimzed:" << op1->isOptimized<<endl;
		cout << "   reg:" << op1->reg<<endl;
		cout << "	isDelete: "<< op1->isDelete<<endl;
		}
		if(op2 != NULL){
		cout << " ____op2 " << "item" << ":"<<endl;
        cout << "	name: " << op2->name << endl;
        cout << "	kind: " << toStringKind(op2->kind) <<endl;
        cout << "	type: " << toStringType(op2->type) <<endl;
		cout << "	level:" << op2->level <<endl;
        cout << "	isTemp:" << op2->isTemp <<endl;
        cout << "	addr:" << op2->addr <<endl;
        cout << "	paranum:" << op2->paranum <<endl;
        cout << "	returnV:" << toStringType(op2->returnV) <<endl;
        cout << "	length:" << op2->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< op2->valueInt<<endl;
        cout << "	valueChar:" << op2->valueChar<<endl;
        cout << "	svalue:" << op2->svalue<<endl;
		cout << "	funlen:" << op2->funlen <<endl;
		cout << "	parath:" << op2->parath <<endl;

		cout << "	isisOptimized:" << op2->isOptimized<<endl;
		cout << "	reg:" << op2->reg<<endl;
		cout << "	isDelete: "<< op2->isDelete<<endl;
		}
		if(obj != NULL){
		cout << " ____obj " << "item" << ":"<<endl;
        cout << "	name: " << obj->name << endl;
        cout << "	kind: " << toStringKind(obj->kind) <<endl;
        cout << "	type: " << toStringType(obj->type) <<endl;
		cout << "	level:" << obj->level <<endl;
        cout << "	isTemp:" << obj->isTemp <<endl;
        cout << "	addr:" << obj->addr <<endl;
        cout << "	paranum:" << obj->paranum <<endl;
        cout << "	returnV:" << toStringType(obj->returnV) <<endl;
        cout << "	length:" << obj->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< obj->valueInt<<endl;
        cout << "	valueChar:" << obj->valueChar<<endl;
        cout << "	svalue:" << obj->svalue<<endl;
		cout << "	funlen:" << obj->funlen <<endl;
		cout << "	parath:" << obj->parath <<endl;
		
		cout << "	isisOptimized:" << obj->isOptimized<<endl;
		cout << "	reg:" << obj->reg<<endl;
		cout << "	isDelete: "<< obj->isDelete<<endl;
		}
    }

}

void printAquayer(quater qt){  //打印单个四元式
	item op1, op2, obj;
	
		op1 = qt->op1;
		op2 = qt->op2;
		obj = qt->obj;
		cout << printOpCode(qt->opc) << "操作:"<<endl;
        cout << " ____op1 " << "item" << ":"<<endl;
		if(op1 != NULL){
        cout << "	name: " << op1->name << endl;
        cout << "	kind: " << toStringKind(op1->kind) <<endl;
        cout << "	type: " << toStringType(op1->type) <<endl;
		cout << "	level:" << op1->level <<endl;
        cout << "	isTemp:" << op1->isTemp <<endl;
        cout << "	addr:" << op1->addr <<endl;
        cout << "	paranum:" << op1->paranum <<endl;
        cout << "	returnV:" << toStringType(op1->returnV) <<endl;
        cout << "	length:" << op1->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< op1->valueInt<<endl;
        cout << "	valueChar:" << op1->valueChar<<endl;
        cout << "	svalue:" << op1->svalue<<endl;
		cout << "	funlen:" << op1->funlen <<endl;
		cout << "	parath:" << op1->parath <<endl;
		
		cout << "	isisOptimzed:" << op1->isOptimized<<endl;
		cout << "	reg:" << op1->reg<<endl;
		cout << "	isDelete: "<< op1->isDelete<<endl;
		}
		if(op2 != NULL){
		cout << " ____op2 " << "item" << ":"<<endl;
        cout << "	name: " << op2->name << endl;
        cout << "	kind: " << toStringKind(op2->kind) <<endl;
        cout << "	type: " << toStringType(op2->type) <<endl;
		cout << "	level:" << op2->level <<endl;
        cout << "	isTemp:" << op2->isTemp <<endl;
        cout << "	addr:" << op2->addr <<endl;
        cout << "	paranum:" << op2->paranum <<endl;
        cout << "	returnV:" << toStringType(op2->returnV) <<endl;
        cout << "	length:" << op2->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< op2->valueInt<<endl;
        cout << "	valueChar:" << op2->valueChar<<endl;
        cout << "	svalue:" << op2->svalue<<endl;
		cout << "	funlen:" << op2->funlen <<endl;
		cout << "	parath:" << op2->parath <<endl;
		
		cout << "	isisOptimized:" << op2->isOptimized<<endl;
		cout << "	reg:" << op2->reg<<endl;
		cout << "	isDelete: "<< op2->isDelete<<endl;
		}
		if(obj != NULL){
		cout << " ____obj " << "item" << ":"<<endl;
        cout << "	name: " << obj->name << endl;
        cout << "	kind: " << toStringKind(obj->kind) <<endl;
        cout << "	type: " << toStringType(obj->type) <<endl;
		cout << "	level:" << obj->level <<endl;
        cout << "	isTemp:" << obj->isTemp <<endl;
        cout << "	addr:" << obj->addr <<endl;
        cout << "	paranum:" << obj->paranum <<endl;
        cout << "	returnV:" << toStringType(obj->returnV) <<endl;
        cout << "	length:" << obj->length << endl;
           // cout << " value:" << symTable[i]->itemTB[j]->value <<endl;
        cout << "	valueInt:"<< obj->valueInt<<endl;
        cout << "	valueChar:" << obj->valueChar<<endl;
        cout << "	svalue:" << obj->svalue<<endl;
		cout << "	funlen:" << obj->funlen <<endl;
		cout << "	parath:" << obj->parath <<endl;

		cout << "	isisOptimzed:" << obj->isOptimized<<endl;
		cout << "	reg:" << obj->reg<<endl;
		cout << "	isDelete: "<< obj->isDelete<<endl;
		}
    

}





string printOpCode(opcode opd){
	string opstr;
	switch(opd){
		case ADD: return opstr = "ADD"; break;
		case SUB: return opstr = "SUB"; break;
		case MUL: return opstr = "MUL"; break;
		case DIV: return opstr = "DIV"; break;
		case NEG: return opstr = "NEG"; break;
		case ASSIGN: return opstr = "ASSIGN"; break;
		case GTR: return opstr = "GTR"; break;
		case LES: return opstr = "LES"; break;
		case EQL: return opstr = "EQL"; break;
		case GEQ: return opstr = "GEQ"; break;
		case LEQ: return opstr = "LEQ"; break;
		case NEQ: return opstr = "NEQ"; break;
		case NEQZ: return opstr = "NEQZ"; break;
		case TJMP: return opstr = "TJMP"; break;
		case FJMP: return opstr = "FJMP"; break;
		case JMP: return opstr = "JMP"; break;
		case TAG: return opstr = "TAG"; break;
		case HEADER: return opstr = "HEADER"; break;
		case FORPARA: return opstr = "FORPARA"; break;
		case ACTPARA: return opstr = "ACTPARA"; break;
		case RCALL: return opstr = "RCALL"; break;
		case CALL: return opstr = "CALL"; break;
		case NRETN: return opstr = "NRETN"; break;
		case RETN:  return opstr = "RETN"; break;
		case FOOT: return opstr = "FOOT"; break;
		case READ: return opstr = "READ"; break;
		case WRITE: return opstr = "WRITE"; break;
		case WARRY: return opstr = "WARRY"; break;
		case RARRY: return opstr = "RARRY"; break;
	}	
	
}

