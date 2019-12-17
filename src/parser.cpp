#include <string>
#include <iostream>
#include "parser.h"
#include "lexicon.h"
#include "symbolTable.h"
#include "Quaternary.h"
#include "error.h"

using namespace std;

  



//������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program(){
    string s("0GOLBAL0");
   symTableNew(s);//��ʼ�����ű����Ƚ���һ��ȫ�ַ��ű�
   if(sym == constsym){
      constdec(0);
   }


   if((sym == intsym || sym == charsym) && (symBuffer[(symPointer + 1) % 4].sym == ident) && (symBuffer[(symPointer + 2) % 4].sym == comma || symBuffer[(symPointer + 2) % 4].sym == semicolon || symBuffer[(symPointer + 2) % 4].sym == lbrack)){
      variabledec(0);
   }


    while((sym == intsym || sym == charsym || sym == voidsym) && symBuffer[(symPointer + 1) % 4].sym == ident ){ //&& symBuffer[(symPointer + 2) % 4].sym == lparen��
        if(sym == intsym || sym == charsym){
            rfuncdef();
        }
        else if(sym == voidsym){
            funcdef();
        }
        else{
           erro(6);//��������ֵ�����д���
        }
    }
	if(sym != voidsym){
		erro(49);
		skipUntil(1, mainSkip);
	}
    mainfunc();
	if(sym != endsym){
		erro(53); //���Ų�ƥ��	    
	}

}


void constdec(int level){
	if(sym == endsym) return;
    if(sym == constsym){
        getsym();
    }else{
        erro(7); //ȱ��const��־��
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

//������˵����  ::= ���������壾;{���������壾;}
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

//���������壾   ::=   int����ʶ��������������{,����ʶ��������������}
 //                           | char����ʶ���������ַ���{,����ʶ���������ַ���}
void constdef(int level){
    string n;
    long v;
	if(sym == endsym) return;
	if(sym != intsym && sym != charsym){  //����ͷ����
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
                erro(9); //ȱ�ٵȺ�
            }
            if(sym == plus_ || sym == minus_){
                if(sym == minus_){
                    getsym();
                    if(sym == number){
                        //�˴�Ҫ������ȡ��
                        v = -1 * num;
                       // printf(" %d �����������\n", lineindex);
                        if(num == 0)
                            erro(32);//0ǰ�����������
                        if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
							erro(30);
							
                        }else{
                          constdefInsert(ints, n, v, level);
						 
                        }
                        getsym();
                    }else{
                        erro(31);//�Ⱥ��ұ߳��ַǷ�����
                    }
                }else if(sym == plus_){
                    getsym();
                    if(sym == number){
                      //  printf(" %d �����������\n", lineindex);

                        if(num == 0)
                            erro(32);//0ǰ�����������
                        v = num;
                        if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
							erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
                        getsym();
                    }else{
                        erro(31);//�Ⱥ��ұ߳��ַǷ�����

                    }

                }
            }else if(sym == number){
               // printf(" %d �����������\n", lineindex);
                v = num;
                if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
					erro(30);

                }else{
                    constdefInsert(ints, n, v, level);

                }
				getsym();
            }else{
                erro(31);//�Ⱥ��ұ߳���δ֪����
            }
        }else{
           erro(17); //�Ⱥ����ȱ�ٱ�ʶ����
        }

		if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //����ʣ�µķ���
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
                 erro(9); //ȱ�ٵȺ�
            }
            if(sym == plus_ || sym == minus_){
                if(sym == minus_){
                    getsym();
                    if(sym == number){
                        //�˴�Ҫ������ȡ��
                        v = -1 * num;
                       // printf(" %d �����������\n", lineindex);
                        if(num == 0)
                             erro(32);//0ǰ�����������
                        if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
							erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
						getsym();
                    }else{
                        erro(33);//����δ��ʼ��
                    }
                }else if(sym == plus_){
                    getsym();
                    if(sym == number){
                        v = num;
                      //  printf(" %d �����������\n", lineindex);
                        if(num == 0)
                            erro(32);//0ǰ�����������
                         if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
							 erro(30);

                        }else{
                          constdefInsert(ints, n, v, level);

                        }
						 getsym();
                    }else{
                         erro(33);//����δ��ʼ��
                    }

                }
            }else if(sym == number){
              //  printf(" %d �����������\n", lineindex);
                v = num;
                 if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
					 erro(30);

                }else{
                    constdefInsert(ints, n, v, level);

                }
				 getsym();
            }else{
               erro(31);//�Ⱥ��ұ߳���δ֪����

            }
        }else{
           erro(17); //�Ⱥ����ȱ�ٱ�ʶ����
         }
        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //����ʣ�µķ���
			erro(49);
		}
        }

        if(sym != semicolon){
             erro(8); //ȱ�ٷֺ�
        }

    }else if(sym == charsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
            if(sym == eql){
                getsym();
            }else{
                 erro(8); //ȱ�ٵȺ�
            }
            if(sym == charconst){
               // printf(" %d �����������\n", lineindex);
                v = chr;
                if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
					erro(30);

                }else{
                    constdefInsert(chars, n, v, level);

                }
                getsym();
            }else{
                 erro(33);//����δ��ʼ��
            }
        }else{
           erro(17);//�Ⱥ����ȱ�ٱ�ʶ����
        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //����ʣ�µķ���
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
                   erro(9); //ȱ�ٵȺ�
               }
               if(sym == charconst){

                  //  printf(" %d �����������\n", lineindex);
                    v = chr;
                    if(findsymCur(n) == 1){
                            //����������ͬ�Ķ���
						erro(30);

                    }else{
                        constdefInsert(chars, n, v, level);

                    }
                    getsym();
               }else{
                 erro(33);//����δ��ʼ��
              }
           }else{
              erro(17); //�Ⱥ����ȱ�ٱ�ʶ����
           }
          if(sym != intsym && sym != charsym && sym != comma && sym != semicolon && sym != constsym){
			skipUntil(5,constSkip);  //����ʣ�µķ���
			erro(49);
		}
        }

        if(sym != semicolon){
           erro(8); //ȱ�ٷֺ�
        }

    }

	if(sym == endsym) return;
}


void variabledef(int level){
    string n;
    long lth;
	if(sym == endsym) return;
	if(sym != intsym && sym != charsym){  //����ͷ����
		erro(49);
		skipUntil(4,varSkip);
	}


    if(sym == intsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
           // printf(" %d �����������\n", lineindex);
        }else{
            erro(17); //ȱ�ٱ�ʶ��
        }
        if(sym == comma || sym == semicolon){
            if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
				erro(30);

            }else{

                vardefInsert(ints, variable, n, -1, level);
            }
        }else if(sym == lbrack){
            getsym();
            if(sym == number){
                lth = num;
            if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
				erro(30);

            }else{

                vardefInsert(ints, arrays, n, lth, level);
            }
            getsym();
            }else{
                erro(10); //ȱ�����鳤��
            }
            if(sym == rbrack){
                getsym();
               // printf(" %d �����������\n", lineindex);
            }else{
                erro(11);//ȱ��]
            }

        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //����ʣ�µķ���
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
                n = id;
                getsym();

            }else{
                erro(17);//ȱ�ٱ�ʶ��
            }
            if(sym == comma || sym == semicolon){
                if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
					erro(30);

                }else{

                    vardefInsert(ints, variable, n, -1, level);
                }

            }else if(sym == lbrack){
                getsym();
                if(sym == number){
                    lth = num;
                     if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
						erro(30);
                    }else{

                        vardefInsert(ints, arrays, n, lth, level);
                    }
                    getsym();
                }else{
                    erro(10); //ȱ�����鳤��
                }
                if(sym == rbrack){
                    getsym();
                  //  printf(" %d �����������\n", lineindex);
                }else{
                    erro(11);//ȱ��]
                }

            }
        }
       if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //����ʣ�µķ���
			erro(49);
		}

    }else if(sym == charsym){
        getsym();
        if(sym == ident){
            n = id;
            getsym();
            //printf(" %d �����������\n", lineindex);  //���ظ����
        }else{
            erro(17); //ȱ�ٱ�ʶ��
        }
        if(sym == comma || sym == semicolon){
             if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
				 erro(30);

             }else{

                vardefInsert(chars, variable, n, -1, level);
             }

        }else if(sym == lbrack){
            getsym();
            if(sym == number){
                lth = num;
                if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
					erro(30);

                }else{

                    vardefInsert(chars, arrays, n, lth, level);
                }
                getsym();
            }else{
                erro(10); //ȱ�����鳤��
            }
            if(sym == rbrack){
                getsym();
               // printf(" %d �����������\n", lineindex);
            }else{
                erro(11);//ȱ��]
            }

        }

        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //����ʣ�µķ���
			erro(49);
		}

        while(sym == comma){
            getsym();
            if(sym == ident){
                n = id;
                getsym();
               // printf(" %d �����������\n", lineindex);
            }else{
                erro(17); //ȱ�ٱ�ʶ��
            }
            if(sym == comma || sym == semicolon){
                if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
					erro(30);

                }else{

                    vardefInsert(chars, variable, n, -1, level);
                }

            }else if(sym == lbrack){
                getsym();
                if(sym == number){
                    lth = num;
                     if(findsymCur(n) == 1){
                    //����������ͬ�Ķ���
						 erro(30);

                    }else{

                        vardefInsert(chars, arrays, n, lth, level);
                    }
                    getsym();
                }else{
                    erro(10); //ȱ�����鳤��
                }
                if(sym == rbrack){
                    getsym();
                   // printf(" %d �����������\n", lineindex);
                }else{
                    erro(11);//ȱ��]
                }

            }
        }
        if(sym != intsym && sym != charsym && sym != comma && sym != semicolon ){
			skipUntil(4,varSkip);  //����ʣ�µķ���
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
        //printf("�޷���ֵ�ĺ�������\n");
        tp = nop;
        getsym();
    }else{
        erro(6);//ȱ��void
    }
    if(sym == ident){
        n = id;
        if(findsymGol(n) || findFuncName(n)){
            //�����������ظ�����
			erro(34);

        }
        getsym();
    }else{
        erro(17);//ȱ�ٺ�����
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12);//ȱ��(
    }
    obj = symTableNew(n);
	insertQuater(HEADER, NULL, NULL, obj);
    parameterlist(paraNum);
    funcInsert(paraNum, tp);
    parameterInfoInsert(paraNum);  //���ββ������������Ϣ

    if(sym == rparen){
        getsym();
    }else{
        erro(13); //ȱ��)
    }

    if(sym == lbrace){
        getsym();
    }else{
        erro(14); //ȱ��{

    }
    comStatement();
    if(sym == rbrace){
        insertQuater(FOOT, NULL, NULL, NULL);
        getsym();

    }else{
        erro(15); //ȱ��}
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
       // printf("�з���ֵ�ĺ�������\n");
        getsym();
        if(sym == ident){
            n = id;
            getsym();
        }else{
           erro(17);//ȱ�ٺ�����
        }
        if(sym == lparen){
            getsym();
        }else{
            erro(12);//ȱ��(
        }

        obj = symTableNew(n);
		insertQuater(HEADER, NULL, NULL, obj);
        parameterlist(paraNum);
        funcInsert(paraNum, tp);
        parameterInfoInsert(paraNum); //���ββ������������Ϣ
        if(sym == rparen){
            getsym();
        }else{
           erro(13);//ȱ��)
        }
        if(sym == lbrace){
            getsym();
        }else{
            erro(14); //ȱ��{

        }
        comStatement();
        if(sym == rbrace){
            insertQuater(FOOT, NULL, NULL, NULL);
            getsym();
        }else{
            erro(15); //ȱ��}
        }

    }else if(sym == charsym){
        tp = chars;
     //   printf("�з���ֵ�ĺ�������\n");
         getsym();
        if(sym == ident){
            n = id;
            getsym();
        }else{
            erro(17);//ȱ�ٺ�����
        }
        if(sym == lparen){
            getsym();
        }else{
            erro(12);//ȱ��(
        }

        obj = symTableNew(n);
		insertQuater(HEADER, NULL, NULL, obj);
        parameterlist(paraNum);
        funcInsert(paraNum, tp);
        parameterInfoInsert(paraNum); //���ββ������������Ϣ

        if(sym == rparen){
            getsym();
        }else{
            erro(13); //ȱ��)
        }
        if(sym == lbrace){
            getsym();
        }else{
            erro(14); //ȱ��{

        }
        comStatement();
        if(sym == rbrace){
            insertQuater(FOOT, NULL, NULL, NULL);
            getsym();
        }else{
            erro(15); //ȱ��}
        }

    }else{
    erro(16);//ȱ�ٷ���ֵ����
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
   // printf("������\n");
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
        erro(18); //����ȱ����������
    }
    if(sym == ident){
        n = id;
        obj = paraInsert(tp, n, parath);
        insertQuater(FORPARA, NULL, NULL, obj);
        paraNum++;
		parath++;

        getsym();

    }else{
        erro(17);//ȱ�ٱ�ʶ����
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
            erro(18); //����ȱ����������
        }
        if(sym == ident){
            n = id;
            obj = paraInsert(tp, n, parath);
            insertQuater(FORPARA, NULL, NULL, obj);
            paraNum++;
			parath++;
            getsym();

        }else{
            erro(17);//ȱ�ٱ�ʶ����
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
  erro(19); //mainǰ��ȱ��void
  }
  if(sym == mainsym){
    //printf("main����\n");
    getsym();
  }else{
    erro(20);//ȱ��main
  }
  if(sym == lparen){
    getsym();
  }else{
    erro(12); //ȱ��(
  }
  if(sym == rparen){
    getsym();
  }else{
    erro(13); //ȱ��)
  }

  obj = symTableNew(string("main"));

  insertQuater(HEADER, NULL, NULL, obj);

  funcInsert(0, tp);

  if(sym == lbrace){
    getsym();
  }else{
    erro(14); //ȱ��{
  }
  comStatement();
  insertQuater(FOOT, NULL, NULL, NULL);
  if(sym == endsym){
	  erro(52);
  
  }else if(sym == rbrace){
	  getsym();
    /////////////////����
  }else if(sym != rbrace){
	  erro(15);
  
  }

}

void comStatement(){
   // printf("�������\n");
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
   // printf("�����\n");
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

void statement(){  //ͨ��statement�õ����з���ֵ�ĺ������õ�ֵ�����ģ�����
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
            erro(15);//ȱ��}
        }
    }else if(sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen){ // ��������
        n = id;
        temp1 =findFunc(n);
        if((temp1 != NULL &&temp1->returnV == chars) || (temp1 != NULL && temp1->returnV == ints)){
            callrfunc(result);
        }else if(temp1 != NULL && temp1->returnV == nop){
            callfunc();

        }else{
            //����δ����ĺ���
            
			erro(35);
			skipUntil(1, statementSkip);

        }

        if(sym == semicolon){
            getsym();
        }else{
         erro(8);//ȱ�ٷֺ�
        }
    }else if(sym == ident && (symBuffer[(symPointer + 1) % 4].sym == eql || symBuffer[(symPointer + 1) % 4].sym == lbrack)){
         becomestatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8); //ȱ�ٷֺ�
         }
    }else if(sym == readsym){
         readstatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8); //ȱ�ٷֺ�
         }
    }else if(sym == writesym){
         writestatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8);//ȱ�ٷֺ�
         }
    }else if(sym == semicolon){
        getsym();
       // printf("�����\n");
    }else if(sym == returnsym){
         returnstatement();
         if(sym == semicolon){
            getsym();
         }else{
            erro(8);//ȱ�ٷֺ�
         }
    }else{
		if(sym == endsym) return;
        erro(49); //�����޷�ʶ������ɷ�
        skipUntil(1, statementSkip);
    }

}

void becomestatement(){
    item op1, obj, tempi;
    string n;
   // printf("��ֵ���\n");
	if(sym == endsym) return;
    if(sym == ident){
        n = id;
        obj = findcvpa(n);
        if(obj == NULL){
            //����δ����ı�ʶ��
            erro(35);
			skipUntil(1, statementSkip);
            return;
        }
        if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
            //������ֵ���󲻺Ϸ���ֻ���Ǳ��������顢������
            erro(36);
			skipUntil(1, statementSkip);
            return;
        }
        getsym();
    }else{
        erro(17); //ȱ�ٱ�ʶ��
    }
    if(sym == eql){  //a = b;
        getsym();
        expression(op1);
        if(op1 == NULL || !(op1->type == cchars || op1->type == cints || op1->type == chars || op1->type == ints)){
            //�������ʽ��ֵ�Ƿ�
            erro(37);
			skipUntil(1, statementSkip);
            return;
        }
        insertQuater(ASSIGN, op1, NULL, obj);

    }else if(sym == lbrack){  //a = b[i];
        getsym();
        if(obj->kind != arrays){
            //������������ȴʹ��[]
           erro(39);
		   skipUntil(1, statementSkip);
           return;

        }

        expression(tempi); //ƫ��

        if(tempi == NULL || (tempi->type != cchars && tempi->type != cints && tempi->type != ints && tempi->type != chars)){
            //�������������Ϊ�ջ������Ͳ�Ϊ���ϼ���
             erro(40);
			 skipUntil(1, statementSkip);
            return;

        }


        if(sym == rbrack){
            getsym();
        }else{
            erro(11);//ȱ��]
        }
        if(sym == eql){
            getsym();
        }else{
            erro(9); // ȱ��=

        }
        expression(op1); //ֵ
		insertQuater(WARRY, obj, tempi, op1); //op1,op2, obj д��������� op1Ϊ���飬 op2Ϊƫ�ƣ� objΪҪд��ֵ
        if(op1 == NULL || !(op1->type == cchars || op1->type == cints || op1->type == chars || op1->type == ints)){
            //�������ʽ��ֵ�Ƿ�
            erro(37);
			skipUntil(1, statementSkip);
            return;
        }

    }else{
		if(sym == endsym) return;
        erro(49); //��ֵ������
		skipUntil(1, statementSkip);

    }

}
void ifstatement(){
    item op1, tagA, tagB;
	if(sym == endsym) return;
    if(sym == ifsym){
        getsym();
       // printf("if�������\n");
    }else{
        erro(50);//ȱ��ifsym
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12); //ȱ��(
    }
    condition(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //��Ч��������������
        erro(41);
        return;
    }
    tagA = tagGen();
    insertQuater(FJMP, op1, NULL, tagA); //Ϊ����ת��FJMP, //op1, -, obj

    if(sym == rparen){
        getsym();
    }else{
        erro(12); //ȱ��)
    }

    statement();
    tagB = tagGen();
    insertQuater(JMP, NULL, NULL, tagB); //JMP, -, -, obj ��������ת
    insertQuater(TAG, NULL, NULL, tagA); //TAG, -�� -�� obj ���
    if(sym == elsesym){
       // printf("else���\n");
        getsym();
        statement();
    }
	insertQuater(TAG, NULL, NULL, tagB); //TAG, -�� -�� obj ���
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
        //�������ʽ���ʹ���
        erro(41);
        return;

    }


    if(sym == lss || sym == leq || sym == gtr || sym == geq || sym == eeql || sym == nql){
        symtemp = sym;
        getsym();
        expression(op2);
        if(op2 == NULL || !(op2->type == ints || op2->type == chars || op2->type == cints || op2->type == cchars)){
            //�������ʽ���ʹ���
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
       // printf("dowhile���\n");
        getsym();
    }else{
        erro(22); //ȱ��do
    }
    tagA = tagGen();  //�ڴ˴�����tagA��ǩ
    insertQuater(TAG, NULL, NULL, tagA); //TAG, -�� -�� obj ���
    statement();
    if(sym == whilesym){
        getsym();
    }else{
       erro(23);//ȱ��while
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12);//ȱ��������
    }
    condition(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //�������ʽ���ʹ���
        erro(41);
        return;

    }
    insertQuater(TJMP,op1, NULL, tagA); //����Ϊ������ת
    if(sym == rparen){
        getsym();
    }else{
        erro(13);//ȱ��)
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
       // printf("forѭ�����\n");
    }else{
        erro(24); //ȱ��for
    }
    if(sym == lparen){
        getsym();
    }else{
        erro(12); //ȱ��(
    }
    if(sym == ident){
        n = id;
        getsym();
        obj = findcvpa(n);
        if(obj == NULL){
            //����δ����ı���
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }else if(obj->kind == constant || obj->kind == arrays){
            //�������Ϸ���ѭ������
			erro(42);
			skipUntil(2,statementSkip);
        }
    }else{
        erro(17); //ȱ�ٱ�ʶ��
    }
    if(sym == eql){
        getsym();
    }else{
        erro(9); //ȱ��
    }
    expression(op1);
    if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //�������ʽ���Ͳ��Ϸ�
        erro(37);
		skipUntil(2,statementSkip);
        return;

    }
    insertQuater( ASSIGN, op1, NULL, obj);
    insertQuater(TAG, NULL, NULL , tagD);  //����tagD

    if(sym == semicolon){
        getsym();
    }else{
        erro(8);//ȱ�ٷֺ�
    }
    condition(op1);

    insertQuater(FJMP,op1, NULL,  tagA);
    insertQuater(TJMP,op1, NULL, tagB);
    insertQuater(TAG, NULL, NULL, tagC);

    if(sym == semicolon){
        getsym();
    }else{
        erro(8);//ȱ�ٷֺ�
    }
    if(sym == ident){
        n = id;
        obj = findcvpa(n);
        if(obj == NULL){
            //����δ����ı���
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }else if(obj->kind == constant || obj->kind == arrays){
            //������ֵ���󲻺Ϸ�
			erro(36);
        }
        getsym();
    }else{
        erro(17); //ȱ�ٱ�ʶ��
    }
    if(sym == eql){
        getsym();
    }else{
        erro(9); //ȱ�ٵȺ�
    }
    if(sym == ident){
        n = id;
        op1 = findcvpa(n);
        if(op1 == NULL || !(op1->type == ints || op1->type == chars || op1->type == cints || op1->type == cchars)){
        //�������ʽ���Ϸ�
        erro(37);
		skipUntil(2,statementSkip);
        return;

        }

        getsym();
    }else{
        erro(17);//ȱ�ٱ�ʶ��
    }
    if(sym == plus_ || sym == minus_){
        symtemp = sym;
        getsym();
    }else{
        erro(51); //ȱ�������
    }
    if(sym == number){
        if(num == 0){
            erro(43); //����Ϊ0
        }
        op2 = genCints(num);
        getsym();
    }else{
        erro(43);//ȱ�ٲ���
    }
    if(sym == rparen){
        getsym();
    }else{
        erro(13); //ȱ��)
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
      //  printf("�з���ֵ�ĺ�������\n");
        n = id;
        tempi = findFunc(n);
        if(tempi == NULL){
            //����δ����ĺ���
            erro(35);
			skipUntil(2, statementSkip);
            return;
        }
		if(tempi->returnV == nop){
			//���󣬸ú����޷���ֵ
		    erro(47);
			
		
		}
        getsym();
   }else{
    erro(17); //ȱ�ٱ�ʶ��
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12);
   }
   valueParaList(paraNum);
   if(tempi->paranum != paraNum){
        //��������������ƥ��
        erro(45);

   }

   result = genTempVar(tempi->returnV);
   insertQuater(RCALL, tempi, NULL, result);


   if(sym == rparen){
      getsym();
   }else{
       erro(13); //ȱ��)
   }
   if(sym == endsym) return;
}

void callfunc(){
    string n;
    item tempi;
    int paraNum;
	if(sym == endsym) return;
   if(sym == ident){
      //  printf("�޷���ֵ�ĺ�������\n");
        n = id;
        tempi = findFunc(n);
        if(tempi == NULL){
            //����δ����ĺ���
            erro(35);
			skipUntil(2,statementSkip);
            return;
        }
        getsym();
   }else{
    erro(17); //ȱ�ٺ�����
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12);
   }
   valueParaList(paraNum);
   if(paraNum != tempi->paranum){
        //��������������ƥ��
        erro(45);
   }

   insertQuater(CALL, tempi, NULL, NULL);

   if(sym == rparen){
      getsym();
   }else{
       erro(13); //ȱ��)
   }

   if(sym == endsym) return;
}

void valueParaList(int &paraNum){  //����ֻ��char��int�������ͣ�������ʱ�Ȳ�������ƥ����
    paraNum = 0;
    item result;
  //  printf("ֵ������\n");
	if(sym == endsym) return;
     if(sym == rparen){
        return;
     }
     expression(result);
     if(result == NULL || !(result->type == ints || result->type == chars || result->type == cints || result->type == cchars)){
        //�������ʽ���Ϸ�
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
        //�������ʽ���Ϸ�
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
    //  printf("�����\n");
      getsym();
   }else{
      erro(25);//ȱ�ٶ�����
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12); //ȱ��(
   }
   if(sym == ident){
       n = id;
       obj =findcvpa(n);
       if(obj == NULL){
            //����δ����ı�ʶ��
            erro(35);
			skipUntil(3,statementSkip);
            return;
       }
       if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
        //������ƥ��
        erro(45);
        
        }
        insertQuater(READ, NULL, NULL, obj);
       getsym();
   }else{
       erro(45); //�����ȱ�ٲ�����������ƥ��
   }
   while(sym == comma){
       getsym();
       if(sym == ident){
          n = id;
          obj =findcvpa(n);
          if(obj == NULL){
            //����δ����ı�ʶ��
            erro(35);
			skipUntil(3,statementSkip);
            return;
          }
          if(obj->kind != variable && obj->kind != arrays && obj->kind != parameter){
             //���������������Ϸ���������ƥ��
             erro(45);
            return;
          }
          insertQuater(READ, NULL, NULL, obj);
          getsym();
       }else{
         erro(27); //����ķ���
       }
   }
   if(sym == rparen){
      getsym();
   }else{
      erro(13); //ȱ��)
   }
   if(sym == endsym) return;
}


void writestatement(){
    item obj, tempi;
    string n, ss;
	if(sym == endsym) return;
   if(sym == writesym){
    //  printf("д���\n");
      getsym();
   }else{
      erro(26);//ȱ��д����
   }
   if(sym == lparen){
        getsym();
   }else{
        erro(12); //ȱ��(
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
      erro(13); //ȱ��)
      }

   }else {
       expression(obj);
       insertQuater(WRITE, NULL, NULL, obj);

       if(sym == rparen){
        getsym();
        }else{
        erro(13); //ȱ��)
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
       // printf("�������\n");
    }else{
      erro(28);//ȱ��return
    }
    if(sym == lparen){
        getsym();
        expression(obj);
        //����int��char����ʽת������ʱ��������ƥ����
        if(obj == NULL || !(obj->type == ints || obj->type == chars || obj->type == cints || obj->type == cchars)){
        //�����Ƿ��ķ���ֵ����
			erro(38);
			skipUntil(2,statementSkip);
			return;
        }
        if(tempi->returnV == nop){
            //���������еķ���ֵ
            erro(46);
            
        }

        insertQuater(RETN,NULL,NULL, obj);

        if(sym == rparen){
            getsym();
        }else{
            erro(13);//ȱ��)
        }
    }else{
        if(tempi->returnV != nop){
            //����û�з���ֵ
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
                    //ֻ�е����߶���charʱ�������char��ֻҪ��int�����Ϊint
                    //���⣬ 'c'+'c'��Ϊchars����cchars�Ƿ���ȷ������
                expResult = genTempVar(chars);
                insertQuater(ADD, tempi, termResult, expResult);
            }else {
                expResult = genTempVar(ints);
                insertQuater(ADD, tempi, termResult, expResult);
            }
			*/
			if(expResult->kind == constant && termResult->kind == constant){  //���ǳ���ʱ���������ϲ�
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
			
			}else{        //����������
				tempi = expResult;
				expResult = genTempVar(ints);   //��ΪֻҪchar��������ͻᱻתΪint��ֻ�в����������cahr����char
				insertQuater(ADD, tempi, termResult, expResult);
			}
			



        }else if(symtemp == minus_){
           
			/*
            if((expResult->type == cchars || expResult->type == chars) &&  (termResult->type == cchars || termResult->type == chars)){
                    //ֻ�е����߶���charʱ�������char��ֻҪ��int�����Ϊint
                    //���⣬ 'c'+'c'��Ϊchars����cchars�Ƿ���ȷ������
                expResult = genTempVar(chars);
                insertQuater(SUB, tempi, termResult, expResult);
            }else {
                expResult = genTempVar(ints);
                insertQuater(SUB, tempi, termResult, expResult);
            }
			*/
			if(expResult->kind == constant && termResult->kind == constant){  //���ǳ���ʱ���������ϲ�
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
			if(result->kind == constant && factResult->kind == constant){  //�����ϲ�
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

			
			}else{  //���ܺϲ���������
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
			if(result->kind == constant && factResult->kind == constant){  //�����ϲ�
				int div1, div2, divV;
				if(result->type == ints || result->type == cints)
					div1 = result->valueInt;
				else if(result->type == chars || result->type == cchars)
					div1 = result->valueChar;
				if(factResult->type == ints || factResult->type == cints)
					div2 = factResult->valueInt;
				else if(factResult->type == chars || factResult->type == cchars)
					div2 = factResult->valueChar;

				if(div2 != 0){   //����Ƿ����
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
            //����δ����ı���
            erro(35);
			skipUntil(8,factorSkip);
            return;

        }
        if(obj->kind != arrays){
            //����������Ԫ��ʹ����[]
            erro(39);

        }
        getsym();
        getsym();
     //   printf("����\n");

        expression(tempi);
        if(tempi == NULL || (tempi->type != cchars && tempi->type != cints && tempi->type != ints && tempi->type != chars)){
            //�����Ƿ������������
            erro(6);
            return;

        }


		tempvar = genTempVar(obj->type);
		insertQuater(RARRY, obj, tempi, tempvar); //����ȡ����Ԫ�ص��м����
		result = tempvar;

        if(sym == rbrack){
            getsym();
        }else{
            erro(11); //ȱ��]
        }

    }else if(sym == ident &&  symBuffer[(symPointer + 1) % 4].sym != lparen){
        n = id;
        result = findcvpa(n);
        if(result == NULL){
            //����δ����ı���
            erro(35);
			skipUntil(8, factorSkip);
            return;
        }
        if(!(result->kind == constant || result->kind == variable || result->kind == parameter)){
            //���󣬷Ƿ�������
            erro(48);
            return;
        }
        getsym();

   //     printf("����\n");

    }else if(sym == ident && symBuffer[(symPointer + 1) % 4].sym == lparen){
   //     printf("����\n");
        callrfunc(result);

    }else if(sym == plus_ || sym == minus_ || sym == number){
        if(sym == plus_){
            getsym();
            if(sym == number){
         //       printf("����\n");
                if(num == 0)
                   erro(32); //0ǰ�治����������
                tempi = genCints(num);
                result = tempi;
                getsym();
            }else{
                erro(29); //ȱ������
            }

        }else if(sym == minus_){
            getsym();
            if(sym == number){
       //         printf("����\n");
                if(num == 0)
                    erro(32);//0ǰ�治����������
                tempi = genCints(num * -1);
				result = tempi;
                //result =genTempVar(ints);
                //insertQuater(NEG, tempi, NULL, result);
                getsym();
                }else{
                  erro(29);//ȱ������
				  tempi = genCints(0); //������һ�����ּ���
                  result =genTempVar(ints);
                }
        }else if(sym == number){
       //      printf("����\n");
             result = genCints(num);
             getsym();

        }

    }else if(sym == charconst){
        result = genChars(chr);
        getsym();
      //  printf("����");

    }else if(sym == lparen){
        getsym();
        expression(result);
        if(sym == rparen){
            getsym();

        }else{
          erro(13);//ȱ��������
        }

    }else{
      erro(49);//������﷨
    }

	if(sym == endsym) return;
}





