#include <stdio.h>
#include <stdlib.h>


#ifndef _LEXICON_H
#define _LEXICON_H

const int al = 30; //最大标识符长度
const int nmax = 9; //允许最长的数字位数为30位
const int smax = 1000;  //允许的最长字符串常量的长度，超过后只取前1000位


typedef enum _symbolenum{
    nul, /*空,暂时用作表示不是保留字*/
    wrongsym, /*非法的字符*/

    /*操作符、符号，单双引号的处理包括在char和string处理中，故不再单独列出*/
    plus_, /* + */
    minus_, /* - */
    times, /* * */
    slash, /* / */
    lss, /* < */
    leq, /* <= */
    gtr, /* > */
    geq, /* >= */
    eeql, /* == */
    nql, /* != */
    lparen, /* ( */
    rparen, /* ) */
    lbrack, /* [ */
    rbrack, /* ] */
    lbrace, /* { */
    rbrace, /* } */
    comma, /* , */
    semicolon, /* ; */
    eql, /* = */


    /*保留字*/
    mainsym,
    ifsym,
    elsesym,
    dosym,
    whilesym,
    forsym,

    constsym,

    intsym,
    charsym,

    voidsym,
    returnsym,

    readsym,
    writesym,

    ident, /*标识符*/
    number, /*数字常量*/
    charconst, /* 'a' */
    strconst, /* "abcd "*/
	endsym  //结束符，当读到文件末尾便循环返回结束符

} symbol;


struct presym{
symbol sym;
char id[31];
long num;
char str[1001];
char chr;
int lineindex;
int err;
int endtag;
int blank;
};












/*词法分析程序对外接口变量*/
extern FILE *fp;  //源代码文件指针
extern symbol sym; //存放最近一次识别出来的符号的类型
extern char id[31];  //暂存读出的标识符的名字
extern long num; //暂存读出的数字的值
extern char str[1001]; //暂存读出的字符串的值
extern char chr; //暂存读出的char字面量
extern int lineindex;  //行号计数器，纪录当前正在分析的行号（包括了空白行）
extern int endtag; //标记是否读完整个文件
extern int blank;   //空白指示符，表示当前处理的单词的前面是否有空白符


/*词法分析函数对外接口函数*/
void getch();
void getsym();
void output(char buff[], symbol c);

/*预读部分对外接口变量*/
extern struct presym symBuffer[4];
extern int symPointer; //缓冲区指针，始终指向有效的当前符号位置

#endif
