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

	cout << "输入文件路径：" << endl;
    gets(path);
	
	
    if(NULL == (fp = fopen(path,"r"))){  /*"F:\abianyishiyan\测试程序集\mytest 2.0.txt" */
        printf("文件打开失败");
        return 0;
    }

	getch(); //if(sym == endsym) { error 程序未正常结束  return;} 除了最后结束的时候的sym以外 
    initSymBuffer();
    firstUse = 0;   
    getsym();
	program();

	


	if(err == 0){
	   addrdispatch();  //分配地址空间
	   	//printSymTable();
	   //printQuaternary();
	   getBlocks();  //划分基本块
	   active_value();  //活跃变量分析与寄存器分配
	 //  printBlocks();

	   fetchcode(); //生成目标代码
	   printcode(); //写入文件
	   cout << "编译成功！" << endl;
	}
	cout << "错误数：" << err<<endl;
	cin >> z;
    return 0;
	  
}
