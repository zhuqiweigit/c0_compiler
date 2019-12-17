#include <vector>
#include <iostream>
#include "optimizer.h"
using namespace std;
using std::vector;

vector<fblock> fblocks;  //�洢������Ԫ
vector<record> records;  //�洢����
int contain(vector<int> &in, int in_find){
	for(unsigned int i = 0; i < in.size(); i++){
		if(in[i] == in_find)
			return 1;
	}
	return 0;
}

int isPre(quater &qtpre, quater &qt){
	if(qtpre->opc == RETN || qtpre->opc == NRETN){
			return 0;
	}else if(qtpre->opc == JMP){
		if(qt->opc == TAG && qt->obj->svalue == qtpre->obj->svalue)
			return 1;
		else{
			return 0;
		}
	}else{
		return 1;
	}
}

int isNext(quater &qtnext, quater &qt){
	if(qt->opc == RETN || qt->opc == NRETN){
		return 0;
	}else if(qt->opc == JMP){
		if(qtnext->opc == TAG && qtnext->obj->svalue == qt->obj->svalue){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 1;
	}
}

int find(vector<block> &p_n, block &blk){
	for(unsigned int i = 0; i < p_n.size(); i++){
		if(p_n[i] == blk)
			return 1;
	}
	return 0;

}

int find(vector<item> &allIt, item &it){
	for(unsigned int i = 0; i < allIt.size(); i++){
		if(allIt[i] == it)
			return 1;
	}
	return 0;
}

int find(vector<actinfo> &a, item &p){  //���ҽڵ��ڻ�Ծ��Ϣ���е�λ�ã����û�����½�һ������������λ��
	int tag = 0;
	for(int i = 0; i < a.size(); i++){
		if(a[i]->act_node == p){
			tag = 1;
			return i;
		}
	}
	if(tag == 0){
		actinfo tempinfo = new ACTINFO();
		tempinfo->act_node = p;
		a.push_back(tempinfo);
		return a.size() - 1;
	}
	
}

void getBlocks(){   //���ֻ�����
	vector<int> in;
	block tempBlock;
	fblock tempFBlock;
	int id = 0;
	for (unsigned int i  = 0; i < quaterList.size(); i++){
		if(quaterList[i]->opc == HEADER || quaterList[i]->opc == TAG)
			in.push_back(i);   //��ת���ĵ�һ����������ʼ
		else if(quaterList[i]->opc == TJMP || quaterList[i]->opc == FJMP || quaterList[i]->opc == JMP || quaterList[i]->opc == RETN ||quaterList[i]->opc == NRETN){ //�������
			if(i + 1 < quaterList.size()){
				in.push_back(i + 1);  //��ת������һ��
			}
		}

	}

	for(unsigned int i = 0; i < quaterList.size(); i++){  //������Ϊ��Ԫ�ռ�������
		if(contain(in, i)){
			tempBlock = new BLOCK();
			if(quaterList[i]->opc == HEADER){
				tempFBlock = new FBLOCK();
				fblocks.push_back(tempFBlock);
				id = 0;
			}
			tempBlock->id = id++;
			tempBlock->block_quter.push_back(quaterList[i]);
			tempFBlock->myblocks.push_back(tempBlock);

		}else{
			if(quaterList[i]->opc == FOOT){  //�Ǻ���β�����½�һ��exit��,�����뵱ǰ����
				tempBlock = new BLOCK();
				tempBlock->id = id++;
				tempBlock->block_quter.push_back(quaterList[i]);
				tempFBlock->myblocks.push_back(tempBlock);
			}else{
				tempBlock->block_quter.push_back(quaterList[i]); //����˵������ͨ����
			}
		}
	}
	//�Կ�������ͳ�����������
	for(unsigned int i = 0; i < fblocks.size(); i++){
		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			fblocks[i]->myblocks[j]->block_quter[0]->isBlockB = 1;
			fblocks[i]->myblocks[j]->block_quter.back()->isBlockE = 1;	
		}	
	}

	//ʶ��ǰ���ͺ�̿�
	for(unsigned int i = 0; i < fblocks.size(); i++){
		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			if(j > 0 && isPre(fblocks[i]->myblocks[j - 1]->block_quter.back(), fblocks[i]->myblocks[j]->block_quter[0])){  //ֱ��ǰ��
				if(!find(fblocks[i]->myblocks[j]->pre, fblocks[i]->myblocks[j - 1]))
					fblocks[i]->myblocks[j]->pre.push_back(fblocks[i]->myblocks[j - 1]);
			}
			if(j + 1 < fblocks[i]->myblocks.size() && isNext(fblocks[i]->myblocks[j + 1]->block_quter[0] , fblocks[i]->myblocks[j]->block_quter.back())){ //ֱ�Ӻ��
				if(!find(fblocks[i]->myblocks[j]->next, fblocks[i]->myblocks[j + 1] ))
					fblocks[i]->myblocks[j]->next.push_back(fblocks[i]->myblocks[j + 1]);
			}
			//////////////////back������Ҫ��֤///////////////////////
			quater tempq = fblocks[i]->myblocks[j]->block_quter.back(); 
			if(tempq->opc == TJMP || tempq->opc == FJMP || tempq->opc == JMP){ //���
				for(unsigned int k = 0; k < fblocks[i]->myblocks.size(); k++){
					quater tempq2 = fblocks[i]->myblocks[k]->block_quter[0];
					if(tempq2->opc == TAG && tempq2->obj->svalue == tempq->obj->svalue)
						if(!find(fblocks[i]->myblocks[j]->next, fblocks[i]->myblocks[k]))
							fblocks[i]->myblocks[j]->next.push_back(fblocks[i]->myblocks[k]);
				}
			}
			tempq = fblocks[i]->myblocks[j]->block_quter[0];  //ǰ��
			if(tempq->opc == TAG){
				for(unsigned int k = 0; k < fblocks[i]->myblocks.size(); k++){
					quater tempq2 = fblocks[i]->myblocks[k]->block_quter.back();
					if((tempq2->opc == TJMP || tempq2->opc == FJMP || tempq2->opc == JMP) && tempq2->obj->svalue == tempq->obj->svalue)
						if(!find(fblocks[i]->myblocks[j]->pre, fblocks[i]->myblocks[k]))
							fblocks[i]->myblocks[j]->pre.push_back(fblocks[i]->myblocks[k]);
				}
			}
			tempq = fblocks[i]->myblocks[j]->block_quter.back();  //��BexitΪ���
			if(tempq->opc == RETN || tempq->opc == NRETN){
				if(!find(fblocks[i]->myblocks[j]->next, fblocks[i]->myblocks.back()))
					fblocks[i]->myblocks[j]->next.push_back(fblocks[i]->myblocks.back());
			}

			if(j == fblocks[i]->myblocks.size() - 1){
				for(unsigned int k = 0; k < fblocks[i]->myblocks.size(); k++){
					quater tempq2 = fblocks[i]->myblocks[k]->block_quter.back();
					if(tempq2->opc == RETN || tempq2->opc == NRETN)
						if(!find(fblocks[i]->myblocks[j]->pre, fblocks[i]->myblocks[k]))
							fblocks[i]->myblocks[j]->pre.push_back(fblocks[i]->myblocks[k]);
				}
			}
		}	
	}



}


int use_loaction(vector <quater> &quaterls, item &it){  //���ص�һ�α�ʹ�õ�λ��
	for(unsigned i = 0; i < quaterls.size(); i++){
		if(quaterls[i]->op1 != NULL && quaterls[i]->op1 == it)
			return i;
		else if(quaterls[i]->op2 != NULL && quaterls[i]->op2 == it)
			return i;
		else if((quaterls[i]->opc == ACTPARA && quaterls[i]->obj == it) || (quaterls[i]->opc == WARRY && quaterls[i]->obj == it))
			return i;
	}

	return -1;
	
}

int def_loaction(vector <quater> &quaterls, item &it){ //��һ�α������λ��
	for(unsigned i = 0; i < quaterls.size(); i++){
		if((quaterls[i]->opc == ACTPARA && quaterls[i]->obj == it) || (quaterls[i]->opc == WARRY && quaterls[i]->obj == it))
			continue;
		if(quaterls[i]->obj != NULL && quaterls[i]->obj == it)
			return i;
	}

	return -1;
}



void use_def(){
	for(unsigned int i = 0; i < fblocks.size(); i++){
		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			vector <item> allIt;
			for(unsigned int k = 0; k < fblocks[i]->myblocks[j]->block_quter.size(); k++){
				quater quatert = fblocks[i]->myblocks[j]->block_quter[k];
				if(quatert->op1 != NULL && quatert->op1->kind == variable && quatert->op1->isTemp == 0 && !find(allIt, quatert->op1)){
					allIt.push_back(quatert->op1);
				}
				if(quatert->op2 != NULL && quatert->op2->kind == variable && quatert->op2->isTemp == 0 && !find(allIt, quatert->op2)){
					allIt.push_back(quatert->op2);
				}
				if(quatert->obj != NULL && quatert->obj->kind == variable && quatert->obj->isTemp == 0 && !find(allIt, quatert->obj)){
					allIt.push_back(quatert->obj);
				}
			}

			for(unsigned int kk = 0; kk < allIt.size(); kk++){
				int firstUse = use_loaction(fblocks[i]->myblocks[j]->block_quter, allIt[kk]);
				int firstDef = def_loaction(fblocks[i]->myblocks[j]->block_quter, allIt[kk]);
				if(firstUse == -1 && firstDef == -1);
				if(firstUse == -1 && firstDef != -1){
					fblocks[i]->myblocks[j]->def.push_back(allIt[kk]);
				}
				if(firstUse != -1 && firstDef == -1){
					fblocks[i]->myblocks[j]->use.push_back(allIt[kk]);
				}
				if(firstUse != -1 && firstDef != -1){
					if(firstUse < firstDef){
						fblocks[i]->myblocks[j]->use.push_back(allIt[kk]);
					}else if(firstUse > firstDef){
						fblocks[i]->myblocks[j]->def.push_back(allIt[kk]);
					}else{
						fblocks[i]->myblocks[j]->use.push_back(allIt[kk]);
					}
				}
			
			}	
		}
	}

}

void Union(vector<item> &a, vector<item> &b){  //��a��b�Ĳ����������a
	for(unsigned int i = 0; i < b.size(); i++){
		if(!find(a, b[i]))
			a.push_back(b[i]);
	}

}
void next_Gunion(block &bk){//�Ըû���������к�̻���������岢������û������out����
	vector<block> next = bk->next;
	bk->out.clear(); ///////////
	for(unsigned int i = 0; i < next.size(); i++){
		Union(bk->out, next[i]->in);
	}
}



void Minus(vector<item> &a, vector<item> &b, vector<item> &c){ //��a��b�Ĳ������c
	for(unsigned i = 0; i < a.size(); i++){
		if(!find(b, a[i]))
			c.push_back(a[i]);
	}
}

void in_value(block &bk){ // ��û������in����
	vector<item> temp;
	Minus(bk->out, bk->def, temp);
	Union(temp, bk->use);
	bk->in.clear();
	for(unsigned i = 0; i < temp.size(); i++)
		bk->in.push_back(temp[i]);
}

void updateRecord(fblock &fbk, vector<record> &recd){  //Ϊin��out��������
	recd.clear();
	record tepre;
	for(unsigned int i = 0; i < fbk->myblocks.size(); i++){
		tepre = new RECORD();
		for(unsigned j = 0; j < fbk->myblocks[i]->in.size(); j++)
			tepre->in.push_back(fbk->myblocks[i]->in[j]);
		for(unsigned j = 0; j < fbk->myblocks[i]->out.size(); j++)
			tepre->out.push_back(fbk->myblocks[i]->out[j]);
		recd.push_back(tepre);
	}
	
}

int isSame(fblock fbk, vector<record> recd){  //��鱸�ݺ͵�ǰ������inout���Ƿ���ͬ
	int tag = 1;
	for(unsigned int i = 0; i < fbk->myblocks.size(); i++){
		if(fbk->myblocks[i]->in.size() != recd[i]->in.size())
			tag = 0;
		else{
			for(unsigned j = 0; j < fbk->myblocks[i]->in.size(); j++){
				if(!find(recd[i]->in, fbk->myblocks[i]->in[j]))
					tag = 0;
			}
		}
		if(fbk->myblocks[i]->out.size() != recd[i]->out.size())
			tag = 0;
		else{
			for(unsigned j = 0; j < fbk->myblocks[i]->out.size(); j++){
				if(!find(recd[i]->out, fbk->myblocks[i]->out[j]))
					tag = 0;
			}
		}
	}

	return tag;
}

int find(vector<confi_node> &a, item &p){  //���ҽڵ��ڳ�ͻͼ�е�λ�ã�û�����½�һ����������λ��
	int tag = 0;
	for(int i = 0; i < a.size(); i++){
		if(a[i]->this_node == p){
			tag = 1;
			return i;
		}
	}
	if(tag == 0){
		confi_node tempcon = new CONFI_NODE();
		tempcon->this_node = p;
		a.push_back(tempcon);
		return a.size() - 1;
	}
}



void scan_var(item &tp, fblock &fbk){
	int fd;
	fd = find(fbk->myactinfo, tp);
	for(int i = 0; i < fbk->myblocks.size(); i++){
		for(int j = 0; j < fbk->myblocks[i]->in.size(); j++){
			if(fbk->myblocks[i]->in[j] == tp){ //�ҵ��ñ����Ļ�Ծλ��
				if(!contain(fbk->myactinfo[fd]->act_location, i)){////////////
					fbk->myactinfo[fd]->act_location.push_back(i);
				}
			}
		}

	}

}
int intersection(vector<int> &a, vector<int> &b){  //�ж����������Ƿ��н���
	for(int i = 0; i < a.size(); i++){
		for(int j = 0; j < b.size(); j++){
			if(a[i] == b[j])
				return 1;
		}
	}
	return 0;
}

void confi_graph(fblock &fbk){
	int fd;
	for(int i = 0; i < fbk->myactinfo.size(); i++){
		fd = find(fbk->myconfi_info, fbk->myactinfo[i]->act_node);
		for(int j = 0; j < fbk->myactinfo.size(); j++){
			if(i != j && intersection(fbk->myactinfo[i]->act_location, fbk->myactinfo[j]->act_location)){ //��Ծ��Χ�󽻼�,�н��������a�ĳ�ͻ��
				fbk->myconfi_info[fd]->confis.push_back(fbk->myactinfo[j]->act_node);
			}
		}
	}

}

int exist_node(fblock &fbk){  //�ж�ͼ���Ƿ��нڵ�δ���Ƴ�
	for(int i = 0; i < fbk->myconfi_info.size();i++){
		if(fbk->myconfi_info[i]->this_node->isDelete == 0)
			return 1;
	}
	return 0;
}


int count(vector<item> &it){  //����δ��ɾ���ı���
	int cout = 0;
	for(int i = 0; i < it.size(); i++){
		if(it[i]->isDelete == 0){
			cout++;
		}
	}
	return cout;
}

void remove_node(fblock &fbk){ //�Ƴ�һ���ڵ㣬����¼
	int tag = 0;
	for(int i = 0; i < fbk->myconfi_info.size();i++){
		if(fbk->myconfi_info[i]->this_node->isDelete == 0 && count(fbk->myconfi_info[i]->confis) < 8){   
			fbk->myconfi_info[i]->this_node->isDelete = 1;  //����ѱ�ɾ��
			fbk->myconfi_info[i]->this_node->isOptimized = 1; //���Ҫ��ȫ���Ż�
			fbk->myorder.push_back(i);
			tag = 1;
			break;
		}
	}
	if(tag == 0){  //δ�ҵ���С��8�ĵ㣬������Ƴ�һ��         
		for(int i = 0; i < fbk->myconfi_info.size(); i++){
			if(fbk->myconfi_info[i]->this_node->isDelete == 0 ){
				fbk->myconfi_info[i]->this_node->isDelete = 1;  //����ѱ�ɾ��
				fbk->myconfi_info[i]->this_node->isOptimized = 0; //��ǲ���ȫ���Ż�
				fbk->myorder.push_back(i);
				break;
			}
		}
	}
}


void build_node(fblock &fbk){
	for(int i = fbk->myorder.size() - 1; i >= 0; i--){
		int order_index = fbk->myorder[i];
		if(fbk->myconfi_info[order_index]->this_node->isOptimized == 1){  //ֻ���Ż��ı�������
			for(int reg = 0; reg < 8; reg++){  //���Է���  
				int tag = 0; //����Ƿ��г�ͻ
				for(int j = i; j < fbk->myorder.size(); j++){
					int order_index2 = fbk->myorder[j];
					if(fbk->myconfi_info[order_index2]->this_node->isOptimized 
						&& find(fbk->myconfi_info[order_index]->confis, fbk->myconfi_info[order_index2]->this_node)
						&& fbk->myconfi_info[order_index2]->this_node->reg == reg){ //�����Ƿ��뵱ǰ�ѽ����Ľڵ��ͻ
						tag = 1;
					}
				
				}
				if(tag == 0){  //�޳�ͻ�����
					fbk->myconfi_info[order_index]->this_node->reg = reg;
					break;
				}
				
			
			}
		}
	}
}
void color_reg(fblock &fbk){   //s0-s7��8��ȫ�ּĴ���
	while(exist_node(fbk)){
		remove_node(fbk);
	}
	//������ͼ��������Ĵ���
	build_node(fbk);
}

void active_value(){  //��Ծ��������
	//����in out����
	use_def(); 
	for(unsigned int i = 0; i < fblocks.size(); i++){
		int cou = 0;
		do{
			updateRecord( fblocks[i], records);
			for(int j = fblocks[i]->myblocks.size() - 1; j >= 0; j--){
				if(j < fblocks[i]->myblocks.size()){ //��out
					next_Gunion(fblocks[i]->myblocks[j]);
				}
				fblocks[i]->myblocks[j];
				in_value(fblocks[i]->myblocks[j]);  //��in
			}
			cou++;
		}while(!isSame(fblocks[i], records) && cou < 10000);  
		
		//����in��out�ҳ����������Ļ�Ծλ��
		for(int j = 0; j < fblocks[i]->myblocks.size(); j++){
			for(int k = 0; k < fblocks[i]->myblocks[j]->in.size(); k++){
				item temp = fblocks[i]->myblocks[j]->in[k];
				scan_var(temp, fblocks[i]);	
			}
		}

		//������ͻͼ
		confi_graph(fblocks[i]);

		//Ⱦɫ����Ĵ���
		color_reg(fblocks[i]);

	}

}





void printBlocks(){  //debug
	int q = 0;
	for(unsigned int i = 0; i < fblocks.size(); i++){
		cout << "function___"<< endl;
		cout<< "��Ծ������" << fblocks[i]->myactinfo.size() <<endl;
		for(unsigned int n = 0; n < fblocks[i]->myactinfo.size(); n++)
			cout<< fblocks[i]->myactinfo[n]->act_node->name << " ";

		cout<< "\n��ͻͼ�� "<< fblocks[i]->myconfi_info.size()<<endl;
		for(unsigned int n = 0; n < fblocks[i]->myconfi_info.size(); n++){
			cout<< "\n�ڵ㣺"<< fblocks[i]->myconfi_info[n]->this_node->name << " " <<"�Ĵ�����" << fblocks[i]->myconfi_info[n]->this_node->reg << " "<<endl;
			cout<<"��ͻ��";
			for(unsigned int nn = 0; nn < fblocks[i]->myconfi_info[n]->confis.size(); nn++)
				cout<< fblocks[i]->myconfi_info[n]->confis[nn]->name << " ";
		}

		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			cout <<"\n����id��"<< fblocks[i]->myblocks[j]->id << "_____________________________________________________________________"<<endl;
			cout << "ǰ�����:" << endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->pre.size(); m++)
				cout << fblocks[i]->myblocks[j]->pre[m]->id << " ";
			cout << "\n��̽��:" << endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->next.size(); m++)
				cout << fblocks[i]->myblocks[j]->next[m]->id << " ";
			cout << "\nin:���ϣ�" << fblocks[i]->myblocks[j]->in.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->in.size(); m++)
				cout << fblocks[i]->myblocks[j]->in[m]->name << " ";
			cout << "\nout:���ϣ�" << fblocks[i]->myblocks[j]->out.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->out.size(); m++)
				cout << fblocks[i]->myblocks[j]->out[m]->name << " ";
			cout << "\nuse:���ϣ�" << fblocks[i]->myblocks[j]->use.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->use.size(); m++)
				cout << fblocks[i]->myblocks[j]->use[m]->name << " ";
			cout << "\ndef:���ϣ�" << fblocks[i]->myblocks[j]->def.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->def.size(); m++)
				cout << fblocks[i]->myblocks[j]->def[m]->name << " ";
			for(unsigned int k = 0; k < fblocks[i]->myblocks[j]->block_quter.size(); k++){
				cout << "\nʽ��" << q++ << endl;
				printAquayer(fblocks[i]->myblocks[j]->block_quter[k]);
			}
		}
	}
}