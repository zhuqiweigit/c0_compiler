#include <stdio.h>
#include <stdlib.h>


#ifndef _LEXICON_H
#define _LEXICON_H

const int al = 30; //����ʶ������
const int nmax = 9; //�����������λ��Ϊ30λ
const int smax = 1000;  //�������ַ��������ĳ��ȣ�������ֻȡǰ1000λ


typedef enum _symbolenum{
    nul, /*��,��ʱ������ʾ���Ǳ�����*/
    wrongsym, /*�Ƿ����ַ�*/

    /*�����������ţ���˫���ŵĴ��������char��string�����У��ʲ��ٵ����г�*/
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


    /*������*/
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

    ident, /*��ʶ��*/
    number, /*���ֳ���*/
    charconst, /* 'a' */
    strconst, /* "abcd "*/
	endsym  //���������������ļ�ĩβ��ѭ�����ؽ�����

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












/*�ʷ������������ӿڱ���*/
extern FILE *fp;  //Դ�����ļ�ָ��
extern symbol sym; //������һ��ʶ������ķ��ŵ�����
extern char id[31];  //�ݴ�����ı�ʶ��������
extern long num; //�ݴ���������ֵ�ֵ
extern char str[1001]; //�ݴ�������ַ�����ֵ
extern char chr; //�ݴ������char������
extern int lineindex;  //�кż���������¼��ǰ���ڷ������кţ������˿հ��У�
extern int endtag; //����Ƿ���������ļ�
extern int blank;   //�հ�ָʾ������ʾ��ǰ����ĵ��ʵ�ǰ���Ƿ��пհ׷�


/*�ʷ�������������ӿں���*/
void getch();
void getsym();
void output(char buff[], symbol c);

/*Ԥ�����ֶ���ӿڱ���*/
extern struct presym symBuffer[4];
extern int symPointer; //������ָ�룬ʼ��ָ����Ч�ĵ�ǰ����λ��

#endif
