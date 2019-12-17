
#include <fstream>
#include <stdlib.h>
#include <strstream>
#include <string>
#include <iostream>              ////����string �����֣� Ϊ����������������������������ַ�� Ϊ����˵���Լ��ǵڼ����βΣ���data��Ϊstring����
#include "Quaternary.h"
#include "lexicon.h"
#include "parser.h"
#include "symbolTable.h"
#include "mipsCode.h"
#include "optimizer.h"
using namespace std;

vector<string> codes;  //�洢���ɵ�mips���
int mipsTagNum = 0;
int base = 0x10040000;  //ջ�ռ��ַ
string funcNameHead("func_");
int fdfunc; //��ǰ���ڷ���ĺ����ڱ��е�����
regs_pool pool = new REGS_POOL();
int debug = 0;
int needload = 1;  //�ɼĴ������ж��Ƿ���Ҫ����

string req_reg(item obj);
//int database = 8000;
string num_to_str(int numb){

	string temp;
	strstream ss;
	ss << numb;
	ss >> temp;

	return temp;

}



string mipsTagGen(){   //mips�������tag
	string temp("jtag");
	string tempn;
	strstream ss;
	ss <<mipsTagNum;
	ss >> tempn;
	mipsTagNum++;
	temp = temp + tempn;
	return temp;

}


void loadreg(item obj, string reg){   //ʹ����$t7
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

	}else if(obj->level == 0){    //ʹ��ջ����ַѰַ
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

void loadreg2(item obj, string reg){   //ʹ����$t7
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

	}else if(obj->level == 0){    //ʹ��ջ����ַѰַ
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

void savereg(item obj, string reg){   //ʹ����$t7
	string mipscd("");
	int objaddr;
	if(obj->isOptimized == 1)
		return;
	if(obj->level == 0){
		objaddr = obj->addr;
		objaddr = objaddr + base;
		mipscd = "addi $t7, $zero, " + num_to_str(objaddr);
		codes.push_back(mipscd);
		mipscd = "sw " + reg+ ", 0($t7)";  //�иĶ�
		codes.push_back(mipscd);
	
	}else{
		
		objaddr = obj->addr;
		mipscd = "sw " + reg + "," + num_to_str(objaddr) + "($fp)";   
		codes.push_back(mipscd);	
	}


}

void savereg2(item obj, string reg){   //ʹ����$t7
	string mipscd("");
	int objaddr;
	if(obj->level == 0){
		objaddr = obj->addr;
		objaddr = objaddr + base;
		mipscd = "addi $t7, $zero, " + num_to_str(objaddr);
		codes.push_back(mipscd);
		mipscd = "sw " + reg+ ", 0($t7)";  //�иĶ�
		codes.push_back(mipscd);
	
	}else{
		
		objaddr = obj->addr;
		mipscd = "sw " + reg + "," + num_to_str(objaddr) + "($fp)";   
		codes.push_back(mipscd);	
	}


}


void addressing(item arr, item offset){   //����Ѱַ�������Ե�ַд��$t9,ʹ��t7,t8,t9
	string  reg1, reg2;  //op1���飬op2ƫ��
	string mipscd("");
	int arraddr, offsetaddr;

	if(arr->level == 0){
			 if(offset->level == 0){  //�����ƫ�ƶ���0��ȫ��
				 if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{
					 reg2 = req_reg(offset);  //����ƫ��
					 if(needload == 0){
						 mipscd = "add $t7, $zero, " + reg2;
						 codes.push_back(mipscd);
					 }else if(needload == 1){
						 loadreg(offset, reg2);
						 mipscd = "add $t7, $zero, " + reg2;
						 codes.push_back(mipscd);
					
					 }
					
				 }
		 
			 }else{  //����0�㣬ƫ��1��

				 if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{

					  reg2 = req_reg(offset);  //����ƫ��
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
			 mipscd = "mul $t7, $t7, $t8"; //�������������ƫ����
			 codes.push_back(mipscd);
			 mipscd = "addi $t8, $zero, " + num_to_str(base);  //����ȫ�ֱ����Ļ�ַ
			 codes.push_back(mipscd);
			 arraddr = arr->addr;
			 mipscd = "addi $t7, $t7, " + num_to_str(arraddr); //������base��ƫ��
			 codes.push_back(mipscd);
			 mipscd = "add $t8, $t8, $t7"; // ��ȡ���Ե�ַ
			 codes.push_back(mipscd);
			 mipscd = "add $t9, $zero, $t8";
			 codes.push_back(mipscd);

		 }else{  //������1������
			  if(offset->level == 0){  //����1�㣬ƫ��0
				  if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				  }else{

					  reg2 = req_reg(offset);  //����ƫ��
					  if(needload == 0){
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					  }else if(needload == 1){
						  loadreg(offset, reg2);
						  mipscd = "add $t7, $zero, " + reg2;
						  codes.push_back(mipscd);
					 }		
				 }
			  }else{  //����1�㣬ƫ��1��
				  if(offset->kind == constant){
					 if(offset->type == cints || offset->type == ints){
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueInt);
						codes.push_back(mipscd);
					 }else{
						mipscd = "addi $t7, $zero, " + num_to_str(offset->valueChar);
						codes.push_back(mipscd);
					 
					 }
 
				 }else{
					  reg2 = req_reg(offset);  //����ƫ��
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
			  mipscd = "mul $t7, $t7, $t8"; //��������������ƫ��
			  codes.push_back(mipscd);
			  arraddr = arr->addr;
			  mipscd = "addi $t7, $t7, " + num_to_str(arraddr);  //���fp��ƫ��
			  codes.push_back(mipscd);
			  mipscd = "add $t7, $t7, $fp"; //���Ե�ַ
			  codes.push_back(mipscd);
			  mipscd = "add $t9, $t7, $zero";
			  codes.push_back(mipscd);
		 	 
		  }



}


void clear_pool(){  //��ռĴ�����
	for(int i = 0; i < 7; i++){
		pool->regs[i] = NULL;
	}
	pool->regPointer = -1;

	for(int i = 0; i < 3; i++)
		pool->recent[i] = -1;
	pool->rept = -1;
}

void writeback_pool(){  //д�ؼĴ�����
	string reg_str = "$t";
	for(unsigned i = 0; i < 7; i++)
		if(pool->regs[i] != NULL && (pool->regs[i]->kind == variable || pool->regs[i]->kind == parameter))  //��������д��
			savereg(pool->regs[i], reg_str + num_to_str(i));
			
		
}

int find(item a[], item &it, int a_len){  //����ָ�����飬�ҵ������±꣬����-1
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
int findAReg(){    //�ҵ�һ������������ù��ļĴ���
	int i = (pool->regPointer + 1) % 7;
	while(find(pool->recent, i, 3)){
		i = (i + 1) % 7;
	}
	return i;
}

string req_reg(item obj){  //������ʱ�Ĵ���,���ú����ѱ����䣬��ֱ�ӷ��ظüĴ���
	string mipscd;
	int fd;
	if(obj->isOptimized == 1){  //ȫ�ּĴ������������
		needload = 0;
		return "$s" + num_to_str(obj->reg);
	}else{  //�����
		fd = find(pool->regs, obj, 7);
		if(fd != -1){  //�ѷ���
			needload = 0;
			pool->rept = (pool->rept + 1) % 3;
			pool->recent[pool->rept] = fd;  //��¼������ù��ļĴ���
			return "$t" + num_to_str(fd);
		}else{  //δ����
			needload = 1;
			pool->regPointer = findAReg();  //Ѱ��һ�����ʵļĴ���
			pool->rept = (pool->rept + 1) % 3;
			pool->recent[pool->rept] = pool->regPointer;  //��¼������ù��ļĴ���

			if(pool->regs[pool->regPointer] == NULL){
				pool->regs[pool->regPointer] = obj;
				return "$t" + num_to_str(pool->regPointer);
			
			}else{
				if(pool->regs[pool->regPointer] != NULL && (pool->regs[pool->regPointer]->kind == variable || pool->regs[pool->regPointer]->kind == parameter))
					savereg(pool->regs[pool->regPointer], "$t" + num_to_str(pool->regPointer)); //��д��ǰһ���Ĵ���
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

	if(qua->isBlockB == 1){  //�鿪ʼ�����ռĴ�����
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
		 
		reg1 = req_reg(op1); //����
		if(needload == 1){
			loadreg(op1, reg1);
		}
		reg2 = req_reg(op2);
		if(needload == 1){
			loadreg(op2, reg2);
		}
		regObj = req_reg(obj);


		if(opcd == ADD){
			mipscd = "add "+ regObj + "," + reg1 +"," + reg2;  //����
		}else if(opcd == SUB){
			mipscd = "sub "+ regObj + "," + reg1 +"," + reg2;  //����
		
		}else if(opcd == MUL){
			mipscd = "mul "+ regObj + "," + reg1 +"," + reg2;  //����
		
		}else if(opcd == DIV){
			mipscd = "div "+ regObj + "," + reg1 +"," + reg2;  //����
		}
		codes.push_back(mipscd);

		//д��
		//savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
		 writeback_pool();
		 clear_pool();
	    }

	
	}else if(opcd == NEG){
		 if(debug == 1){
			 mipscd =  "#neg_____";
			codes.push_back(mipscd);
		 }

		reg1 = req_reg(op1); //����
		if(needload == 1){
			loadreg(op1, reg1);
		}
		regObj = req_reg(obj);


		mipscd = "sub " + regObj +", $zero," + reg1;
		codes.push_back(mipscd);

		//д��
		//savereg(obj, regObj);
		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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

		regObj = req_reg(obj); //����
		if(needload == 1){
			loadreg(obj, regObj);
		}
		mipscd = "add " + regObj + ", $zero, " + reg1;
		codes.push_back(mipscd);
		
		//savereg(obj, reg1);
		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
		 writeback_pool();
		 clear_pool();
	    }
	
	
	}else if(opcd == GTR || opcd == LES || opcd == EQL || opcd == GEQ || opcd == LEQ || opcd == NEQ ){  // ʹ��9�żĴ���
		 if(debug == 1){
			 mipscd =  "#gtr_____";
			codes.push_back(mipscd);
		 }
		reg1 = req_reg(op1); //����
		if(needload == 1){
			loadreg(op1, reg1);
		}
		reg2 = req_reg(op2);
		if(needload == 1){
			loadreg(op2, reg2);
		}
		regObj = req_reg(obj);


		mipscd = "add " + regObj + ", $zero, $zero"; //Ŀ��Ĵ�����0
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

		//д��t2
		   //savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
		 writeback_pool();
		 clear_pool();
	    }
	
	}else if(opcd == NEQZ){
		 if(debug == 1){
			 mipscd =  "#neqz_____";
			codes.push_back(mipscd);
		 }

		reg1 = req_reg(op1); //����
		if(needload == 1){
			loadreg(op1, reg1);
		}
		regObj = req_reg(obj);

		mipscd = "add " + regObj + ", $zero, $zero"; //Ŀ��Ĵ�����0
		codes.push_back(mipscd);
		compareTag = mipsTagGen();
		mipscd = "beq " + reg1 + ", $zero, " + compareTag;
		codes.push_back(mipscd);
		mipscd = "addi " + regObj +", $zero, 1";
		codes.push_back(mipscd);
		mipscd = compareTag + ":";
		codes.push_back(mipscd);

		//д��t2
		//savereg(obj, "$t2");
		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
		 writeback_pool();
		 clear_pool();
	    }


	   }else if(opcd == TJMP){
		    if(debug == 1){
				mipscd =  "#tjmp_____";
			 codes.push_back(mipscd);
			}

			 reg1 = req_reg(op1); //����
			 if(needload == 1){
				loadreg(op1, reg1);
			 }

			 mipscd = "addi $t9, $zero, 1";
			 codes.push_back(mipscd);

			 if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����,��������תǰִ��
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
			 
			 reg1 = req_reg(op1); //����
			 if(needload == 1){
				loadreg(op1, reg1);
			 }

			 mipscd = "addi $t9, $zero, 0";
			 codes.push_back(mipscd);

			 if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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
			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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
			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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
			mipscd = "sw $ra, 0($sp)";   //����ra
			codes.push_back(mipscd);
			mipscd = "sw $fp, 4($sp)";  //����fp
			codes.push_back(mipscd);
			Npara = obj->paranum;
			Npara = Npara * 4;
			mipscd = "subi $t0, $sp," + num_to_str(Npara);  //t0��Ϊsp�Ļ��˵�ַ
			codes.push_back(mipscd);
			mipscd = "sw $t0, 8($sp)";  //����sp
			codes.push_back(mipscd);
			mipscd = "add $fp, $zero, $sp";  //�����Լ���ջ
			codes.push_back(mipscd);
			mipscd = "sw $s0, 12($sp)";  //����s0
			codes.push_back(mipscd);
			mipscd = "sw $s1, 16($sp)";  //����s1
			codes.push_back(mipscd);
			mipscd = "sw $s2, 20($sp)";  //����s2
			codes.push_back(mipscd);
			mipscd = "sw $s3, 24($sp)";  //����s3
			codes.push_back(mipscd);
			mipscd = "sw $s4, 28($sp)";  //����s4
			codes.push_back(mipscd);
			mipscd = "sw $s5, 32($sp)";  //����s5
			codes.push_back(mipscd);
			mipscd = "sw $s6, 36($sp)";  //����s6
			codes.push_back(mipscd);
			mipscd = "sw $s7, 40($sp)";  //����s7
			codes.push_back(mipscd);
			funlen = obj->funlen;
			funlen = funlen * 4 + 44; // ������������顢�����Ŀռ� ����44��ʼ��
			mipscd = "addi $sp, $sp, " + num_to_str(funlen);  //sp�ƶ���ջ��
			codes.push_back(mipscd);

			//����ú���������ȫ�ֱ�����s�Ĵ���
			fdfunc = findFuncTable(obj->name);
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					loadreg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				 writeback_pool();
				 clear_pool();
			}

			
	  
	 
	 }else if(opcd == FORPARA){
		  if(debug == 1){
			  mipscd =  "#forpara_____";
			codes.push_back(mipscd);
		  }
			//��������,ע�ⷴ��
			parath = obj->paranum - obj->parath + 1;  //����
			parath = -4 * parath;
			mipscd = "lw $t9," + num_to_str(parath) +"($fp)";  //ȡ����
			codes.push_back(mipscd);
			mipscd = "sw $t9," + num_to_str(obj->addr) + "($fp)"; //��������	
			codes.push_back(mipscd);

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				 writeback_pool();
				clear_pool();
			}
	 
	 }else if(opcd == ACTPARA){
	     //����
		  if(debug == 1){
			  mipscd =  "#actpara_____";
			  codes.push_back(mipscd);
		  }

		   regObj = req_reg(obj);
		   if(needload == 1){
			  loadreg(obj, regObj);
			  mipscd = "add $t8, $zero, " + regObj;
			  codes.push_back(mipscd);
		   }else if(needload == 0){ //obj��ֵδд���ڴ棬��s�Ĵ�����
			   mipscd = "add $t8, $zero, " + regObj;
			   codes.push_back(mipscd);	
		   }
		   mipscd = "sw $t8, 0($sp)";
		   codes.push_back(mipscd);
		   mipscd = "addi $sp, $sp, 4"; //ջ������
		   codes.push_back(mipscd);

		   if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				writeback_pool();
				clear_pool();
			}
	  
	 
	 }else if(opcd == RCALL){
		  if(debug == 1){
			  mipscd =  "#rcall_____";
			codes.push_back(mipscd);
		  }
		  writeback_pool();  //��������ǰ���桢��ռĴ�����
		  clear_pool();

		  mipscd = "jal " + funcNameHead + op1->name;  //ֱ����ת
		  codes.push_back(mipscd);
		  mipscd = "sw $v1, " + num_to_str(obj->addr) + "($fp)" ; //ȡ����ֵ
		  codes.push_back(mipscd);

		 
	 
	 }else if(opcd == CALL){
		  if(debug == 1){
			  mipscd =  "#call_____";
			codes.push_back(mipscd);
		  }
		  writeback_pool();
		  clear_pool();

		  mipscd = "jal " + funcNameHead + op1->name;  //ֱ����ת
		  codes.push_back(mipscd);

		  
	  
	 }else if(opcd == NRETN){
		  if(debug == 1){
			  mipscd =  "#nretn_____";
			 codes.push_back(mipscd);
		  }

			//д�ر�������ȫ�ּĴ���
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					savereg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				 writeback_pool();
				clear_pool();
			}



		    mipscd = "lw $ra, 0($fp)";   //�ָ�ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //�ָ�s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //�ָ�s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //�ָ�s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //�ָ�s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //�ָ�s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //�ָ�s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //�ָ�s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //�ָ�s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //�ָ�sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //�ָ�fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //����
			codes.push_back(mipscd);
 
	 }else if(opcd == RETN){
		    if(debug == 1){
				mipscd =  "#retn_____";
			   codes.push_back(mipscd);
		    }
		 //д�ر�������ȫ�ּĴ���
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

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				writeback_pool();
				clear_pool();
			}

			mipscd = "lw $ra, 0($fp)";   //�ָ�ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //�ָ�s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //�ָ�s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //�ָ�s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //�ָ�s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //�ָ�s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //�ָ�s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //�ָ�s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //�ָ�s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //�ָ�sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //�ָ�fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //����
			codes.push_back(mipscd);
	 	 
	 }else if(opcd == FOOT){
		  if(debug == 1){
			  mipscd =  "#foot_____";
			 codes.push_back(mipscd);
		  }
			//д�ر�������ȫ�ּĴ���
			for(int ii = 0; ii < symTable[fdfunc]->itemTB.size(); ii++){
				item itt = symTable[fdfunc]->itemTB[ii];
				if((itt->kind == variable || itt->kind == parameter) && itt->isOptimized == 1){
					savereg2(itt, "$s" + num_to_str(itt->reg));
				}
			
			}

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
				 writeback_pool();
				clear_pool();
			}

		   //��û����ʽ����ʱ������ʽ����
			mipscd = "lw $ra, 0($fp)";   //�ָ�ra
			codes.push_back(mipscd);
			mipscd = "lw $s0, 12($fp)";  //�ָ�s0
			codes.push_back(mipscd);
			mipscd = "lw $s1, 16($fp)";  //�ָ�s1
			codes.push_back(mipscd);
			mipscd = "lw $s2, 20($fp)";  //�ָ�s2
			codes.push_back(mipscd);
			mipscd = "lw $s3, 24($fp)";  //�ָ�s3
			codes.push_back(mipscd);
			mipscd = "lw $s4, 28($fp)";  //�ָ�s4
			codes.push_back(mipscd);
			mipscd = "lw $s5, 32($fp)";  //�ָ�s5
			codes.push_back(mipscd);
			mipscd = "lw $s6, 36($fp)";  //�ָ�s6
			codes.push_back(mipscd);
			mipscd = "lw $s7, 40($fp)";  //�ָ�s7
			codes.push_back(mipscd);
			mipscd = "lw $sp, 8($fp)";  //�ָ�sp
			codes.push_back(mipscd);  
			mipscd = "lw $fp, 4($fp)";  //�ָ�fp
			codes.push_back(mipscd);
			mipscd = "jr $ra"; //����
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

			if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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

		 if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
			writeback_pool();
			clear_pool();
	     }
	 
	 
	 }else if(opcd == RARRY){  //��ȡ����Ԫ��
		  if(debug == 1){
			  mipscd =  "#rarry_____";
			codes.push_back(mipscd);
		  }
		 
		addressing(op1, op2); //Ѱַ
		 
		mipscd = "lw $t8, 0($t9)"; //ȡ��
		codes.push_back(mipscd);
		regObj = req_reg(obj);
		mipscd = "add " + regObj + ", $zero, $t8";
		codes.push_back(mipscd);
		//savereg(obj, "$t8"); //д��

		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
		  writeback_pool();
		  clear_pool();
	    }
	 
	 }else if(opcd == WARRY){  //д������
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
		codes.push_back(mipscd);//д��

		if(qua->isBlockE == 1){  //�������䱣�桢��ռĴ�����
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
		for(unsigned int strpt = 0; strpt < strdata[i]->svalue.length(); strpt++){   //����ת���ַ�
			if(strdata[i]->svalue[strpt] == '\\'){
				strdata[i]->svalue = strdata[i]->svalue.insert(strpt, 1, '\\');//�˺�����p0������n���ַ�c
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
	mipscd = "addi $sp, $zero, " + num_to_str(addrbase + 44);   //�͵�ַ����ͬ��
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
	cout<<"ָ�룺"<< pool-> regPointer<<endl;
	for(int i = 0; i < 7; i++){
		cout<< "reg" << i<< " "<< pool->regs[i]->name<<endl;
	}
	cout <<"���ʹ�ã�"<<endl;
	for(int i = 0; i < 3; i++){
		cout<< "reg" << pool->recent[i]<<endl;
	}


}