#include <stdlib.h>
#include <iostream>
#include <string>
#include <strstream>
#include "lexicon.h"
#include "parser.h"
#include "Quaternary.h"
#include "symbolTable.h"
using namespace std;

vector <ftable> symTable;  //�Ժ���Ϊ��Ԫ�洢�ķ��ű�
long tempVarNum = 0;
long stringNum = 0;


string stringNameGen(){  //tag��������ÿ�ε��ö�����һ����һ�޶���tag
	strstream ss;
    string s, s2("string");
    ss << stringNum;
	ss >> s;
    s = s2 + s;
	stringNum++;
    return s;
}

item symTableNew(string name){  //����һ���µķ��ű�,�����ڷ��ű��в���ú�������
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



int findsymCur(string name){//�ڵ�ǰ���ű��в��ҷ������֣�������ҵ�������1.���±�1��ʼ�ң��Թ�������

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


int findsymGol(string name){  //��ȫ�ַ��ű��в���name
    int i, q = (symTable[0]->itemTB).size();
    for(i = 1; i < q; i++){
        if(name == symTable[0]->itemTB[i]->name){
            return 1;
     }
}
return 0;

}

int findFuncName(string name){  //�����з��ű��в��ҷ��ű�������������
    unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return 1;
        }

    }
    return 0;

}

item findFunc(string name){  //���Һ������������غ������ķ��ű���
    unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return (symTable[i]->itemTB[0]);
        }

    }

    return NULL;

}


int findFuncTable(string name){  //�Ż�ʹ�ã����ݺ��������ҵ�ǰ�����������
	unsigned int i;
    for(i = 0; i < symTable.size(); i++){
        if(symTable[i]->name == name){
            return i;
        }

    }

    return -1;

}

item findFuncCur(){   //��õ�ǰ���ں����ĺ�����
    int p = symTable.size() - 1;
    item tempi;
    tempi = symTable[p]->itemTB[0];
    return tempi;

}

item findcvpa(string name){  //���ҳ�������������������飬���ұ��㣬�Ҳ�������ȫ�֣����Ҳ����ͷ���NULL
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





void constdefInsert(_type tp, string name, long value, int level){   //���볣����������ǰ���ű�
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


void vardefInsert(_type tp, _kind kd, string name, long length, int level){   //���������������ǰ���ű�
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



string genTempVarName(){   //Ϊ��ʱ��������һ����һ�޶�������
    strstream ss;
    string s, s2("0t");  //����ǰ��0����ֹ�����б�������
    ss << tempVarNum;
	ss >> s;
    s = s2 + s;
    tempVarNum++;
    return s;
}



item genTempVar(_type tp){  //������ʱ��������������ű�
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

item genCints(long valueInt){  //����������item����������ű�
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

item genChars(long valueChar){  //����������item����������ű�
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

item genCstring(string str){  //��������ű�
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




void funcInsert(int paranum, _type returnV){   //���䵱ǰ��������ϸ��Ϣ�����ű�
    int p = symTable.size() - 1;
    item tempi = symTable[p]->itemTB[0];
    tempi->kind = func;
    tempi->type = nop;
    tempi->isTemp = 0;
    tempi->paranum = paranum;
    tempi->returnV = returnV;
    
}


void parameterInfoInsert(int paraNum){   //���������������ǰ���ű�������β�
	int p = symTable.size() - 1;
	unsigned int j = 0;

	for(j = 0; j < symTable[p]->itemTB.size(); j++){
		if(symTable[p]->itemTB[j]->kind == parameter){
			symTable[p]->itemTB[j]->paranum = paraNum;
		}
	

	}

}


void addrdispatch(){   //�����ڴ�
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






void printSymTable(){  //��ӡ���ű�
    unsigned int i,j;
    for(i = 0; i < symTable.size(); i++){
        cout << "���ű�" << symTable[i]->name <<endl;
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


























