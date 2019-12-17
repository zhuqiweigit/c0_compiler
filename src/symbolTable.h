#include <string>
#include <vector>
using std::vector;
using std::string;



#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H



typedef enum _kindenum{
    constant, //常量
    variable, //变量
    parameter, //函数参数
    func, //函数
    arrays, //数组
    nope //无
} _kind;

typedef enum _typeenum{
    ints,
    chars,
    cints, //整数字面量
    cchars, //char字面量
    cstring, //字符串字面量
    nop //无
} _type;

typedef class node{   //符号表项, 函数名，参数，常量，变量，数组，临时变量
    public:
    string name;
    _kind kind;
    _type type;
    int level; //所在层次，只有两种取值，0全局，1局部
    int isTemp; //标记是否是临时变量
    int valid;  //优化时使用，查看是否被优化删除
    int addr;  //相对于基地址的偏移
    int paranum;  //函数名使用,参数个数
    _type returnV;   //函数名使用,返回值类型，取值为ints、chars、nop，nop表示无返回值
    int length;  //数组长度（由于只有一维数组）
 //   class node* offset;  //仅仅在四元式中使用，纪录此项是数组的第几个项，从0开始，仅对数组有用
//	class node* father;  //指向父节点，即符号表中的数组模板,仅数组元素使用
    long valueInt; //字面量int的值
    char valueChar; //字面量char的值
    string svalue; //字符串字面量的值,也可以作为tag
	int funlen; //包括了参数+数组元素+变量+常量的个数(仅函数使用)
	int parath; //说明自己是第几个参数(形参或实参)，仅参数使用
	int isOptimized;  //是否被全局优化
	int reg;  //全局优化所分配的寄存器, 0-7
	int isDelete; //是否在构建的dag图中被删掉，0表示未删掉
	

} ITEM;
typedef ITEM *item;






typedef class node1{
    public:
    string name;  //函数名,方便检索
    vector<item> itemTB; //存储符号表项
} * ftable,  fTABLE;






//函数定义
item symTableNew(string name);
int findsymCur(string name);
int findsymGol(string name);
int findFuncName(string name);
item findFunc(string name);
item findFuncCur();
item findcvpa(string name);
string toStringKind(_kind a);
string toStringType(_type a);
void constdefInsert(_type tp, string name, long value, int level);
void vardefInsert(_type tp, _kind kd, string name, long length, int level);
item paraInsert(_type tp, string name, int parath);
string genTempVarName();
item genTempVar(_type tp);
item genCints(long valueInt);
item genChars(long valueChar);
item genCstring(string str);
void funcInsert(int paranum, _type returnV);
void printSymTable();
void itemInit(item &it);
void addrdispatch();
void parameterInfoInsert(int paraNum);
int findFuncTable(string name);

extern vector <ftable> symTable;  //以函数为单元存储的符号表


#endif // _SYMBOLTABLE_H
