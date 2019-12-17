#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

#ifndef _PARSER_H
#define _PARSER_H


void program();
void constdec(int level);
void variabledec(int level);
void constdef(int level);
void variabledef(int level);
void funcdef();
void rfuncdef();
void parameterlist(int &paraNum);
void mainfunc();
void comStatement();
void statements();
void statement();
void ifstatement();
void condition(item &result);
void dowhilestatement();
void forstatement();
void callrfunc(item &result);
void callfunc();
void valueParaList(int &paraNum);
void readstatement();
void writestatement();
void returnstatement();
void expression(item &expResult);
void term(item &result);
void factor(item &result);
void becomestatement();

#endif // _PARSER_H
