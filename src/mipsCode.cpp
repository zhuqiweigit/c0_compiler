
#include <fstream>
#include <stdlib.h>
#include <strstream>
#include <string>
#include <iostream>              ////建立string 的名字， 为函数计算参数个数，并分配变量地址， 为参数说明自己是第几个形参，在data段为string生成
#include "Quaternary.h"
#include "lexicon.h"
#include "parser.h"
#include "symbolTable.h"
#include "mipsCode.h"
#include "optimizer.h"
using namespace std;

vector<string> codes;  //存储生成的mips汇编
int mipsTagNum = 0;
int base = 0x10040000;  //栈空间基址
string funcNameHead("func_");
int fdfunc; //当前正在翻译的函数在表中的索引
regs_pool pool = new REGS_POOL();
int debug = 0;
int needload = 1;  //由寄存器池判断是否需要载入

string req_reg(item obj);
//int database = 8000;
string num_to_str(int numb){

	string temp;
	strstream ss;
	ss << numb;
	ss >> temp;

	return temp;

}



string mipsTagGen(){   //mips汇编生成tag
	string temp("jtag");
	string tempn;
	strstream ss;
	ss <<mipsTagNum;
	ss >> tempn;
	mipsTagNum++;
	temp = temp + tempn;
	return temp;

}


void loadreg(item obj, string reg){   //使用了$t7
	string mipscd("");
	int objaddr;
	if(obj->isOptimized == 1)
		return;
	if(obj->level == 1){
		if(obj->kind == constant){
			 mipscd = "addi " + reg + "," + " $zero,";
				if(obj->type == cchars || obj->type == chars){
					mipscd = mipscd + num_to_str((int)obj->valueChar);
					
				}else{
					mipscd = mipscd + num_to_str((int)obj->valueInt);
				
				}
			codes.push_back(mipscd);
		
		}else if(obj->kind == parameter || obj->kind == variable){
			objaddr = obj->addr;
			mipscd = "lw " + reg + ", " + num_to_str(objaddr) + "($fp)";
			codes.push_back(mipscd);
		
		}

	}else if(obj->level == 0){    //使用栈基地址寻址
		 if(obj->kind == constant){
			 mipscd = "addi " + reg + "," + " $zero,";
				if(obj->type == cchars || obj->type == chars){
					mipscd = mipscd + num_to_str((int)obj->valueChar );
					
				}else{
					mipscd = mipscd + num_to_str((int)obj->valueInt );
				
				}
			codes.push_back(mipscd);
		
		}else if(obj->kind == parameter || obj->kind == variable){
			mipscd = "addi $t7, $zero, " + num_to_str(base);
			codes.push_back(mipscd);
			objaddr = obj->addr;
			mipscd = "lw " + reg + ", " + num_to_str(objaddr) + "($t7)";
			codes.push_back(mipscd);
		
		}
		
	
	}

}

void loadreg2(item obj, string reg){   //使用了$t7
	string mipscd("");
	int objaddr;
	if(obj->level == 1){
		if(obj->kind == constant){
			 mipscd = "addi " + reg + "," + " $zero,";
				if(obj->type == cchars || obj->type == chars){
					mipscd = mipscd + num_to_str((int)obj->valueChar);
					
				}else{
					mipscd = mipscd + num_to_str((int)obj->valueInt);
				
				}
			codes.push_back(mipscd);
		
		}else if(obj->kind == parameter || obj->kind == variable){
			objaddr = obj->addr;
			mipscd = "lw " + reg + ", " + num_to_str(objaddr) + "($fp)";
			codes.push_back(mipscd);
		
		}

	}else if(obj->level == 0){    //使用栈基地址寻址
		 if(obj->kind == constant){
			 mipscd = "addi " + reg + "," + " $zero,";
				if(obj->type == cchars || obj->type == chars){
					mipscd = mipscd + num_to_str((int)obj->valueChar );
					
				}else{
					mipscd = mipscd + num_to_str((int)obj->valueInt );
				
				}
			codes.push_back(mipscd);
		
		}else if(obj->kind == parameter || obj->kind == variable){
			mipscd = "addi $t7, $zero, " + num_to_str(base);
			codes.push_back(mipscd);
			objaddr = obj->addr;
			mipscd = "lw " + reg + ", " + num_to_str(objaddr) + "($t7)";
			codes.push_back(mipscd);
		
		}
		
	
	}

}

void savereg(item obj, string reg){   //使用了$t7
	string mipscd("");
	int objaddr;
	if(obj->isOptimized == 1)
		return;
	if(obj->level == 0){
		objaddr = obj->addr;
		objaddr = objaddr + base;
		mipscd = "addi $t7, $zero, " + num_to_str(objaddr);
		codes.push_back(mipscd);
		mipscd = "sw " + reg+ ", 0($t7)";  //有改动
		codes.push_back(mipscd);
	
	}else{
		
		objaddr = obj->addr;
		mipscd = "sw " + reg + "," + num_to_str(objaddr) + "($fp)";   
		codes.push_back(mipscd);	
	}


}

void savereg2(item obj, string reg){   //使用了$t7
	string mipscd("");
	int objaddr;
	if(obj->level == 0){
		objaddr = obj->addr;
		objaddr = objaddr + base;
		mipscd = "addi $t7, $zero, " + num_to_str(objaddr);
		codes.push_back(mipscd);
		mipscd = "sw " + reg+ ", 0($t7)";  //有改动
		codes.push_back(mipscd);
	
	}else{
		
		objaddr = obj->addr;
		mipscd = "sw " + reg + "," + num_to_str(objaddr) + "($fp)";   
		codes.push_back(mipscd);	
	}


}


void addressing(item arr, item offset){   //数组寻址，将绝对地址写入$t9,使用t7,t8,t9
	string  reg1, reg2;  //op1数组，op2偏移
	string mipscd("");
	int arraddr, offsetaddr;

	if(arr->level == 0){
			 if(offset->level == 0){  //数组和偏移都是0层全局
				 if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{
					 reg2 = req_reg(offset);  //载入偏移
					 if(needload == 0){
						 mipscd = "add $t7, $zero, " + reg2;
						 codes.push_back(mipscd);
					 }else if(needload == 1){
						 loadreg(offset, reg2);
						 mipscd = "add $t7, $zero, " + reg2;
						 codes.push_back(mipscd);
					
					 }
					
				 }
		 
			 }else{  //数组0层，偏移1层

				 if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{

					  reg2 = req_reg(offset);  //载入偏移
					  if(needload == 0){
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					  }else if(needload == 1){
						  loadreg(offset, reg2);
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					
					 }
				 }
		 
			 }

			 mipscd = "addi $t8, $zero, 4";
			 codes.push_back(mipscd);
			 mipscd = "mul $t7, $t7, $t8"; //获得相对于数组的偏移量
			 codes.push_back(mipscd);
			 mipscd = "addi $t8, $zero, " + num_to_str(base);  //载入全局变量的基址
			 codes.push_back(mipscd);
			 arraddr = arr->addr;
			 mipscd = "addi $t7, $t7, " + num_to_str(arraddr); //获得相对base的偏移
			 codes.push_back(mipscd);
			 mipscd = "add $t8, $t8, $t7"; // 获取绝对地址
			 codes.push_back(mipscd);
			 mipscd = "add $t9, $zero, $t8";
			 codes.push_back(mipscd);

		 }else{  //数组是1层数组
			  if(offset->level == 0){  //数组1层，偏移0
				  if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				  }else{

					  reg2 = req_reg(offset);  //载入偏移
					  if(needload == 0){
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					  }else if(needload == 1){
						  loadreg(offset, reg2);
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					 }		
				 }
			  }else{  //数组1层，偏移1层
				  if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{
					  reg2 = req_reg(offset);  //载入偏移
					  if(needload == 0){
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					  }else if(needload == 1){
						  loadreg(offset, reg2);
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					
					  }
				}
				 
			  }
			 
			  mipscd = "addi $t8, $zero, 4";
			  codes.push_back(mipscd);
			  mipscd = "mul $t7, $t7, $t8"; //计算相对于数组的偏移
			  codes.push_back(mipscd);
			  arraddr = arr->addr;
			  mipscd = "addi $t7, $t7, " + num_to_str(arraddr);  //相对fp的偏移
			  codes.push_back(mipscd);
			  mipscd = "add $t7, $t7, $fp"; //绝对地址
			  codes.push_back(mipscd);
			  mipscd = "add $t9, $t7, $zero";
			  codes.push_back(mipscd);
		 	 
		  }



}


void clear_pool(){  //清空寄存器池
	for(int i = 0; i < 7; i++){
		pool->regs[i] = NULL;
	}
	pool->regPointer = -1;

	for(int i = 0; i < 3; i++)
		pool->recent[i] = -1;
	pool->rept = -1;
}

void writeback_pool(){  //写回寄存器池
	string reg_str = "$t";
	for(unsigned i = 0; i < 7; i++)
		if(pool->regs[i] != NULL && (pool->regs[i]->kind == variable || pool->regs[i]->kind == parameter))  //常量不用写回
			savereg(pool->regs[i], reg_str + num_to_str(i));
			
		
}

int find(item a[], item &it, int a_len){  //查找指定数组，找到返回下标，否则-1
	for(int i = 0; i < a_len; i++){
		if(a[i] == it)
			return i;
	}
	return -1;
}
int find(int a[], int b, int len){
	int tag = 0;
	for(int i = 0; i < len; i++)
		if(a[i] == b)
			return 1;

	return 0;

}
int findAReg(){    //找到一个不是最近新用过的寄存器
	int i = (pool->regPointer + 1) % 7;
	while(find(pool->recent, i, 3)){
		i = (i + 1) % 7;
	}
	return i;
}

string req_reg(item obj){  //申请临时寄存器,若该函数已被分配，则直接返回该寄存器
	string mipscd;
	int fd;
	if(obj->isOptimized == 1){  //全局寄存器，不需分配
		needload = 0;
		return "$s" + num_to_str(obj->reg);
	}else{  //需分配
		fd = find(pool->regs, obj, 7);
		if(fd != -1){  //已分配
			needload = 0;
			pool->rept = (pool->rept + 1) % 3;
			pool->recent[pool->rept] = fd;  //纪录最近新用过的寄存器
			return "$t" + num_to_str(fd);
		}else{  //未分配
			needload = 1;
			pool->regPointer = findAReg();  //寻找一个合适的寄存器
			pool->rept = (pool->rept + 1) % 3;
			pool->recent[pool->rept] = pool->regPointer;  //纪录最近新用过的寄存器

			if(pool->regs[pool->regPointer] == NULL){
				pool->regs[pool->regPointer] = obj;
				return "$t" + num_to_str(pool->regPointer);
			
			}else{
				if(pool->regs[pool->regPointer] != NULL && (pool->regs[pool->regPointer]->kind == variable || pool->regs[pool->regPointer]->kind == parameter))
					savereg(pool->regs[pool->regPointer], "$t" + num_to_str(pool->regPointer)); //先写回前一个寄存器
				pool->regs[pool->regPointer] = obj;
				return "$t" + num_to_str(pool->regPointer);
			
			}

		}
	
	}

}


void genMips(quater qua){
	string reg1, reg2, regObj;
	opcode opcd = qua->opc;
	string mipscd(""), intv, compareTag;
	item  op1 = qua->op1, op2 = qua->op2, obj = qua->obj;
	int Npara = 0, funlen = 0, parath = 0;

	if(qua->isBlockB == 1){  //块开始语句清空寄存器池
		clear_pool();
	}
	
	if(opcd == ADD || opcd == SUB || opcd == MUL || opcd == DIV){
		 if(debug == 1){
			 if(opcd == DIV)
				 mipscd =  "#div_____";
			 if(opcd == ADD)
				 mipscd =  "#add_____";
			 if(opcd == SUB)
				 mipscd =  "#sub_____";
			 if(opcd == MUL)
				 mipscd =  "#mul_____";
			codes.push_back(mipscd);
		 }
		 
		reg1 = req_reg(op1); //申请
		if(needload == 1){
			loadreg(op1, reg1);
		}
		reg2 = req_reg(op2);
		if(needload == 1){
			loadreg(op2, reg2);
		}
		regObj = req_reg(obj);


		if(opcd == ADD){
			mipscd = "add "+ regObj + "," + reg1 +"," + reg2;  //计算
		}else if(opcd == SUB){
			mipscd = "sub "+ regObj + "," + reg1 +"," + reg2;  //计算
		
		}else if(opcd == MUL){
			mipscd = "mul "+ regObj + "," + reg1 +"," + reg2;  //计算
		
		}else if(opcd == DIV){
			mipscd = "div "+ regObj + "," + reg1 +"," + reg2;  //计算
		}
		codes.push_back(mipscd);

		//写回
		//savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		 writeback_pool();
		 clear_pool();
	    }

	
	}else if(opcd == NEG){
		 if(debug == 1){
			 mipscd =  "#neg_____";
			codes.push_back(mipscd);
		 }

		reg1 = req_reg(op1); //申请
		if(needload == 1){
			loadreg(op1, reg1);
		}
		regObj = req_reg(obj);


		mipscd = "sub " + regObj +", $zero," + reg1;
		codes.push_back(mipscd);

		//写回
		//savereg(obj, regObj);
		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		 writeback_pool();
		 clear_pool();
	    }
	
	}else if(opcd == ASSIGN){
		 if(debug == 1){
			 mipscd =  "#assign_____";
			codes.push_back(mipscd);
		 }

		reg1 = req_reg(op1);
		if(needload == 1){
			loadreg(op1, reg1);
		}

		regObj = req_reg(obj); //申请
		if(needload == 1){
			loadreg(obj, regObj);
		}
		mipscd = "add " + regObj + ", $zero, " + reg1;
		codes.push_back(mipscd);
		
		//savereg(obj, reg1);
		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		 writeback_pool();
		 clear_pool();
	    }
	
	
	}else if(opcd == GTR || opcd == LES || opcd == EQL || opcd == GEQ || opcd == LEQ || opcd == NEQ ){  // 使用9号寄存器
		 if(debug == 1){
			 mipscd =  "#gtr_____";
			codes.push_back(mipscd);
		 }
		reg1 = req_reg(op1); //申请
		if(needload == 1){
			loadreg(op1, reg1);
		}
		reg2 = req_reg(op2);
		if(needload == 1){
			loadreg(op2, reg2);
		}
		regObj = req_reg(obj);


		mipscd = "add " + regObj + ", $zero, $zero"; //目标寄存器清0
		codes.push_back(mipscd);
		mipscd = "sub $t9," + reg1 + "," + reg2;
		codes.push_back(mipscd);
		if(opcd == GTR){
			compareTag = mipsTagGen();
			mipscd = "blez $t9," + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj +", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);		
		
		}else if(opcd == LES){
			compareTag = mipsTagGen();
			mipscd = "bgez $t9," + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj + ", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);
	
		}else if(opcd == EQL){
			compareTag = mipsTagGen();
			mipscd = "bne $t9,$zero," + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj + ", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);
			
		}else if(opcd == GEQ){
			compareTag = mipsTagGen();
			mipscd = "bltz $t9," + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj +", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);
	
		}else if(opcd == LEQ){
			compareTag = mipsTagGen();
			mipscd = "bgtz $t9," + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj +", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);

		}else if(opcd == NEQ){
			compareTag = mipsTagGen();
			mipscd = "beq $t9,$zero, " + compareTag;
			codes.push_back(mipscd);
			mipscd = "addi " + regObj +", $zero, 1";
			codes.push_back(mipscd);
			mipscd = compareTag + ":";
			codes.push_back(mipscd);		
		}

		//写回t2
		   //savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		 writeback_pool();
		 clear_pool();
	    }
	
	}else if(opcd == NEQZ){
		 if(debug == 1){
			 mipscd =  "#neqz_____";
			codes.push_back(mipscd);
		 }

		reg1 = req_reg(op1); //申请
		if(needload == 1){
			loadreg(op1, reg1);
		}
		regObj = req_reg(obj);

		mipscd = "add " + regObj + ", $zero, $zero"; //目标寄存器清0
		codes.push_back(mipscd);
		compareTag = mipsTagGen();
		mipscd = "beq " + reg1 + ", $zero, " + compareTag;
		codes.push_back(mipscd);
		mipscd = "addi " + regObj +", $zero, 1";
		codes.push_back(mipscd);
		mipscd = compareTag + ":";
		codes.push_back(mipscd);

		//写回t2
		//savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		 writeback_pool();
		 clear_pool();
	    }


	   }else if(opcd == TJMP){
		    if(debug == 1){
				mipscd =  "#tjmp_____";
			 codes.push_back(mipscd);
			}

			 reg1 = req_reg(op1); //申请
			 if(needload == 1){
				loadreg(op1, reg1);
			 }

			 mipscd = "addi $t9, $zero, 1";
			 codes.push_back(mipscd);

			 if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池,必须在跳转前执行
				writeback_pool();
				clear_pool();
			 }

			 mipscd = "beq " + reg1 + ", $t9," + obj->svalue;
			 codes.push_back(mipscd);

	 }else if(opcd == FJMP){
		  if(debug == 1){
			  mipscd =  "#fjmp_____";
			codes.push_back(mipscd);
		  }
			 
			 reg1 = req_reg(op1); //申请
			 if(needload == 1){
				loadreg(op1, reg1);
			 }

			 mipscd = "addi $t9, $zero, 0";
			 codes.push_back(mipscd);

			 if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				 writeback_pool();
				 clear_pool();
			 }

			 mipscd = "beq " + reg1 +", $t9," + obj->svalue;
			 codes.push_back(mipscd);
	   
	   
	   }else if(opcd == JMP){
		    if(debug == 1){
				mipscd =  "#jmp_____";
			    codes.push_back(mipscd);
			}
			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				writeback_pool();
				clear_pool();
			}
		    mipscd = "j, " + obj->svalue;
			codes.push_back(mipscd);
	 
	 
	 }else if(opcd == TAG){
		  if(debug == 1){
			  mipscd =  "#tag_____";
			 codes.push_back(mipscd);
		  }
			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				writeback_pool();
				clear_pool();
			}
	        mipscd = obj->svalue + ":";
			codes.push_back(mipscd);
	   
	 }else if(opcd == HEADER){
		  if(debug == 1){
			  mipscd =  "#header_____";
				codes.push_back(mipscd);
		   }
			mipscd = funcNameHead + obj->name + ":";
			codes.push_back(mipscd);
			mipscd = "sw $ra, 0($sp)";   //保存ra
			codes.push_back(mipscd);
			mipscd = "sw $fp, 4($sp)";  //保存fp
			codes.push_back(mipscd);
			Npara = obj->paranum;
			Npara = Npara * 4;
			mipscd = "subi $t0, $sp," + num_to_str(Npara);  //t0作为sp的回退地址
			codes.push_back(mipscd);
			mipscd = "sw $t0, 8($sp)";  //保存sp
			codes.push_back(mipscd);
			mipscd = "add $fp, $zero, $sp";  //建立自己的栈
			codes.push_back(mipscd);
			mipscd = "sw $s0, 12($sp)";  //保存s0
			codes.push_back(mipscd);
			mipscd = "sw $s1, 16($sp)";  //保存s1
			codes.push_back(mipscd);
			mipscd = "sw $s2, 20($sp)";  //保存s2
			codes.push_back(mipscd);
			mipscd = "sw $s3, 24($sp)";  //保存s3
			codes.push_back(mipscd);
			mipscd = "sw $s4, 28($sp)";  //保存s4
			codes.push_back(mipscd);
			mipscd = "sw $s5, 32($sp)";  //保存s5
			codes.push_back(mipscd);
			mipscd = "sw $s6, 36($sp)";  //保存s6
			codes.push_back(mipscd);
			mipscd = "sw $s7, 40($sp)";  //保存s7
			codes.push_back(mipscd);
			funlen = obj->funlen;
			funlen = funlen * 4 + 44; // 分配变量、数组、参数的空间 （从44开始）
			mipscd = "addi $sp, $sp, " + num_to_str(funlen);  //sp移动到栈顶
			codes.push_back(mipscd);

			//载入该函数的所有全局变量至s寄存器
			fdfunc = findFuncTable(obj->name);
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					loadreg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				 writeback_pool();
				 clear_pool();
			}

			
	  
	 
	 }else if(opcd == FORPARA){
		  if(debug == 1){
			  mipscd =  "#forpara_____";
			codes.push_back(mipscd);
		  }
			//参数拷贝,注意反向
			parath = obj->paranum - obj->parath + 1;  //反向
			parath = -4 * parath;
			mipscd = "lw $t9," + num_to_str(parath) +"($fp)";  //取参数
			codes.push_back(mipscd);
			mipscd = "sw $t9," + num_to_str(obj->addr) + "($fp)"; //拷贝参数	
			codes.push_back(mipscd);

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				 writeback_pool();
				clear_pool();
			}
	 
	 }else if(opcd == ACTPARA){
	     //传参
		  if(debug == 1){
			  mipscd =  "#actpara_____";
			  codes.push_back(mipscd);
		  }

		   regObj = req_reg(obj);
		   if(needload == 1){
			  loadreg(obj, regObj);
			  mipscd = "add $t8, $zero, " + regObj;
			  codes.push_back(mipscd);
		   }else if(needload == 0){ //obj的值未写回内存，在s寄存器中
			   mipscd = "add $t8, $zero, " + regObj;
			   codes.push_back(mipscd);	
		   }
		   mipscd = "sw $t8, 0($sp)";
		   codes.push_back(mipscd);
		   mipscd = "addi $sp, $sp, 4"; //栈顶增长
		   codes.push_back(mipscd);

		   if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				writeback_pool();
				clear_pool();
			}
	  
	 
	 }else if(opcd == RCALL){
		  if(debug == 1){
			  mipscd =  "#rcall_____";
			codes.push_back(mipscd);
		  }
		  writeback_pool();  //函数调用前保存、清空寄存器池
		  clear_pool();

		  mipscd = "jal " + funcNameHead + op1->name;  //直接跳转
		  codes.push_back(mipscd);
		  mipscd = "sw $v1, " + num_to_str(obj->addr) + "($fp)" ; //取返回值
		  codes.push_back(mipscd);

		 
	 
	 }else if(opcd == CALL){
		  if(debug == 1){
			  mipscd =  "#call_____";
			codes.push_back(mipscd);
		  }
		  writeback_pool();
		  clear_pool();

		  mipscd = "jal " + funcNameHead + op1->name;  //直接跳转
		  codes.push_back(mipscd);

		  
	  
	 }else if(opcd == NRETN){
		  if(debug == 1){
			  mipscd =  "#nretn_____";
			 codes.push_back(mipscd);
		  }

			//写回本函数的全局寄存器
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					savereg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				 writeback_pool();
				clear_pool();
			}



		    mipscd = "lw $ra, 0($fp)";   //恢复ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //恢复s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //恢复s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //恢复s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //恢复s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //恢复s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //恢复s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //恢复s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //恢复s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //恢复sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //恢复fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //返回
			codes.push_back(mipscd);
 
	 }else if(opcd == RETN){
		    if(debug == 1){
				mipscd =  "#retn_____";
			   codes.push_back(mipscd);
		    }
		 //写回本函数的全局寄存器
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					savereg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}


		    regObj = req_reg(obj);
			if(needload == 1){
				loadreg(obj, regObj);
				mipscd = "add $v1, $zero, " + regObj;
				codes.push_back(mipscd);
			}else if(needload == 0){
				mipscd = "add $v1, $zero, " + regObj;
				codes.push_back(mipscd);
			
			}

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				writeback_pool();
				clear_pool();
			}

			mipscd = "lw $ra, 0($fp)";   //恢复ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //恢复s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //恢复s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //恢复s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //恢复s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //恢复s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //恢复s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //恢复s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //恢复s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //恢复sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //恢复fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //返回
			codes.push_back(mipscd);
	 	 
	 }else if(opcd == FOOT){
		  if(debug == 1){
			  mipscd =  "#foot_____";
			 codes.push_back(mipscd);
		  }
			//写回本函数的全局寄存器
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					savereg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				 writeback_pool();
				clear_pool();
			}

		   //当没有显式返回时，作隐式返回
			mipscd = "lw $ra, 0($fp)";   //恢复ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //恢复s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //恢复s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //恢复s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //恢复s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //恢复s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //恢复s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //恢复s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //恢复s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //恢复sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //恢复fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //返回
			codes.push_back(mipscd);
	 
	 
	 }else if(opcd == READ){
		  if(debug == 1){
			  mipscd =  "#read_____";
			codes.push_back(mipscd);
		  }
		if(obj->type == ints){
			mipscd = "addi $v0, $zero, 5";
		}else{
			mipscd = "addi $v0, $zero, 12";
		}
			codes.push_back(mipscd);
			mipscd = "syscall";
			codes.push_back(mipscd);
			regObj = req_reg(obj);
			mipscd = "add " + regObj + ", $zero, $v0";
			codes.push_back(mipscd);

			if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
				writeback_pool();
				clear_pool();
			}

	 }else if(opcd == WRITE){
		  if(debug == 1){
			  mipscd =  "#write_____";
			 codes.push_back(mipscd);
		  }
		 if(obj->type != cstring){

			 regObj = req_reg(obj);
			 if(needload == 1){
				loadreg(obj, regObj);
			 }
			 mipscd = "add $a0, $zero, " + regObj;
			 codes.push_back(mipscd);

			if(obj->type == chars || obj->type == cchars){
				 mipscd = "addi $v0, $zero, 11";
				 codes.push_back(mipscd);
				 mipscd = "syscall";
				 codes.push_back(mipscd);
		
			}else if(obj->type == ints || obj->type == cints){
				 mipscd = "addi $v0, $zero, 1";
				 codes.push_back(mipscd);
				 mipscd = "syscall";
				 codes.push_back(mipscd);
		 
		 }
	 
		 }else if(obj->type == cstring){
			mipscd = "la $a0, " + obj->name;
			codes.push_back(mipscd);
			mipscd = "addi $v0, $zero, 4";
			codes.push_back(mipscd);
			mipscd = "syscall";
			codes.push_back(mipscd);
					  
		 
		 }

		 if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
			writeback_pool();
			clear_pool();
	     }
	 
	 
	 }else if(opcd == RARRY){  //读取数组元素
		  if(debug == 1){
			  mipscd =  "#rarry_____";
			codes.push_back(mipscd);
		  }
		 
		addressing(op1, op2); //寻址
		 
		mipscd = "lw $t8, 0($t9)"; //取数
		codes.push_back(mipscd);
		regObj = req_reg(obj);
		mipscd = "add " + regObj + ", $zero, $t8";
		codes.push_back(mipscd);
		//savereg(obj, "$t8"); //写入

		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		  writeback_pool();
		  clear_pool();
	    }
	 
	 }else if(opcd == WARRY){  //写回数组
		 if(debug == 1){
			 mipscd =  "#warry_____";
			codes.push_back(mipscd);
		 }
		addressing(op1, op2);
		regObj = req_reg(obj);
		if(needload == 1){
			loadreg(obj, regObj);
			mipscd = "add $t8, $zero, " + regObj;
			codes.push_back(mipscd);
		}else{
			mipscd = "add $t8, $zero, " + regObj;
			codes.push_back(mipscd);
		}
		mipscd = "sw $t8, 0($t9)";
		codes.push_back(mipscd);//写入

		if(qua->isBlockE == 1){  //块结束语句保存、清空寄存器池
		  writeback_pool();
		  clear_pool();
	    }
	 
	 }

	 

}



void fetchcode(){

	string mipscd;
	unsigned int i = 0;
	int gollen = symTable[0]->itemTB[0]->funlen;
	int addrbase = 0;
	mipscd = ".data";
	codes.push_back(mipscd);
	for(i = 0; i <  strdata.size(); i++){
		for(unsigned int strpt = 0; strpt < strdata[i]->svalue.length(); strpt++){   //测试转义字符
			if(strdata[i]->svalue[strpt] == '\\'){
				strdata[i]->svalue = strdata[i]->svalue.insert(strpt, 1, '\\');//此函数在p0处插入n个字符c
				strpt++;
			}
		}
		mipscd = strdata[i]->name + ": .asciiz " + "\"" + strdata[i]->svalue + "\"";
		codes.push_back(mipscd);
	}
	mipscd = ".text";
	codes.push_back(mipscd);
	mipscd = "add $ra, $zero, $zero";
	codes.push_back(mipscd);
	mipscd = "addi $fp, $zero, " + num_to_str(base);
	codes.push_back(mipscd);
	addrbase = gollen * 4 + base;
	mipscd = "addi $sp, $zero, " + num_to_str(addrbase + 44);   //和地址分配同步
	codes.push_back(mipscd);
	mipscd = "jal " + funcNameHead + "main";
	codes.push_back(mipscd);
	mipscd = "j end";
	codes.push_back(mipscd);
	for(i = 0; i < quaterList.size(); i++){
		genMips(quaterList[i]);
		
	}
	mipscd = "end:";
	codes.push_back(mipscd);

}


void printcode(){
	unsigned int i = 0;
	ofstream outfile;
	outfile.open("mymips.txt");
	for(i = 0; i < codes.size(); i++){
		outfile << codes[i] << endl;
		
	}
	outfile.close();

}



void printPool(){
	cout<<"指针："<< pool-> regPointer<<endl;
	for(int i = 0; i < 7; i++){
		cout<< "reg" << i<< " "<< pool->regs[i]->name<<endl;
	}
	cout <<"最近使用："<<endl;
	for(int i = 0; i < 3; i++){
		cout<< "reg" << pool->recent[i]<<endl;
	}


}