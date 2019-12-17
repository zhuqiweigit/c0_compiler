#ifndef _MIPSCODE_H
#define _MIPSCODE_H

typedef class class_regs_pool{
public:
	int regPointer;  //指针,始终指向	
	item regs[7];  //临时寄存器池，t0-t6
	int recent[3];  //保存最近用过的三个寄存器
	int rept;  //指针，指向最新用过的寄存器的recent

}REGS_POOL;

typedef REGS_POOL* regs_pool;


extern vector<string> codes;
void fetchcode();
void printcode();
void printPool();
#endif