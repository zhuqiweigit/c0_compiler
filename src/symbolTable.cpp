#include <stdlib.h>
#include <iostream>
#include <string>
#include <strstream>
#include "lexicon.h"
#include "parser.h"
#include "Quaternary.h"
#include "symbolTable.h"
using namespace std;

vector <ftable> symTable;  //以函数为单元存储的符号表
long tempVarNum = 0;
long stringNum = 0;


string stringNameGen(){  //tag生成器，每次调用都生成一个独一无二的tag
	strstream ss;
    string s, s2("string");
    ss << stringNum;
	ss >> s;
    s = s2 + s;
	stringNum++;
    return s;
}

item symTableNew(string name){  //创建一个新的符号表,并且在符号表中插入该函数名字
  int p;
  ftable temp = new fTABLE();
  temp->name = name;
  symTable.push_back(temp);
  p = symTable.size() - 1;
  item tempi = new ITEM();
  tempi->name = name;
  (symTable[p]->itemTB).push_back(tempi);
  return tempi;

}



int findsymCur(string name){//在当前符号表中查找符号名字，如果查找到，返回1.从下标1开始找，略过函数名

    int p = symTable.size() - 1;
    int q = (symTable[p]->itemTB).size();
    int i;

for(i = 1; i < q; i++){
    if(name == symTable[p]->itemTB[i]->name){
        return 1;
    }

}
return 0;
}


int findsymGol(string name){  //在全局符号表中查找name
    int i, q = (symTable[0]->itemTB).size();
    for(i = 1; i < q; i++){
        if(name == symTable[0]->itemTB[i]->name){
            return 1;
     }
}
return 0;

}

int findFuncName(string name){  //在所有符号表中查找符号表名（函数名）
    unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return 1;
        }

    }
    return 0;

}

item findFunc(string name){  //查找函数名，并返回函数名的符号表项
    unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return (symTable[i]->itemTB[0]);
        }

    }

    return NULL;

}


int findFuncTable(string name){  //优化使用，根据函数名查找当前函数表的索引
	unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return i;
        }

    }

    return -1;

}

item findFuncCur(){   //获得当前所在函数的函数项
    int p = symTable.size() - 1;
    item tempi;
    tempi = symTable[p]->itemTB[0];
    return tempi;

}

item findcvpa(string name){  //查找常量、变量或参数或数组，先找本层，找不到就找全局，还找不到就返回NULL
    item tempi = NULL;
    unsigned int i;
    int p = symTable.size() - 1;
    for(i = 1; i < (symTable[p]->itemTB).size(); i++){
        if(name == (symTable[p]->itemTB[i])->name){
            tempi = symTable[p]->itemTB[i];
            return tempi;
        }

    }
    if(tempi == NULL){
        for(i = 1;  i < (symTable[0]->itemTB).size(); i++){
            if(name == (symTable[0]->itemTB[i])->name){
                tempi = symTable[0]->itemTB[i];
                return tempi;
            }
        }

    }
    return tempi;

}




string toStringKind(_kind a){
    switch(a){
        case constant: return "constant"; break;
        case variable: return "variable"; break;
        case parameter: return "parameter"; break;
        case func: return "func"; break;
        case arrays: return "arrays"; break;
		case nope: return "nope"; break;
        default: return ""; break;
    }


}

string toStringType(_type a){
    switch(a){
        case ints: return "ints"; break;
		case cints: return "cints"; break;
        case chars: return "chars"; break;
        case cchars: return "cchars"; break;
        case cstring: return "cstring"; break;
        case nop: return "nop"; break;
        default: return "";break;

    }

}





void constdefInsert(_type tp, string name, long value, int level){   //插入常量定义至当前符号表
    int p = symTable.size() - 1;
    item tempi = new ITEM();
    itemInit(tempi);
    tempi->kind = constant;
    tempi->type = tp;
    tempi->name = name;
    tempi->level = level;
    if(tp == ints){
      tempi->valueInt = value;
    }else{
      tempi->valueChar = (char)value;
    }
    tempi->isTemp = 0;
    (symTable[p]->itemTB).push_back(tempi);
}


void vardefInsert(_type tp, _kind kd, string name, long length, int level){   //插入变量定义至当前符号表
    int p = symTable.size() - 1;
    item tempi = new ITEM();
    itemInit(tempi);
    tempi->kind = kd;
    tempi->type = tp;
    tempi->name = name;
    tempi->level = level;
    if(kd == arrays){
      tempi->length = length;
    }
    tempi->isTemp = 0;
    (symTable[p]->itemTB).push_back(tempi);
}

item paraInsert(_type tp, string name, int parath){
    int p = symTable.size() - 1;
    item tempi = new ITEM();
    itemInit(tempi);
    tempi->kind = parameter;
    tempi->type = tp;
    tempi->name = name;
    tempi->isTemp = 0;
    tempi->level = 1;
	tempi->parath = parath;
    (symTable[p]->itemTB).push_back(tempi);
    return tempi;

}



string genTempVarName(){   //为临时变量产生一个独一无二的名字
    strstream ss;
    string s, s2("0t");  //名字前加0，防止和已有变量重名
    ss << tempVarNum;
	ss >> s;
    s = s2 + s;
    tempVarNum++;
    return s;
}



item genTempVar(_type tp){  //产生临时变量，并存入符号表
    item tempi = new ITEM();
    int p = symTable.size() - 1;
    itemInit(tempi);
    tempi->kind = variable;
    tempi->type = tp;
    tempi->name = genTempVarName();
    tempi->level = 1;
    tempi->isTemp = 1;
    (symTable[p]->itemTB).push_back(tempi);

    return tempi;

}

item genCints(long valueInt){  //生成字面量item，不插入符号表
   // int p = symTable.size() - 1;
    item tempi = new ITEM();
    itemInit(tempi);
    tempi->kind = constant;
    tempi->type = cints;
    tempi->level = 1;
    tempi->isTemp = 0;
    tempi->valueInt = valueInt;
   
    return tempi;

}

item genChars(long valueChar){  //生成字面量item，不插入符号表
   // int p = symTable.size() - 1;
    item tempi = new ITEM();
    itemInit(tempi);
    tempi->kind = constant;
    tempi->type = cchars;
    tempi->level = 1;
    tempi->isTemp = 0;
    tempi->valueChar = valueChar;
 
    return tempi;

}

item genCstring(string str){  //不存入符号表
	//int i = 0, tag = 0;
    item tempi = new ITEM();
    itemInit(tempi);
	tempi->name = stringNameGen();
    tempi->kind = constant;
    tempi->type = cstring;
    tempi->level = 0;
    tempi->isTemp = 0;
    tempi->svalue = str;
    return tempi;

}



void itemInit(item &it){
   it->kind = nope;
   it->type = nop;
   it->isTemp = 0;
   it->paranum = 0;
   it->returnV = nop;
   it->length = 0;
   it->valueInt = 0;
   it->valueChar = 0;
   it->isOptimized = 0; 
   it->isDelete = 0;
   it->reg = -1;
}




void funcInsert(int paranum, _type returnV){   //补充当前函数的详细信息到符号表
    int p = symTable.size() - 1;
    item tempi = symTable[p]->itemTB[0];
    tempi->kind = func;
    tempi->type = nop;
    tempi->isTemp = 0;
    tempi->paranum = paranum;
    tempi->returnV = returnV;
    
}


void parameterInfoInsert(int paraNum){   //补充参数总数到当前符号表的所有形参
	int p = symTable.size() - 1;
	unsigned int j = 0;

	for(j = 0; j < symTable[p]->itemTB.size(); j++){
		if(symTable[p]->itemTB[j]->kind == parameter){
			symTable[p]->itemTB[j]->paranum = paraNum;
		}
	

	}

}


void addrdispatch(){   //分配内存
	unsigned int i = 0, j = 0, k = 0;
	int baseAddr = 44, funclen = 0 , leng;
	item obj;
	for(i = 0; i < symTable.size(); i++){
		baseAddr = 44;
		funclen = 0;
		for(j = 1; j < symTable[i]->itemTB.size(); j++){
			obj =  symTable[i]->itemTB[j];
			if(obj->kind == constant || obj->kind == variable || obj->kind == parameter){
				obj->addr = baseAddr;
				baseAddr = baseAddr + 4;
				funclen++;
				
			}else if(obj->kind == arrays){
				leng = obj->length;
				obj->addr = baseAddr;
				baseAddr = baseAddr + 4 * leng;
				funclen = funclen + obj->length;
			}
				
		}
		symTable[i]->itemTB[0]->funlen = funclen;
	
	}

}






void printSymTable(){  //打印符号表
    unsigned int i,j;
    for(i = 0; i < symTable.size(); i++){
        cout << "符号表：" << symTable[i]->name <<endl;
        for(j = 0; j < (symTable[i]->itemTB).size(); j++){
            cout << " item " << j << ":"<<endl;
            cout << "	name: " << symTable[i]->itemTB[j]->name << endl;
            cout << "	kind: " << toStringKind(symTable[i]->itemTB[j]->kind) <<endl;
            cout << "	type: " << toStringType(symTable[i]->itemTB[j]->type) <<endl;
			cout << "	level:" << symTable[i]->itemTB[j]->level <<endl;
            cout << "	isTemp:" << symTable[i]->itemTB[j]->isTemp <<endl;
            cout << "	addr:" << symTable[i]->itemTB[j]->addr <<endl;
            cout << "	paranum:" << symTable[i]->itemTB[j]->paranum <<endl;
            cout << "	returnV:" << toStringType(symTable[i]->itemTB[j]->returnV) <<endl;
            cout << "	length:" << symTable[i]->itemTB[j]->length << endl;
            cout << "	valueInt:"<< symTable[i]->itemTB[j]->valueInt<<endl;
            cout << "	valueChar:" << symTable[i]->itemTB[j]->valueChar<<endl;
            cout << "	svalue:" << symTable[i]->itemTB[j]->svalue<<endl;
			cout << "	funlen:" << symTable[i]->itemTB[j]->funlen <<endl;
			cout << "	parath:" << symTable[i]->itemTB[j]->parath <<endl;

        }

    }

}


























