#include <iostream>
#include "lexicon.h"
#include "parser.h"
#include "Quaternary.h"
#include "symbolTable.h"
#include "mipsCode.h"
#include "error.h"
#include "optimizer.h"
using namespace std;
extern int firstUse;
void initSymBuffer();




int main()
{  

    int z = 0;
   // char name[20];
    char path[50];

	cout << "�����ļ�·����" << endl;
    gets(path);
	
	
    if(NULL == (fp = fopen(path,"r"))){  /*"F:\abianyishiyan\���Գ���\mytest 2.0.txt" */
        printf("�ļ���ʧ��");
        return 0;
    }

	getch(); //if(sym == endsym) { error ����δ��������  return;} ������������ʱ���sym���� 
    initSymBuffer();
    firstUse = 0;   
    getsym();
	program();

	


	if(err == 0){
	   addrdispatch();  //�����ַ�ռ�
	   	//printSymTable();
	   //printQuaternary();
	   getBlocks();  //���ֻ�����
	   active_value();  //��Ծ����������Ĵ�������
	 //  printBlocks();

	   fetchcode(); //����Ŀ�����
	   printcode(); //д���ļ�
	   cout << "����ɹ���" << endl;
	}
	cout << "��������" << err<<endl;
	cin >> z;
    return 0;
	  
}
