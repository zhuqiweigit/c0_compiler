#include "lexicon.h"
#ifndef _ERROR_H
#define _ERROR_H


extern symbol varSkip[4];
extern symbol constSkip[5];
extern symbol mainSkip[1] ;
extern symbol statementSkip[3];
extern symbol factorSkip[8];
extern symbol comstate[9];


extern int err;  //纪录出错总数
void erro(int errcode);
void skipUntil(int i, symbol a[]);

#endif