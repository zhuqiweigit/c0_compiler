
#include <iostream>
#include "error.h"
#include "lexicon.h"
using namespace std;


int err = 0;  //纪录出错总数


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
		case 0: cout << "line:" << lineindex << " error:" << err << " 出现未知符号" << endl; break;
		case 1: cout << "line:" << lineindex << " error:" << err << " 数字长度超过限制" << endl; break;
		case 2: cout << "line:" << lineindex << " error:" << err << " 字符串" << str << "缺少\"" << endl; break;
		case 3: cout << "line:" << lineindex << " error:" << err << " char " << chr << "缺少\'" << endl; break;
		case 4: cout << "line:" << lineindex << " error:" << err << " 错误的字符" << endl; break;
		case 5: cout << "line:" << lineindex << " error:" << err << " 数字前面出现前导0" << endl; break;
		case 6: cout << "line:" << lineindex << " error:" << err << " 函数返回值部分有错误" << endl; break;
		case 7: cout << "line:" << lineindex << " error:" << err << " 缺少const " << endl; break;
		case 8: cout << "line:" << lineindex << " error:" << err << " 缺少;" << endl; break;
		case 9: cout << "line:" << lineindex << " error:" << err << " 缺少=" << endl; break;
		case 10: cout << "line:" << lineindex << " error:" << err << " 缺少数组长度" << endl; break;
		case 11: cout << "line:" << lineindex << " error:" << err << " 缺少]" << endl; break;
		case 12: cout << "line:" << lineindex << " error:" << err << " 缺少(" << endl; break;
		case 13: cout << "line:" << lineindex << " error:" << err << " 缺少)" << endl; break;
		case 14: cout << "line:" << lineindex << " error:" << err << " 缺少{" << endl; break;
		case 15: cout << "line:" << lineindex << " error:" << err << " 缺少}" << endl; break;
		case 16: cout << "line:" << lineindex << " error:" << err << " 缺少返回值符号" << endl; break;
		case 17: cout << "line:" << lineindex << " error:" << err << " 缺少标识符" << endl; break;
		case 18: cout << "line:" << lineindex << " error:" << err << " 缺少类型说明" << endl; break;
		case 19: cout << "line:" << lineindex << " error:" << err << " main函数返回值不为void"<< endl; break;
		case 20: cout << "line:" << lineindex << " error:" << err << " 缺少main" << endl; break;
		case 21: cout << "line:" << lineindex << " error:" << err << " 赋值语句错误" << endl; break;
		case 22: cout << "line:" << lineindex << " error:" << err << " 缺少do" << endl; break;
		case 23: cout << "line:" << lineindex << " error:" << err << " 缺少while" << endl; break;
		case 24: cout << "line:" << lineindex << " error:" << err << " 缺少for" << endl; break;
		case 25: cout << "line:" << lineindex << " error:" << err << " 缺少scanf" << endl; break;
		case 26: cout << "line:" << lineindex << " error:" << err << " 缺少printf" << endl; break;
		case 27: cout << "line:" << lineindex << " error:" << err << " 错误的符号" << endl; break;
		case 28: cout << "line:" << lineindex << " error:" << err << " 缺少return" << endl; break;
		case 29: cout << "line:" << lineindex << " error:" << err << " 缺少数字" << endl; break;


		case 30: cout << "line:" << lineindex << " error:" << err << " 重复定义" << endl; break;
		case 31: cout << "line:" << lineindex << " error:" << err << " 等号右边非法类型" << endl; break;
		case 32: cout << "line:" << lineindex << " error:" << err << " 0前面出现正负号" << endl; break;
		case 33: cout << "line:" << lineindex << " error:" << err << " 未初始化的常量" << endl; break;
		case 34: cout << "line:" << lineindex << " error:" << err << " 函数重复定义" << endl; break;
		case 35: cout << "line:" << lineindex << " error:" << err << " 未定义的函数或常量或变量" << endl; break;
		case 36: cout << "line:" << lineindex << " error:" << err << " 赋值对象不合法" << endl; break;
		case 37: cout << "line:" << lineindex << " error:" << err << " 非法的表达式的值" << endl; break;
		case 38: cout << "line:" << lineindex << " error:" << err << " 非法的返回值类型" << endl; break;
		case 39: cout << "line:" << lineindex << " error:" << err << " 错误地使用[]" << endl; break;
		case 40: cout << "line:" << lineindex << " error:" << err << " 非法的数组索引" << endl; break;
		case 41: cout << "line:" << lineindex << " error:" << err << " 无效的条件数据类型" << endl; break;
		case 42: cout << "line:" << lineindex << " error:" << err << " 非法的循环变量" << endl; break;
		case 43: cout << "line:" << lineindex << " error:" << err << " 非法的循环步长或缺少步长" << endl; break;
		case 44: cout << "line:" << lineindex << " error:" << err << " 无返回值的函数" << endl; break;
		case 45: cout << "line:" << lineindex << " error:" << err << " 参数不匹配" << endl; break;
		case 46: cout << "line:" << lineindex << " error:" << err << " 不应有的返回值" << endl; break;
		case 47: cout << "line:" << lineindex << " error:" << err << " 没有返回值" << endl; break;
		case 48: cout << "line:" << lineindex << " error:" << err << " 非法的因子" << endl; break;

		case 49: cout << "line:" << lineindex << " error:" << err << " 非法的符号" << endl; break;
		case 50: cout << "line:" << lineindex << " error:" << err << " 缺少if" << endl; break;
		case 51: cout << "line:" << lineindex << " error:" << err << " 缺少运算符" << endl; break;
		case 52: cout << "line:" << lineindex << " error:" << err << " 程序提前结束" << endl; break;
		case 53: cout << "line:" << lineindex << " error:" << err << " 括号不匹配或出现未知符号" << endl; break;
		case 54: cout << "line:" << lineindex << " error:" << err << " 除0" << endl; break;

    }

	}
}