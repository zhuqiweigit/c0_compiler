
#include <iostream>
#include "error.h"
#include "lexicon.h"
using namespace std;


int err = 0;  //��¼��������


symbol varSkip[4] = {intsym, charsym, comma, semicolon}, constSkip[5] = {intsym, charsym, comma, semicolon, constsym};
symbol mainSkip[1] = { mainsym};
symbol statementSkip[3] = {semicolon, lbrace, comma};
symbol factorSkip[8] = {plus_, minus_, ident, lparen, number, charconst,comma,semicolon};
symbol comstate[9] = {ifsym, dosym, forsym, lbrace, lparen, readsym, writesym, returnsym ,semicolon};





int exist(int i, symbol a[], symbol b){
   int j = 0, tag = 0;
   for(j = 0; j < i; j++){
      if(a[j] == b){
        tag = 1;
        break;
      }
   }
   return tag;
}

void skipUntil(int i, symbol a[]){
  //  int flag = 0;
    while(!exist(i, a, sym)){
        getsym();
       // flag = 1;
	    if(sym == endsym) return;
    }

}








void erro(int errcode){
    err++;
	if(err <= 100){
    switch(errcode){
		case 0: cout << "line:" << lineindex << " error:" << err << " ����δ֪����" << endl; break;
		case 1: cout << "line:" << lineindex << " error:" << err << " ���ֳ��ȳ�������" << endl; break;
		case 2: cout << "line:" << lineindex << " error:" << err << " �ַ���" << str << "ȱ��\"" << endl; break;
		case 3: cout << "line:" << lineindex << " error:" << err << " char " << chr << "ȱ��\'" << endl; break;
		case 4: cout << "line:" << lineindex << " error:" << err << " ������ַ�" << endl; break;
		case 5: cout << "line:" << lineindex << " error:" << err << " ����ǰ�����ǰ��0" << endl; break;
		case 6: cout << "line:" << lineindex << " error:" << err << " ��������ֵ�����д���" << endl; break;
		case 7: cout << "line:" << lineindex << " error:" << err << " ȱ��const " << endl; break;
		case 8: cout << "line:" << lineindex << " error:" << err << " ȱ��;" << endl; break;
		case 9: cout << "line:" << lineindex << " error:" << err << " ȱ��=" << endl; break;
		case 10: cout << "line:" << lineindex << " error:" << err << " ȱ�����鳤��" << endl; break;
		case 11: cout << "line:" << lineindex << " error:" << err << " ȱ��]" << endl; break;
		case 12: cout << "line:" << lineindex << " error:" << err << " ȱ��(" << endl; break;
		case 13: cout << "line:" << lineindex << " error:" << err << " ȱ��)" << endl; break;
		case 14: cout << "line:" << lineindex << " error:" << err << " ȱ��{" << endl; break;
		case 15: cout << "line:" << lineindex << " error:" << err << " ȱ��}" << endl; break;
		case 16: cout << "line:" << lineindex << " error:" << err << " ȱ�ٷ���ֵ����" << endl; break;
		case 17: cout << "line:" << lineindex << " error:" << err << " ȱ�ٱ�ʶ��" << endl; break;
		case 18: cout << "line:" << lineindex << " error:" << err << " ȱ������˵��" << endl; break;
		case 19: cout << "line:" << lineindex << " error:" << err << " main��������ֵ��Ϊvoid"<< endl; break;
		case 20: cout << "line:" << lineindex << " error:" << err << " ȱ��main" << endl; break;
		case 21: cout << "line:" << lineindex << " error:" << err << " ��ֵ������" << endl; break;
		case 22: cout << "line:" << lineindex << " error:" << err << " ȱ��do" << endl; break;
		case 23: cout << "line:" << lineindex << " error:" << err << " ȱ��while" << endl; break;
		case 24: cout << "line:" << lineindex << " error:" << err << " ȱ��for" << endl; break;
		case 25: cout << "line:" << lineindex << " error:" << err << " ȱ��scanf" << endl; break;
		case 26: cout << "line:" << lineindex << " error:" << err << " ȱ��printf" << endl; break;
		case 27: cout << "line:" << lineindex << " error:" << err << " ����ķ���" << endl; break;
		case 28: cout << "line:" << lineindex << " error:" << err << " ȱ��return" << endl; break;
		case 29: cout << "line:" << lineindex << " error:" << err << " ȱ������" << endl; break;


		case 30: cout << "line:" << lineindex << " error:" << err << " �ظ�����" << endl; break;
		case 31: cout << "line:" << lineindex << " error:" << err << " �Ⱥ��ұ߷Ƿ�����" << endl; break;
		case 32: cout << "line:" << lineindex << " error:" << err << " 0ǰ�����������" << endl; break;
		case 33: cout << "line:" << lineindex << " error:" << err << " δ��ʼ���ĳ���" << endl; break;
		case 34: cout << "line:" << lineindex << " error:" << err << " �����ظ�����" << endl; break;
		case 35: cout << "line:" << lineindex << " error:" << err << " δ����ĺ������������" << endl; break;
		case 36: cout << "line:" << lineindex << " error:" << err << " ��ֵ���󲻺Ϸ�" << endl; break;
		case 37: cout << "line:" << lineindex << " error:" << err << " �Ƿ��ı��ʽ��ֵ" << endl; break;
		case 38: cout << "line:" << lineindex << " error:" << err << " �Ƿ��ķ���ֵ����" << endl; break;
		case 39: cout << "line:" << lineindex << " error:" << err << " �����ʹ��[]" << endl; break;
		case 40: cout << "line:" << lineindex << " error:" << err << " �Ƿ�����������" << endl; break;
		case 41: cout << "line:" << lineindex << " error:" << err << " ��Ч��������������" << endl; break;
		case 42: cout << "line:" << lineindex << " error:" << err << " �Ƿ���ѭ������" << endl; break;
		case 43: cout << "line:" << lineindex << " error:" << err << " �Ƿ���ѭ��������ȱ�ٲ���" << endl; break;
		case 44: cout << "line:" << lineindex << " error:" << err << " �޷���ֵ�ĺ���" << endl; break;
		case 45: cout << "line:" << lineindex << " error:" << err << " ������ƥ��" << endl; break;
		case 46: cout << "line:" << lineindex << " error:" << err << " ��Ӧ�еķ���ֵ" << endl; break;
		case 47: cout << "line:" << lineindex << " error:" << err << " û�з���ֵ" << endl; break;
		case 48: cout << "line:" << lineindex << " error:" << err << " �Ƿ�������" << endl; break;

		case 49: cout << "line:" << lineindex << " error:" << err << " �Ƿ��ķ���" << endl; break;
		case 50: cout << "line:" << lineindex << " error:" << err << " ȱ��if" << endl; break;
		case 51: cout << "line:" << lineindex << " error:" << err << " ȱ�������" << endl; break;
		case 52: cout << "line:" << lineindex << " error:" << err << " ������ǰ����" << endl; break;
		case 53: cout << "line:" << lineindex << " error:" << err << " ���Ų�ƥ������δ֪����" << endl; break;
		case 54: cout << "line:" << lineindex << " error:" << err << " ��0" << endl; break;

    }

	}
}