#ifndef _MIPSCODE_H
#define _MIPSCODE_H

typedef class class_regs_pool{
public:
	int regPointer;  //ָ��,ʼ��ָ��	
	item regs[7];  //��ʱ�Ĵ����أ�t0-t6
	int recent[3];  //��������ù��������Ĵ���
	int rept;  //ָ�룬ָ�������ù��ļĴ�����recent

}REGS_POOL;

typedef REGS_POOL* regs_pool;


extern vector<string> codes;
void fetchcode();
void printcode();
void printPool();
#endif