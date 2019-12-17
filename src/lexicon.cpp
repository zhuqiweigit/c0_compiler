#include <string.h>
#include "lexicon.h"
#include <string>
#include "symbolTable.h"
#include "Quaternary.h"
#include "error.h"

FILE *fp;  //Դ�����ļ�ָ��
char ch = ' '; //���ڴʷ���������������һ�ζ�����һ���ַ�
symbol sym; //������һ��ʶ������ķ��ŵ�����
char id[31];  //�ݴ�����ı�ʶ��������
int id_k; //��ʶ������ָ�룬ָ���һ����Ч�ַ���λ��
long num; //�ݴ���������ֵ�ֵ
char str[1001]; //�ݴ�������ַ�����ֵ
char chr; //�ݴ������char������
int cc = -1; //�л�����ָ��
int ll = -1; //�л���������,ָ�����һ����Ч�ַ�
char line[302]; //�л����������ļ��ж�ȡһ�У����ʷ�����ʹ��
int linenum = 0;  //�кż���������¼��ǰ���ڷ������кţ������˿հ��У�,getch()�����ڲ�ʹ�ã��������ṩ��
int lineindex = 0; //�кż���������linenum��ʾͬ������˼�����ɸ��ⲿʹ��
int endtag = 0; //����Ƿ���������ļ�
int blank;   //�հ�ָʾ������ʾ��ǰ����ĵ��ʵ�ǰ���Ƿ��пհ׷�


/////////////////��ǰԤ������///////////////////


/*Ԥ��������������ȡ�������ţ����洢�������Ϣ*/
struct presym symBuffer[4];
int symPointer = 0; //������ָ�룬ʼ��ָ����Ч�ĵ�ǰ����λ��
int firstUse = 1; //ָʾ�Ƿ�Ϊ��һ��ʹ�ã�����init����

void saveSym(int i){
    symBuffer[i].sym = sym;
    strcpy(symBuffer[i].id, id);
    symBuffer[i].num = num;
    strcpy(symBuffer[i].str, str);
    symBuffer[i].chr = chr;
    symBuffer[i].lineindex = lineindex;
    symBuffer[i].endtag = endtag;
    symBuffer[i].blank = blank;
}

void restoreSym(int i){ 
    sym = symBuffer[i].sym;
    strcpy(id, symBuffer[i].id);
    num = symBuffer[i].num;
    strcpy(str, symBuffer[i].str);
    chr = symBuffer[i].chr;
    lineindex = symBuffer[i].lineindex;
    endtag = symBuffer[i].endtag;
    blank = symBuffer[i].blank;
}

void initSymBuffer(){
    symPointer = 0;
    saveSym(0);
    getsym();
    saveSym(1);
    getsym();
    saveSym(2);
    getsym();
    saveSym(3);
    restoreSym(0);    //��ȡ��ɺ�ȫ�ִʷ���Ϣ�ָ���0��
}

void updateSymBuffer(){
    saveSym(symPointer);
    symPointer = (symPointer + 1) % 4;  //ָ��ָ����һ����Чλ�á�
    restoreSym(symPointer);
}


///////////////////////////////////////////////////////////////////

void getch(){
	lineindex = linenum;
    if(endtag == 1){
        //printf("����");
				        
    }
    while(cc == ll && endtag == 0){
        linenum ++;

        if(NULL == (fgets(line, 300, fp))){
            
            endtag = 1;
            fclose(fp);
        }else{
            cc = -1;
            ll = 0;
            while(line[ll] != '\0' && line[ll] != '\r' && line[ll] != '\n')
                ll++;
            ll = ll - 1;
        }

    }
	if(endtag == 1){
		ch = 3;
	}else{
		ch = line[++cc];
	}
	
}

int isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
        return 1;
    }
    else
    return 0;
}

int isNumber(char c){
    return (c >= '0' && c <= '9') ? 1 : 0;
}

symbol reserver(char buff[]){
    if(strcmp("if", buff) == 0)
        return ifsym;
    else if(strcmp("else", buff) == 0)
        return elsesym;
    else if(strcmp("do",buff) == 0)
        return dosym;
    else if(strcmp("while",buff) == 0)
        return whilesym;
    else if(strcmp("for", buff) == 0)
        return forsym;
    else if(strcmp("const", buff) == 0)
        return constsym;
    else if(strcmp("int", buff) == 0)
        return intsym;
    else if(strcmp("char", buff) == 0)
        return charsym;
    else if(strcmp("void", buff) == 0)
        return voidsym;
    else if(strcmp("scanf", buff) == 0)
        return readsym;
    else if(strcmp("printf", buff) == 0)
        return writesym;
    else if(strcmp("main", buff) == 0)
        return mainsym;
    else if(strcmp("return", buff) == 0)
        return returnsym;
    else
        return nul;


}



void output(char buff[], symbol c){
    switch(c){
    case plus_:
        strcpy(buff, "plus_"); break;
    case minus_:
        strcpy(buff, "minus_"); break;
    case times:
        strcpy(buff, "times"); break;
    case slash:
        strcpy(buff, "slash"); break;
    case lss:
        strcpy(buff, "lss"); break;
    case leq:
        strcpy(buff, "leq"); break;
    case gtr:
        strcpy(buff, "gtr"); break;
    case geq:
        strcpy(buff, "geq"); break;
    case eeql:
        strcpy(buff, "eeql"); break;
    case nql:
        strcpy(buff, "nql"); break;
    case lparen:
        strcpy(buff, "lparen"); break;
    case rparen:
        strcpy(buff, "rparen"); break;
    case lbrack:
        strcpy(buff, "lbrack"); break;
    case rbrack:
        strcpy(buff, "rbrack"); break;
    case lbrace:
        strcpy(buff, "lbrace"); break;
    case rbrace:
        strcpy(buff, "rbrace"); break;
    case comma:
        strcpy(buff, "comma"); break;
    case semicolon:
        strcpy(buff, "semicolon"); break;
    case eql:
        strcpy(buff, "eql"); break;
    case ifsym:
        strcpy(buff, "ifsym"); break;
    case elsesym:
        strcpy(buff, "elsesym"); break;
    case dosym:
        strcpy(buff, "dosym"); break;
    case whilesym:
        strcpy(buff, "whilesym"); break;
    case forsym:
        strcpy(buff, "forsym"); break;
    case constsym:
        strcpy(buff, "constsym"); break;
    case intsym:
        strcpy(buff, "intsym"); break;
    case charsym:
        strcpy(buff, "charsym"); break;
    case voidsym:
        strcpy(buff, "voidsym"); break;
    case readsym:
        strcpy(buff, "readsym"); break;
    case writesym:
        strcpy(buff, "writesym"); break;
    case ident:
        strcpy(buff, "ident"); break;
    case number:
        strcpy(buff, "number"); break;
    case charconst:
        strcpy(buff, "charconst"); break;
    case strconst:
        strcpy(buff, "strconst"); break;
    case mainsym:
        strcpy(buff, "mainsym"); break;
    case returnsym:
        strcpy(buff, "returnsym"); break;
    case wrongsym:
        strcpy(buff, "wrongsym"); break;
    default:
        strcpy(buff, ""); break;
    }

}

void getsym(){
    int i, zerotag;
    blank = 0;
    while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0'){
        blank = 1;
        getch();
    }
    if(isLetter(ch)){
        id_k = 0;
        do{
            if(id_k < al)
                id[id_k++] = ch;
            getch();

        }while(isNumber(ch) || isLetter(ch));
        id[id_k] = '\0';
        sym = reserver(id);
        if(sym == nul)
            sym = ident;

    } else if(isNumber(ch)){
        zerotag = 0;  //��ʶ��һ�������Ƿ�Ϊ0
        if(ch == '0')
            zerotag = 1;
        i = 0;
        num = 0;
        sym = number;
        do{
            num = 10 * num + (ch - '0');
            i = i + 1;
            getch();
        }while(isNumber(ch));

        if(zerotag == 1 && i > 1){
            erro(5 );  //����ǰ��0
        }
        if(i > nmax){
            erro( 1);//��������λ����������
            sym = wrongsym;
        }
    }else if(ch == '\"'){  //�����ַ���
        i = 0;
        sym = strconst;
        getch();
        while(ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)){
            if(i <= smax)
                str[i++] = ch;
            getch();
        }
        str[i] = '\0';
        if(ch != '\"'){

            erro(2); /*����û�е�����˫����*/

        }else{
            getch();
        }

    }else if(ch == '\''){  /*�����п�����char������*/
        getch();
        if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || isNumber(ch) || isLetter(ch)){
            chr = ch;
            sym = charconst;
            getch();
            if(ch == '\''){
                getch();
            }else{

                erro(3);  /*����ȱ���ҵ�����*/

            }
        }else{
            //�������Ϸ����ַ�char
            erro(4);
            sym = wrongsym;
        }
    }else if(ch == '+'){
        sym = plus_;
        getch();
    }else if(ch == '-'){
        sym = minus_;
        getch();
    }else if(ch == '*'){
        sym = times;
        getch();
    }else if(ch == '/'){
        sym = slash;
        getch();
    }else if(ch == '<'){
        getch();
        if(ch == '='){
            sym = leq;
            getch();
        }else
            sym = lss;
    }else if(ch == '>'){
        getch();
        if(ch == '='){
            sym = geq;
            getch();
        }else
            sym = gtr;
    }else if(ch == '='){
        getch();
        if(ch == '='){
            sym = eeql;
            getch();
        }else
            sym = eql;
    }else if(ch == '!'){
        getch();
        if(ch == '='){
            sym = nql;
            getch();
        }else{
            //����������!�ǷǷ��ַ�
            erro( 0);
            sym = wrongsym;
        }
    }else if(ch == '('){
        sym = lparen;
        getch();
    }else if(ch == ')'){
        sym = rparen;
        getch();
    }else if(ch == '['){
        sym = lbrack;
        getch();
    }else if(ch == ']'){
        sym = rbrack;
        getch();
    }else if(ch == '{'){
        sym = lbrace;
        getch();
    }else if(ch == '}'){
        sym = rbrace;
        getch();
    }else if(ch == ','){
        sym = comma;
        getch();
    }else if(ch == ';'){
        sym = semicolon;
        getch();
	}else if(ch == 3){
		sym = endsym;	
	}
    else{
        //����δ֪����
        erro( 0);
        sym = wrongsym;
        getch();
    }

    if(firstUse == 0){
        updateSymBuffer();
    }



}




