#include <string>
#include <vector>
using std::vector;
using std::string;



#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H



typedef enum _kindenum{
    constant, //����
    variable, //����
    parameter, //��������
    func, //����
    arrays, //����
    nope //��
} _kind;

typedef enum _typeenum{
    ints,
    chars,
    cints, //����������
    cchars, //char������
    cstring, //�ַ���������
    nop //��
} _type;

typedef class node{   //���ű���, �����������������������������飬��ʱ����
    public:
    string name;
    _kind kind;
    _type type;
    int level; //���ڲ�Σ�ֻ������ȡֵ��0ȫ�֣�1�ֲ�
    int isTemp; //����Ƿ�����ʱ����
    int valid;  //�Ż�ʱʹ�ã��鿴�Ƿ��Ż�ɾ��
    int addr;  //����ڻ���ַ��ƫ��
    int paranum;  //������ʹ��,��������
    _type returnV;   //������ʹ��,����ֵ���ͣ�ȡֵΪints��chars��nop��nop��ʾ�޷���ֵ
    int length;  //���鳤�ȣ�����ֻ��һά���飩
 //   class node* offset;  //��������Ԫʽ��ʹ�ã���¼����������ĵڼ������0��ʼ��������������
//	class node* father;  //ָ�򸸽ڵ㣬�����ű��е�����ģ��,������Ԫ��ʹ��
    long valueInt; //������int��ֵ
    char valueChar; //������char��ֵ
    string svalue; //�ַ�����������ֵ,Ҳ������Ϊtag
	int funlen; //�����˲���+����Ԫ��+����+�����ĸ���(������ʹ��)
	int parath; //˵���Լ��ǵڼ�������(�βλ�ʵ��)��������ʹ��
	int isOptimized;  //�Ƿ�ȫ���Ż�
	int reg;  //ȫ���Ż�������ļĴ���, 0-7
	int isDelete; //�Ƿ��ڹ�����dagͼ�б�ɾ����0��ʾδɾ��
	

} ITEM;
typedef ITEM *item;






typedef class node1{
    public:
    string name;  //������,�������
    vector<item> itemTB; //�洢���ű���
} * ftable,  fTABLE;






//��������
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

extern vector <ftable> symTable;  //�Ժ���Ϊ��Ԫ�洢�ķ��ű�


#endif // _SYMBOLTABLE_H
