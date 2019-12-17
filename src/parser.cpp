#include <string>
#include <iostream>
#include "parser.h"
#include "lexicon.h"
#include "symbolTable.h"
#include "Quaternary.h"
#include "error.h"

using namespace std;

  



//＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program(){
    string s("0GOLBAL0");
   symTableNew(s);//初始化符号表，并先建立一个全局符号表，
   if(sym == constsym){
      constdec(0);
   }


   if((sym == intsym || sym == charsym) && (symBuffer[(symPointer + 1) % 4].sym == ident) && (symBuffer[(symPointer + 2) % 4].sym == comma || symBuffer[(symPointer + 2) % 4].sym == semicolon || symBuffer[(symPointer + 2) % 4].sym == lbrack)){
      variabledec(0);
   }


    while((sym == intsym || sym == charsym || sym == voidsym) && symBuffer[(symPointer + 1) % 4].sym == ident ){ //&& symBuffer[(symPointer + 2) % 4].sym == lparen？
        if(sym == intsym || sym == charsym){
            rfuncdef();
        }
        else if(sym == voidsym){
            funcdef();
        }
        else{
           erro(6);//函数返回值部分有错误
        }
    }
	if(sym != voidsym){
		erro(49);
		skipUntil(1, mainSkip);
	}
    mainfunc();
	if(sym != endsym){
		erro(53); //括号不匹配	    
	}

}


void constdec(int level){
	if(sym == endsym) return;
    if(sym == constsym){
        getsym();
    }else{
        erro(7); //缺少const标志符
    }
    constdef(level);
    if(sym == semicolon)
        getsym();
  

    while(sym == constsym){
        getsym();
        constdef(level);
        if(sym == semicolon)
            getsym();
     

    }
	if(sym == endsym) return;
}

//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
void variabledec(int level){
	if(sym == endsym) return;
    variabledef(level);
    if(sym == semicolon)
        getsym();
    
    while((sym == intsym || sym == charsym ) && (symBuffer[(symPointer + 1) % 4].sym == ident) && (symBuffer[(symPointer + 2) % 4].sym == comma || symBuffer[(symPointer + 2) % 4].sym == semicolon || symBuffer[(symPointer + 2) % 4].sym == lbrack)){
        variabledef(level);
        if(sym == semicolon)
            getsym();
        
        }
	if(sym == endsym) return;
}

//＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
 //                           | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void constdef(int level){
    string n;
    long v;
	if(sym == endsym) return;
	if(sym != intsym && sym != charsym){  //检验头符号
		erro(49);
		skipUntil(5,constSkip);
	}


    if(sym == intsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
            if(sym == eql){
                getsym();
            }else{
                erro(9); //缺少等号
            }
            if(sym == plus_ || sym == minus_){
                if(sym == minus_){
                    getsym();
                    if(sym == number){
                        //此处要给数字取反
                        v = -1 * num;
                       // printf(" %d 常量定义语句\n", lineindex);
                        if(num == 0)
                            erro(32);//0前面出现正负号
                        if(findsymCur(n) == 1){
                            //出错，出现相同的定义
							erro(30);
							
                        }else{
                          constdefInsert(ints, n, v, level);
						 
                        }
                        getsym();
                    }else{
                        erro(31);//等号右边出现非法类型
                    }
                }else if(sym == plus_){
                    getsym();
                    if(sym == number){
                      //  printf(" %d 常量定义语句\n", lineindex);

                        if(num == 0)
                            erro(32);//0前面出现正负号
                        v = num;
                        if(findsymCur(n) == 1){
                            //出错，出现相同的定义
							erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
                        getsym();
                    }else{
                        erro(31);//等号右边出现非法类型

                    }

                }
            }else if(sym == number){
               // printf(" %d 常量定义语句\n", lineindex);
                v = num;
                if(findsymCur(n) == 1){
                            //出错，出现相同的定义
					erro(30);

                }else{
                    constdefInsert(ints, n, v, level);

                }
				getsym();
            }else{
                erro(31);//等号右边出现未知符号
            }
        }else{
           erro(17); //等号左边缺少标识符号
        }

		if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //跳过剩下的符号
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
            n = id;
            getsym();
            if(sym == eql){
                getsym();
            }else{
                 erro(9); //缺少等号
            }
            if(sym == plus_ || sym == minus_){
                if(sym == minus_){
                    getsym();
                    if(sym == number){
                        //此处要给数字取反
                        v = -1 * num;
                       // printf(" %d 常量定义语句\n", lineindex);
                        if(num == 0)
                             erro(32);//0前面出现正负号
                        if(findsymCur(n) == 1){
                            //出错，出现相同的定义
							erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
						getsym();
                    }else{
                        erro(33);//常量未初始化
                    }
                }else if(sym == plus_){
                    getsym();
                    if(sym == number){
                        v = num;
                      //  printf(" %d 常量定义语句\n", lineindex);
                        if(num == 0)
                            erro(32);//0前面出现正负号
                         if(findsymCur(n) == 1){
                            //出错，出现相同的定义
							 erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
						 getsym();
                    }else{
                         erro(33);//常量未初始化
                    }

                }
            }else if(sym == number){
              //  printf(" %d 常量定义语句\n", lineindex);
                v = num;
                 if(findsymCur(n) == 1){
                            //出错，出现相同的定义
					 erro(30);

                }else{
                    constdefInsert(ints, n, v, level);

                }
				 getsym();
            }else{
               erro(31);//等号右边出现未知符号

            }
        }else{
           erro(17); //等号左边缺少标识符号
         }
        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //跳过剩下的符号
			erro(49);
		}
        }

        if(sym != semicolon){
             erro(8); //缺少分号
        }

    }else if(sym == charsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
            if(sym == eql){
                getsym();
            }else{
                 erro(8); //缺少等号
            }
            if(sym == charconst){
               // printf(" %d 常量定义语句\n", lineindex);
                v = chr;
                if(findsymCur(n) == 1){
                            //出错，出现相同的定义
					erro(30);

                }else{
                    constdefInsert(chars, n, v, level);

                }
                getsym();
            }else{
                 erro(33);//常量未初始化
            }
        }else{
           erro(17);//等号左边缺少标识符号
        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //跳过剩下的符号
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
               n = id;
               getsym();
               if(sym == eql){
                   getsym();
               }else{
                   erro(9); //缺少等号
               }
               if(sym == charconst){

                  //  printf(" %d 常量定义语句\n", lineindex);
                    v = chr;
                    if(findsymCur(n) == 1){
                            //出错，出现相同的定义
						erro(30);

                    }else{
                        constdefInsert(chars, n, v, level);

                    }
                    getsym();
               }else{
                 erro(33);//常量未初始化
              }
           }else{
              erro(17); //等号左边缺少标识符号
           }
          if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //跳过剩下的符号
			erro(49);
		}
        }

        if(sym != semicolon){
           erro(8); //缺少分号
        }

    }

	if(sym == endsym) return;
}


void variabledef(int level){
    string n;
    long lth;
	if(sym == endsym) return;
	if(sym != intsym && sym != charsym){  //检验头符号
		erro(49);
		skipUntil(4,varSkip);
	}


    if(sym == intsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
           // printf(" %d 变量定义语句\n", lineindex);
        }else{
            erro(17); //缺少标识符
        }
        if(sym == comma || sym == semicolon){
            if(findsymCur(n) == 1){
                    //出错，出现相同的定义
				erro(30);

            }else{

                vardefInsert(ints, variable, n, -1, level);
            }
        }else if(sym == lbrack){
            getsym();
            if(sym == number){
                lth = num;
            if(findsymCur(n) == 1){
                    //出错，出现相同的定义
				erro(30);

            }else{

                vardefInsert(ints, arrays, n, lth, level);
            }
            getsym();
            }else{
                erro(10); //缺少数组长度
            }
            if(sym == rbrack){
                getsym();
               // printf(" %d 变量定义语句\n", lineindex);
            }else{
                erro(11);//缺少]
            }

        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //跳过剩下的符号
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
                n = id;
                getsym();

            }else{
                erro(17);//缺少标识符
            }
            if(sym == comma || sym == semicolon){
                if(findsymCur(n) == 1){
                    //出错，出现相同的定义
					erro(30);

                }else{

                    vardefInsert(ints, variable, n, -1, level);
                }

            }else if(sym == lbrack){
                getsym();
                if(sym == number){
                    lth = num;
                     if(findsymCur(n) == 1){
                    //出错，出现相同的定义
						erro(30);
                    }else{

                        vardefInsert(ints, arrays, n, lth, level);
                    }
                    getsym();
                }else{
                    erro(10); //缺少数组长度
                }
                if(sym == rbrack){
                    getsym();
                  //  printf(" %d 变量定义语句\n", lineindex);
                }else{
                    erro(11);//缺少]
                }

            }
        }
       if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //跳过剩下的符号
			erro(49);
		}

    }else if(sym == charsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
            //printf(" %d 变量定义语句\n", lineindex);  //有重复输出
        }else{
            erro(17); //缺少标识符
        }
        if(sym == comma || sym == semicolon){
             if(findsymCur(n) == 1){
                    //出错，出现相同的定义
				 erro(30);

             }else{

                vardefInsert(chars, variable, n, -1, level);
             }

        }else if(sym == lbrack){
            getsym();
            if(sym == number){
                lth = num;
                if(findsymCur(n) == 1){
                    //出错，出现相同的定义
					erro(30);

                }else{

                    vardefInsert(chars, arrays, n, lth, level);
                }
                getsym();
            }else{
                erro(10); //缺少数组长度
            }
            if(sym == rbrack){
                getsym();
               // printf(" %d 变量定义语句\n", lineindex);
            }else{
                erro(11);//缺少]
            }

        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //跳过剩下的符号
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
                n = id;
                getsym();
               // printf(" %d 变量定义语句\n", lineindex);
            }else{
                erro(17); //缺少标识符
            }
            if(sym == comma || sym == semicolon){
                if(findsymCur(n) == 1){
                    //出错，出现相同的定义
					erro(30);

                }else{

                    vardefInsert(chars, variable, n, -1, level);
                }

            }else if(sym == lbrack){
                getsym();
                if(sym == number){
                    lth = num;
                     if(findsymCur(n) == 1){
                    //出错，出现相同的定义
						 erro(30);

                    }else{

                        vardefInsert(chars, arrays, n, lth, level);
                    }
                    getsym();
                }else{
                    erro(10); //缺少数组长度
                }
                if(sym == rbrack){
                    getsym();
                   // printf(" %d 变量定义语句\n", lineindex);
                }else{
                    erro(11);//缺少]
                }

            }
        }
        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //跳过剩下的符号
			erro(49);
		}

    }
	if(sym == endsym) return;

}

void funcdef(){
    _type tp = nop;
    string n;
    int paraNum = 0;
    item obj;
	if(sym == endsym) return;
    if(sym == voidsym){
        //printf("无返回值的函数定义\n");
        tp = nop;
        getsym();
    }else{
        erro(6);//缺少void
    }
    if(sym == ident){
        n = id;
        if(findsymGol(n) || findFuncName(n)){
            //出错，函数名重复定义
			erro(34);

        }
        getsym();
    }else{
        erro(17);//缺少函数名
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12);//缺少(
    }
    obj = symTableNew(n);
	insertQuater(HEADER, NULL, NULL, obj);
    parameterlist(paraNum);
    funcInsert(paraNum, tp);
    parameterInfoInsert(paraNum);  //给形参补充参数总数信息

    if(sym == rparen){
        getsym();
    }else{
        erro(13); //缺少)
    }

    if(sym == lbrace){
        getsym();
    }else{
        erro(14); //缺少{

    }
    comStatement();
    if(sym == rbrace){
        insertQuater(FOOT, NULL, NULL, NULL);
        getsym();

    }else{
        erro(15); //缺少}
    }
	if(sym == endsym) return;
}

void rfuncdef(){
    _type tp = nop;
    string n;
    int paraNum = 0;
    item obj;
	if(sym == endsym) return;
    if(sym == intsym){
        tp = ints;
       // printf("有返回值的函数定义\n");
        getsym();
        if(sym == ident){
            n = id;
            getsym();
        }else{
           erro(17);//缺少函数名
        }
        if(sym == lparen){
            getsym();
        }else{
            erro(12);//缺少(
        }

        obj = symTableNew(n);
		insertQuater(HEADER, NULL, NULL, obj);
        parameterlist(paraNum);
        funcInsert(paraNum, tp);
        parameterInfoInsert(paraNum); //给形参补充参数总数信息
        if(sym == rparen){
            getsym();
        }else{
           erro(13);//缺少)
        }
        if(sym == lbrace){
            getsym();
        }else{
            erro(14); //缺少{

        }
        comStatement();
        if(sym == rbrace){
            insertQuater(FOOT, NULL, NULL, NULL);
            getsym();
        }else{
            erro(15); //缺少}
        }

    }else if(sym == charsym){
        tp = chars;
     //   printf("有返回值的函数定义\n");
         getsym();
        if(sym == ident){
            n = id;
            getsym();
        }else{
            erro(17);//缺少函数名
        }
        if(sym == lparen){
            getsym();
        }else{
            erro(12);//缺少(
        }

        obj = symTableNew(n);
		insertQuater(HEADER, NULL, NULL, obj);
        parameterlist(paraNum);
        funcInsert(paraNum, tp);
        parameterInfoInsert(paraNum); //给形参补充参数总数信息

        if(sym == rparen){
            getsym();
        }else{
            erro(13); //缺少)
        }
        if(sym == lbrace){
            getsym();
        }else{
            erro(14); //缺少{

        }
        comStatement();
        if(sym == rbrace){
            insertQuater(FOOT, NULL, NULL, NULL);
            getsym();
        }else{
            erro(15); //缺少}
        }

    }else{
    erro(16);//缺少返回值符号
    }
	if(sym == endsym) return;

}

void parameterlist(int &paraNum){
    _type tp = nop;
    string n;
    item obj;
	int parath = 1;
    paraNum = 0;
	if(sym == endsym) return;
   // printf("参数表\n");
    if(sym == rparen){
        return;
    }
    if(sym == intsym || sym == charsym){
        if(sym == intsym)
            tp = ints;
        if(sym == charsym)
            tp = chars;
        getsym();
    }else{
        erro(18); //参数缺少类型声明
    }
    if(sym == ident){
        n = id;
        obj = paraInsert(tp, n, parath);
        insertQuater(FORPARA, NULL, NULL, obj);
        paraNum++;
		parath++;

        getsym();

    }else{
        erro(17);//缺少标识符号
    }
    while(sym == comma){
        getsym();
        if(sym == intsym || sym == charsym){
            if(sym == intsym)
                tp = ints;
            if(sym == charsym)
                tp = chars;
            getsym();
        }else{
            erro(18); //参数缺少类型声明
        }
        if(sym == ident){
            n = id;
            obj = paraInsert(tp, n, parath);
            insertQuater(FORPARA, NULL, NULL, obj);
            paraNum++;
			parath++;
            getsym();

        }else{
            erro(17);//缺少标识符号
        }

    }
	if(sym == endsym) return;
}



void mainfunc(){
  _type tp = nop;
  string n;

  item obj;
  if(sym == endsym) return;
  if(sym == voidsym){
    tp = nop;
    getsym();
  }else{
  erro(19); //main前面缺少void
  }
  if(sym == mainsym){
    //printf("main函数\n");
    getsym();
  }else{
    erro(20);//缺少main
  }
  if(sym == lparen){
    getsym();
  }else{
    erro(12); //缺少(
  }
  if(sym == rparen){
    getsym();
  }else{
    erro(13); //缺少)
  }

  obj = symTableNew(string("main"));

  insertQuater(HEADER, NULL, NULL, obj);

  funcInsert(0, tp);

  if(sym == lbrace){
    getsym();
  }else{
    erro(14); //缺少{
  }
  comStatement();
  insertQuater(FOOT, NULL, NULL, NULL);
  if(sym == endsym){
	  erro(52);
  
  }else if(sym == rbrace){
	  getsym();
    /////////////////结束
  }else if(sym != rbrace){
	  erro(15);
  
  }

}

void comStatement(){
   // printf("复合语句\n");
	if(sym == endsym) return;
    if(sym == constsym){
      constdec(1);
    }


    if((sym == intsym || sym == charsym) && (symBuffer[(symPointer + 1) % 4].sym == ident) && (symBuffer[(symPointer + 2) % 4].sym == comma || symBuffer[(symPointer + 2) % 4].sym == semicolon || symBuffer[(symPointer + 2) % 4].sym == lbrack)){
      variabledec(1);
    }
	   
    statements();
	if(sym == endsym) return;

}

void statements(){
   // printf("语句列\n");
	if(sym == endsym) return;
	if(!(sym == ifsym || sym == dosym || sym == forsym || sym == lbrace || (sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen) || (sym == ident && (symBuffer[(symPointer + 1) % 4].sym == eql || symBuffer[(symPointer + 1) % 4].sym == lbrack))
		|| sym == readsym || sym == writesym || sym == returnsym || sym == semicolon || sym == rbrace)){
			erro(49);
		    skipUntil(9, comstate);
	
	}

	
    while(sym == ifsym || sym == dosym || sym == forsym || sym == lbrace || (sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen) || (sym == ident && (symBuffer[(symPointer + 1) % 4].sym == eql || symBuffer[(symPointer + 1) % 4].sym == lbrack))
          || sym == readsym || sym == writesym || sym == returnsym || sym == semicolon){
                statement();

     }
	if(sym == endsym) return;

}

void statement(){  //通过statement得到的有返回值的函数调用的值用在哪？？？
    string n;
    item temp1, result;
	if(sym == endsym) return;
    if(sym == ifsym){
        ifstatement();
    }else if(sym == dosym){
        dowhilestatement();
    }else if(sym == forsym){
        forstatement();
    }else if(sym == lbrace){
        getsym();
        statements();
        if(sym == rbrace){
            getsym();
        }else{
            erro(15);//缺少}
        }
    }else if(sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen){ // 函数调用
        n = id;
        temp1 =findFunc(n);
        if((temp1 != NULL &&temp1->returnV == chars) || (temp1 != NULL && temp1->returnV == ints)){
            callrfunc(result);
        }else if(temp1 != NULL && temp1->returnV == nop){
            callfunc();

        }else{
            //错误，未定义的函数
            
			erro(35);
			skipUntil(1, statementSkip);

        }

        if(sym == semicolon){
            getsym();
        }else{
         erro(8);//缺少分号
        }
    }else if(sym == ident && (symBuffer[(symPointer + 1) % 4].sym == eql || symBuffer[(symPointer + 1) % 4].sym == lbrack)){
         becomestatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8); //缺少分号
         }
    }else if(sym == readsym){
         readstatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8); //缺少分号
         }
    }else if(sym == writesym){
         writestatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8);//缺少分号
         }
    }else if(sym == semicolon){
        getsym();
       // printf("空语句\n");
    }else if(sym == returnsym){
         returnstatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8);//缺少分号
         }
    }else{
		if(sym == endsym) return;
        erro(49); //出现无法识别的语句成分
        skipUntil(1, statementSkip);
    }

}

void becomestatement(){
    item op1, obj, tempi;
    string n;
   // printf("赋值语句\n");
	if(sym == endsym) return;
    if(sym == ident){
        n = id;
        obj = findcvpa(n);
        if(obj == NULL){
            //出错，未定义的标识符
            erro(35);
			skipUntil(1, statementSkip);
            return;
        }
        if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
            //出错，赋值对象不合法（只能是变量、数组、参数）
            erro(36);
			skipUntil(1, statementSkip);
            return;
        }
        getsym();
    }else{
        erro(17); //缺少标识符
    }
    if(sym == eql){  //a = b;
        getsym();
        expression(op1);
        if(op1 == NULL || !(op1->type == cchars || op1->type == cints || op1->type == chars || op1->type == ints)){
            //出错，表达式的值非法
            erro(37);
			skipUntil(1, statementSkip);
            return;
        }
        insertQuater(ASSIGN, op1, NULL, obj);

    }else if(sym == lbrack){  //a = b[i];
        getsym();
        if(obj->kind != arrays){
            //出错，非数组名却使用[]
           erro(39);
		   skipUntil(1, statementSkip);
           return;

        }

        expression(tempi); //偏移

        if(tempi == NULL || (tempi->type != cchars && tempi->type != cints && tempi->type != ints && tempi->type != chars)){
            //出错，数组的索引为空或者类型不为以上几种
             erro(40);
			 skipUntil(1, statementSkip);
            return;

        }


        if(sym == rbrack){
            getsym();
        }else{
            erro(11);//缺少]
        }
        if(sym == eql){
            getsym();
        }else{
            erro(9); // 缺少=

        }
        expression(op1); //值
		insertQuater(WARRY, obj, tempi, op1); //op1,op2, obj 写数组操作， op1为数组， op2为偏移， obj为要写的值
        if(op1 == NULL || !(op1->type == cchars || op1->type == cints || op1->type == chars || op1->type == ints)){
            //出错，表达式的值非法
            erro(37);
			skipUntil(1, statementSkip);
            return;
        }

    }else{
		if(sym == endsym) return;
        erro(49); //赋值语句错误
		skipUntil(1, statementSkip);

    }

}
void ifstatement(){
    item op1, tagA, tagB;
	if(sym == endsym) return;
    if(sym == ifsym){
        getsym();
       // printf("if条件语句\n");
    }else{
        erro(50);//缺少ifsym
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12); //缺少(
    }
    condition(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //无效的条件数据类型
        erro(41);
        return;
    }
    tagA = tagGen();
    insertQuater(FJMP, op1, NULL, tagA); //为假跳转，FJMP, //op1, -, obj

    if(sym == rparen){
        getsym();
    }else{
        erro(12); //缺少)
    }

    statement();
    tagB = tagGen();
    insertQuater(JMP, NULL, NULL, tagB); //JMP, -, -, obj 无条件跳转
    insertQuater(TAG, NULL, NULL, tagA); //TAG, -， -， obj 标号
    if(sym == elsesym){
       // printf("else语句\n");
        getsym();
        statement();
    }
	insertQuater(TAG, NULL, NULL, tagB); //TAG, -， -， obj 标号
	if(sym == endsym) return;

}

void condition(item &result){
    symbol symtemp;
    item op1 = NULL, op2 = NULL;
    result = NULL;
    result = genTempVar(ints);
	if(sym == endsym) return;
    expression(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //出错，表达式类型错误
        erro(41);
        return;

    }


    if(sym == lss || sym == leq || sym == gtr || sym == geq || sym == eeql || sym == nql){
        symtemp = sym;
        getsym();
        expression(op2);
        if(op2 == NULL || !(op2->type == ints || op2->type == chars || op2->type == cints || op2->type == cchars)){
            //出错，表达式类型错误
            erro(41);
            return;
        }
        switch(symtemp){
            case lss: insertQuater(LES, op1, op2, result); break;
            case leq: insertQuater(LEQ, op1, op2, result); break;
            case gtr: insertQuater(GTR, op1, op2, result); break;
            case geq: insertQuater(GEQ, op1, op2, result); break;
            case eeql: insertQuater(EQL, op1, op2, result); break;
            case nql: insertQuater(NEQ, op1, op2, result); break;
            default:  break;
        }

    }else{

        insertQuater(NEQZ, op1, NULL, result);


    }
	if(sym == endsym) return;

}

void dowhilestatement(){
    item op1, tagA;
	if(sym == endsym) return;
    if(sym == dosym){
       // printf("dowhile语句\n");
        getsym();
    }else{
        erro(22); //缺少do
    }
    tagA = tagGen();  //在此处放置tagA标签
    insertQuater(TAG, NULL, NULL, tagA); //TAG, -， -， obj 标号
    statement();
    if(sym == whilesym){
        getsym();
    }else{
       erro(23);//缺少while
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12);//缺少左括号
    }
    condition(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //出错，表达式类型错误
        erro(41);
        return;

    }
    insertQuater(TJMP,op1, NULL, tagA); //条件为真则跳转
    if(sym == rparen){
        getsym();
    }else{
        erro(13);//缺少)
    }
	if(sym == endsym) return;
}

void forstatement(){
    string n;
    item tagA = tagGen();
    item tagB = tagGen();
    item tagC = tagGen();
    item tagD = tagGen();
    item op1 = NULL, op2 = NULL, obj = NULL;
    symbol symtemp;
	if(sym == endsym) return;
    if(sym == forsym){
        getsym();
       // printf("for循环语句\n");
    }else{
        erro(24); //缺少for
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12); //缺少(
    }
    if(sym == ident){
        n = id;
        getsym();
        obj = findcvpa(n);
        if(obj == NULL){
            //出错，未定义的变量
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }else if(obj->kind == constant || obj->kind == arrays){
            //出错，不合法的循环变量
			erro(42);
			skipUntil(2,statementSkip);
        }
    }else{
        erro(17); //缺少标识符
    }
    if(sym == eql){
        getsym();
    }else{
        erro(9); //缺少
    }
    expression(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //出错，表达式类型不合法
        erro(37);
		skipUntil(2,statementSkip);
        return;

    }
    insertQuater( ASSIGN, op1, NULL, obj);
    insertQuater(TAG, NULL, NULL , tagD);  //插入tagD

    if(sym == semicolon){
        getsym();
    }else{
        erro(8);//缺少分号
    }
    condition(op1);

    insertQuater(FJMP,op1, NULL,  tagA);
    insertQuater(TJMP,op1, NULL, tagB);
    insertQuater(TAG, NULL, NULL, tagC);

    if(sym == semicolon){
        getsym();
    }else{
        erro(8);//缺少分号
    }
    if(sym == ident){
        n = id;
        obj = findcvpa(n);
        if(obj == NULL){
            //出错，未定义的变量
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }else if(obj->kind == constant || obj->kind == arrays){
            //出错，赋值对象不合法
			erro(36);
        }
        getsym();
    }else{
        erro(17); //缺少标识符
    }
    if(sym == eql){
        getsym();
    }else{
        erro(9); //缺少等号
    }
    if(sym == ident){
        n = id;
        op1 = findcvpa(n);
        if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //出错，表达式不合法
        erro(37);
		skipUntil(2,statementSkip);
        return;

        }

        getsym();
    }else{
        erro(17);//缺少标识符
    }
    if(sym == plus_ || sym == minus_){
        symtemp = sym;
        getsym();
    }else{
        erro(51); //缺少运算符
    }
    if(sym == number){
        if(num == 0){
            erro(43); //步长为0
        }
        op2 = genCints(num);
        getsym();
    }else{
        erro(43);//缺少步长
    }
    if(sym == rparen){
        getsym();
    }else{
        erro(13); //缺少)
    }
    if(symtemp == plus_){
       insertQuater(ADD, op1, op2, obj);

    }else{
        insertQuater(SUB, op1, op2, obj);

    }
    insertQuater(JMP, NULL, NULL, tagD);
    insertQuater(TAG, NULL, NULL, tagB);

    statement();
    insertQuater(JMP, NULL, NULL, tagC);
    insertQuater(TAG, NULL, NULL, tagA);
	if(sym == endsym) return;
}

void callrfunc(item &result){
    string n;
    item tempi;
    int paraNum;
	if(sym == endsym) return;
   if(sym == ident){
      //  printf("有返回值的函数调用\n");
        n = id;
        tempi = findFunc(n);
        if(tempi == NULL){
            //错误，未定义的函数
            erro(35);
			skipUntil(2, statementSkip);
            return;
        }
		if(tempi->returnV == nop){
			//错误，该函数无返回值
		    erro(47);
			
		
		}
        getsym();
   }else{
    erro(17); //缺少标识符
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12);
   }
   valueParaList(paraNum);
   if(tempi->paranum != paraNum){
        //出错，参数个数不匹配
        erro(45);

   }

   result = genTempVar(tempi->returnV);
   insertQuater(RCALL, tempi, NULL, result);


   if(sym == rparen){
      getsym();
   }else{
       erro(13); //缺少)
   }
   if(sym == endsym) return;
}

void callfunc(){
    string n;
    item tempi;
    int paraNum;
	if(sym == endsym) return;
   if(sym == ident){
      //  printf("无返回值的函数调用\n");
        n = id;
        tempi = findFunc(n);
        if(tempi == NULL){
            //错误，未定义的函数
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }
        getsym();
   }else{
    erro(17); //缺少函数名
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12);
   }
   valueParaList(paraNum);
   if(paraNum != tempi->paranum){
        //出错，参数个数不匹配
        erro(45);
   }

   insertQuater(CALL, tempi, NULL, NULL);

   if(sym == rparen){
      getsym();
   }else{
       erro(13); //缺少)
   }

   if(sym == endsym) return;
}

void valueParaList(int &paraNum){  //由于只有char和int两种类型，所以暂时先不做类型匹配检查
    paraNum = 0;
    item result;
  //  printf("值参数表\n");
	if(sym == endsym) return;
     if(sym == rparen){
        return;
     }
     expression(result);
     if(result == NULL || !(result->type == ints || result->type == chars || result->type == cints || result->type == cchars)){
        //出错，表达式不合法
        erro(37);
		skipUntil(3,statementSkip);
		return;
     }
     paraNum++;
     insertQuater(ACTPARA, NULL, NULL, result);

     while(sym == comma){
        getsym();
        expression(result);
        if(result == NULL || !(result->type == ints || result->type == chars || result->type == cints || result->type == cchars)){
        //出错，表达式不合法
        erro(37);
		skipUntil(3,statementSkip);
        return;
        }
        paraNum++;
        insertQuater(ACTPARA, NULL, NULL, result);
     }

	if(sym == endsym) return;

}

void readstatement(){
    string n;
    item obj;
	if(sym == endsym) return;
   if(sym == readsym){
    //  printf("读语句\n");
      getsym();
   }else{
      erro(25);//缺少读符号
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12); //缺少(
   }
   if(sym == ident){
       n = id;
       obj =findcvpa(n);
       if(obj == NULL){
            //出错，未定义的标识符
            erro(35);
			skipUntil(3,statementSkip);
            return;
       }
       if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
        //参数不匹配
        erro(45);
        
        }
        insertQuater(READ, NULL, NULL, obj);
       getsym();
   }else{
       erro(45); //读语句缺少参数，参数不匹配
   }
   while(sym == comma){
       getsym();
       if(sym == ident){
          n = id;
          obj =findcvpa(n);
          if(obj == NULL){
            //出错，未定义的标识符
            erro(35);
			skipUntil(3,statementSkip);
            return;
          }
          if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
             //出错，读语句参数不合法，参数不匹配
             erro(45);
            return;
          }
          insertQuater(READ, NULL, NULL, obj);
          getsym();
       }else{
         erro(27); //错误的符号
       }
   }
   if(sym == rparen){
      getsym();
   }else{
      erro(13); //缺少)
   }
   if(sym == endsym) return;
}


void writestatement(){
    item obj, tempi;
    string n, ss;
	if(sym == endsym) return;
   if(sym == writesym){
    //  printf("写语句\n");
      getsym();
   }else{
      erro(26);//缺少写符号
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12); //缺少(
   }
   if(sym == strconst){
     ss = str;

     tempi = genCstring(ss);
     insertStrData(tempi);
     insertQuater(WRITE, NULL, NULL, tempi);

     getsym();
     if(sym == comma){
      getsym();
      expression(obj);
      insertQuater(WRITE, NULL, NULL, obj);
    }
     if(sym == rparen){
        getsym();
     }else{
      erro(13); //缺少)
      }

   }else {
       expression(obj);
       insertQuater(WRITE, NULL, NULL, obj);

       if(sym == rparen){
        getsym();
        }else{
        erro(13); //缺少)
        }
   }
   if(sym == endsym) return;

}

void returnstatement(){
    item obj;
    item tempi = findFuncCur();

	if(sym == endsym) return;
    if(sym == returnsym){
        getsym();
       // printf("返回语句\n");
    }else{
      erro(28);//缺少return
    }
    if(sym == lparen){
        getsym();
        expression(obj);
        //由于int和char的隐式转换，暂时不做类型匹配检查
        if(obj == NULL || !(obj->type == ints || obj->type == chars || obj->type == cints || obj->type == cchars)){
        //出错，非法的返回值类型
			erro(38);
			skipUntil(2,statementSkip);
			return;
        }
        if(tempi->returnV == nop){
            //出错，不该有的返回值
            erro(46);
            
        }

        insertQuater(RETN,NULL,NULL, obj);

        if(sym == rparen){
            getsym();
        }else{
            erro(13);//缺少)
        }
    }else{
        if(tempi->returnV != nop){
            //出错，没有返回值
            erro(47);
            
        }
        insertQuater(NRETN, NULL, NULL, NULL);

    }

	if(sym == endsym) return;

}

void expression(item &expResult){
    symbol symtemp = wrongsym;
    item termResult, tempi;
	if(sym == endsym) return;
    if(sym == plus_ || sym == minus_){
        symtemp = sym;
        getsym();

    }

    term(termResult);
    if(symtemp == plus_){
        expResult = termResult;
    }else if(symtemp == minus_){
        expResult = genTempVar(termResult->type);
        insertQuater(NEG, termResult, NULL, expResult);
	}else{
		expResult = termResult;	
	}


    while(sym == plus_ || sym == minus_){
        symtemp = sym;
        getsym();
        term(termResult);
        if(symtemp == plus_){
            
			/*
            if((expResult->type == cchars || expResult->type == chars) &&  (termResult->type == cchars || termResult->type == chars)){
                    //只有当两者都是char时结果才是char，只要有int结果就为int
                    //另外， 'c'+'c'变为chars而非cchars是否正确？？？
                expResult = genTempVar(chars);
                insertQuater(ADD, tempi, termResult, expResult);
            }else {
                expResult = genTempVar(ints);
                insertQuater(ADD, tempi, termResult, expResult);
            }
			*/
			if(expResult->kind == constant && termResult->kind == constant){  //都是常量时，做常量合并
				int add1, add2, addV;
				if(expResult->type == ints || expResult->type == cints)
					add1 = expResult->valueInt;
				else if(expResult->type == chars || expResult->type == cchars)
					add1 = expResult->valueChar;

				if(termResult->type == ints || termResult->type == cints)
					add2 = termResult->valueInt;
				else if(termResult->type == chars || termResult->type == cchars)
					add2 = termResult->valueChar;

				addV = add1 + add2;
				expResult = genCints(addV);
			
			}else{        //否则做运算
				tempi = expResult;
				expResult = genTempVar(ints);   //改为只要char参与运算就会被转为int，只有不参与运算的cahr才是char
				insertQuater(ADD, tempi, termResult, expResult);
			}
			



        }else if(symtemp == minus_){
           
			/*
            if((expResult->type == cchars || expResult->type == chars) &&  (termResult->type == cchars || termResult->type == chars)){
                    //只有当两者都是char时结果才是char，只要有int结果就为int
                    //另外， 'c'+'c'变为chars而非cchars是否正确？？？
                expResult = genTempVar(chars);
                insertQuater(SUB, tempi, termResult, expResult);
            }else {
                expResult = genTempVar(ints);
                insertQuater(SUB, tempi, termResult, expResult);
            }
			*/
			if(expResult->kind == constant && termResult->kind == constant){  //都是常量时，做常量合并
				int sub1, sub2, subV;
				if(expResult->type == ints || expResult->type == cints)
					sub1 = expResult->valueInt;
				else if(expResult->type == chars || expResult->type == cchars)
					sub1 = expResult->valueChar;

				if(termResult->type == ints || termResult->type == cints)
					sub2 = termResult->valueInt;
				else if(termResult->type == chars || termResult->type == cchars)
					sub2 = termResult->valueChar;

				subV = sub1 - sub2;
				expResult = genCints(subV);
			
			}else{
				tempi = expResult;
				expResult = genTempVar(ints);
				insertQuater(SUB, tempi, termResult, expResult);
			}

        }
    }
	if(sym == endsym) return;
}

void term(item &result){
    symbol symtemp;
    item factResult, tempi;

	if(sym == endsym) return;
    factor(factResult);
    result = factResult;
    while(sym == times || sym == slash){
        symtemp = sym;
        getsym();
        factor(factResult);
        if(symtemp == times){
            
			/*
            if((result->type == chars || result->type == cchars) && (factResult->type == chars || factResult->type == cchars)){
                result = genTempVar(chars);
                insertQuater(MUL, tempi, factResult, result);
            }else{
                result = genTempVar(ints);
                insertQuater(MUL, tempi, factResult, result);

            }  */
			if(result->kind == constant && factResult->kind == constant){  //常量合并
				int times1, times2, timesV;
				if(result->type == ints || result->type == cints)
					times1 = result->valueInt;
				else if(result->type == chars || result->type == cchars)
					times1 = result->valueChar;
				if(factResult->type == ints || factResult->type == cints)
					times2 = factResult->valueInt;
				else if(factResult->type == chars || factResult->type == cchars)
					times2 = factResult->valueChar;

				timesV = times1 * times2;
				result = genCints(timesV);

			
			}else{  //不能合并则做运算
				tempi = result;
				result = genTempVar(ints);
				insertQuater(MUL, tempi, factResult, result);
			}

        }else{
			/*
            if((result->type == chars || result->type == cchars) && (factResult->type == chars || factResult->type ==cchars)){
                result = genTempVar(chars);
                insertQuater(DIV, tempi, factResult, result);
            }else{
                result = genTempVar(ints);
                insertQuater(DIV, tempi, factResult, result);

            } */
			if(result->kind == constant && factResult->kind == constant){  //常量合并
				int div1, div2, divV;
				if(result->type == ints || result->type == cints)
					div1 = result->valueInt;
				else if(result->type == chars || result->type == cchars)
					div1 = result->valueChar;
				if(factResult->type == ints || factResult->type == cints)
					div2 = factResult->valueInt;
				else if(factResult->type == chars || factResult->type == cchars)
					div2 = factResult->valueChar;

				if(div2 != 0){   //检查是否除零
					divV = div1 / div2;  
				}else{
					divV = div1 / 1;
					erro(54);
				}
				result = genCints(divV);
			}else{
				tempi = result;
				result = genTempVar(ints);
				insertQuater(DIV, tempi, factResult, result);
			}

        }
    }
	if(sym == endsym) return;
}

void factor(item &result){
    item  obj, tempi, tempvar;
    string n;
    result = NULL;
	if(sym == endsym) return;
    if(sym == ident && symBuffer[(symPointer + 1) % 4].sym == lbrack){
        n = id;
        obj = findcvpa(n);
        if(obj == NULL){
            //出错，未定义的变量
            erro(35);
			skipUntil(8,factorSkip);
            return;

        }
        if(obj->kind != arrays){
            //出错，非数组元素使用了[]
            erro(39);

        }
        getsym();
        getsym();
     //   printf("因子\n");

        expression(tempi);
        if(tempi == NULL || (tempi->type != cchars && tempi->type != cints && tempi->type != ints && tempi->type != chars)){
            //出错，非法的数组的索引
            erro(6);
            return;

        }


		tempvar = genTempVar(obj->type);
		insertQuater(RARRY, obj, tempi, tempvar); //生成取数组元素的中间代码
		result = tempvar;

        if(sym == rbrack){
            getsym();
        }else{
            erro(11); //缺少]
        }

    }else if(sym == ident &&  symBuffer[(symPointer + 1) % 4].sym != lparen){
        n = id;
        result = findcvpa(n);
        if(result == NULL){
            //错误，未定义的变量
            erro(35);
			skipUntil(8, factorSkip);
            return;
        }
        if(!(result->kind == constant || result->kind == variable || result->kind == parameter)){
            //错误，非法的因子
            erro(48);
            return;
        }
        getsym();

   //     printf("因子\n");

    }else if(sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen){
   //     printf("因子\n");
        callrfunc(result);

    }else if(sym == plus_ || sym == minus_ || sym == number){
        if(sym == plus_){
            getsym();
            if(sym == number){
         //       printf("因子\n");
                if(num == 0)
                   erro(32); //0前面不能有正负号
                tempi = genCints(num);
                result = tempi;
                getsym();
            }else{
                erro(29); //缺少数字
            }

        }else if(sym == minus_){
            getsym();
            if(sym == number){
       //         printf("因子\n");
                if(num == 0)
                    erro(32);//0前面不能有正负号
                tempi = genCints(num * -1);
				result = tempi;
                //result =genTempVar(ints);
                //insertQuater(NEG, tempi, NULL, result);
                getsym();
                }else{
                  erro(29);//缺少数字
				  tempi = genCints(0); //随便产生一个数字即可
                  result =genTempVar(ints);
                }
        }else if(sym == number){
       //      printf("因子\n");
             result = genCints(num);
             getsym();

        }

    }else if(sym == charconst){
        result = genChars(chr);
        getsym();
      //  printf("因子");

    }else if(sym == lparen){
        getsym();
        expression(result);
        if(sym == rparen){
            getsym();

        }else{
          erro(13);//缺少右括号
        }

    }else{
      erro(49);//错误的语法
    }

	if(sym == endsym) return;
}





