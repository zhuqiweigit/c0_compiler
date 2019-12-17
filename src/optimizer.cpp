#include <vector>
#include <iostream>
#include "optimizer.h"
using namespace std;
using std::vector;

vector<fblock> fblocks;  //存储函数单元
vector<record> records;  //存储备份
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

int find(vector<actinfo> &a, item &p){  //查找节点在活跃信息表中的位置，如果没有则新建一个，并传回其位置
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

void getBlocks(){   //划分基本块
	vector<int> in;
	block tempBlock;
	fblock tempFBlock;
	int id = 0;
	for (unsigned int i  = 0; i < quaterList.size(); i++){
		if(quaterList[i]->opc == HEADER || quaterList[i]->opc == TAG)
			in.push_back(i);   //跳转到的第一条、函数开始
		else if(quaterList[i]->opc == TJMP || quaterList[i]->opc == FJMP || quaterList[i]->opc == JMP || quaterList[i]->opc == RETN ||quaterList[i]->opc == NRETN){ //返回语句
			if(i + 1 < quaterList.size()){
				in.push_back(i + 1);  //跳转语句的下一条
			}
		}

	}

	for(unsigned int i = 0; i < quaterList.size(); i++){  //按函数为单元收集基本块
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
			if(quaterList[i]->opc == FOOT){  //是函数尾部则新建一个exit块,并加入当前函数
				tempBlock = new BLOCK();
				tempBlock->id = id++;
				tempBlock->block_quter.push_back(quaterList[i]);
				tempFBlock->myblocks.push_back(tempBlock);
			}else{
				tempBlock->block_quter.push_back(quaterList[i]); //否则说明是普通代码
			}
		}
	}
	//对块入口语句和出口语句做标记
	for(unsigned int i = 0; i < fblocks.size(); i++){
		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			fblocks[i]->myblocks[j]->block_quter[0]->isBlockB = 1;
			fblocks[i]->myblocks[j]->block_quter.back()->isBlockE = 1;	
		}	
	}

	//识别前驱和后继块
	for(unsigned int i = 0; i < fblocks.size(); i++){
		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			if(j > 0 && isPre(fblocks[i]->myblocks[j - 1]->block_quter.back(), fblocks[i]->myblocks[j]->block_quter[0])){  //直接前驱
				if(!find(fblocks[i]->myblocks[j]->pre, fblocks[i]->myblocks[j - 1]))
					fblocks[i]->myblocks[j]->pre.push_back(fblocks[i]->myblocks[j - 1]);
			}
			if(j + 1 < fblocks[i]->myblocks.size() && isNext(fblocks[i]->myblocks[j + 1]->block_quter[0] , fblocks[i]->myblocks[j]->block_quter.back())){ //直接后继
				if(!find(fblocks[i]->myblocks[j]->next, fblocks[i]->myblocks[j + 1] ))
					fblocks[i]->myblocks[j]->next.push_back(fblocks[i]->myblocks[j + 1]);
			}
			//////////////////back函数需要验证///////////////////////
			quater tempq = fblocks[i]->myblocks[j]->block_quter.back(); 
			if(tempq->opc == TJMP || tempq->opc == FJMP || tempq->opc == JMP){ //后继
				for(unsigned int k = 0; k < fblocks[i]->myblocks.size(); k++){
					quater tempq2 = fblocks[i]->myblocks[k]->block_quter[0];
					if(tempq2->opc == TAG && tempq2->obj->svalue == tempq->obj->svalue)
						if(!find(fblocks[i]->myblocks[j]->next, fblocks[i]->myblocks[k]))
							fblocks[i]->myblocks[j]->next.push_back(fblocks[i]->myblocks[k]);
				}
			}
			tempq = fblocks[i]->myblocks[j]->block_quter[0];  //前驱
			if(tempq->opc == TAG){
				for(unsigned int k = 0; k < fblocks[i]->myblocks.size(); k++){
					quater tempq2 = fblocks[i]->myblocks[k]->block_quter.back();
					if((tempq2->opc == TJMP || tempq2->opc == FJMP || tempq2->opc == JMP) && tempq2->obj->svalue == tempq->obj->svalue)
						if(!find(fblocks[i]->myblocks[j]->pre, fblocks[i]->myblocks[k]))
							fblocks[i]->myblocks[j]->pre.push_back(fblocks[i]->myblocks[k]);
				}
			}
			tempq = fblocks[i]->myblocks[j]->block_quter.back();  //以Bexit为后继
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


int use_loaction(vector <quater> &quaterls, item &it){  //返回第一次被使用的位置
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

int def_loaction(vector <quater> &quaterls, item &it){ //第一次被定义的位置
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

void Union(vector<item> &a, vector<item> &b){  //求a与b的并，结果放入a
	for(unsigned int i = 0; i < b.size(); i++){
		if(!find(a, b[i]))
			a.push_back(b[i]);
	}

}
void next_Gunion(block &bk){//对该基本块的所有后继基本块求广义并，存入该基本块的out集合
	vector<block> next = bk->next;
	bk->out.clear(); ///////////
	for(unsigned int i = 0; i < next.size(); i++){
		Union(bk->out, next[i]->in);
	}
}



void Minus(vector<item> &a, vector<item> &b, vector<item> &c){ //求a与b的差集，放入c
	for(unsigned i = 0; i < a.size(); i++){
		if(!find(b, a[i]))
			c.push_back(a[i]);
	}
}

void in_value(block &bk){ // 求该基本块的in集合
	vector<item> temp;
	Minus(bk->out, bk->def, temp);
	Union(temp, bk->use);
	bk->in.clear();
	for(unsigned i = 0; i < temp.size(); i++)
		bk->in.push_back(temp[i]);
}

void updateRecord(fblock &fbk, vector<record> &recd){  //为in和out建立拷贝
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

int isSame(fblock fbk, vector<record> recd){  //检查备份和当前函数的inout集是否相同
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

int find(vector<confi_node> &a, item &p){  //查找节点在冲突图中的位置，没有则新建一个，返回其位置
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
			if(fbk->myblocks[i]->in[j] == tp){ //找到该变量的活跃位置
				if(!contain(fbk->myactinfo[fd]->act_location, i)){////////////
					fbk->myactinfo[fd]->act_location.push_back(i);
				}
			}
		}

	}

}
int intersection(vector<int> &a, vector<int> &b){  //判断两个集合是否有交集
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
			if(i != j && intersection(fbk->myactinfo[i]->act_location, fbk->myactinfo[j]->act_location)){ //活跃范围求交集,有交集则加入a的冲突表
				fbk->myconfi_info[fd]->confis.push_back(fbk->myactinfo[j]->act_node);
			}
		}
	}

}

int exist_node(fblock &fbk){  //判断图中是否还有节点未被移除
	for(int i = 0; i < fbk->myconfi_info.size();i++){
		if(fbk->myconfi_info[i]->this_node->isDelete == 0)
			return 1;
	}
	return 0;
}


int count(vector<item> &it){  //计算未被删除的边数
	int cout = 0;
	for(int i = 0; i < it.size(); i++){
		if(it[i]->isDelete == 0){
			cout++;
		}
	}
	return cout;
}

void remove_node(fblock &fbk){ //移除一个节点，并纪录
	int tag = 0;
	for(int i = 0; i < fbk->myconfi_info.size();i++){
		if(fbk->myconfi_info[i]->this_node->isDelete == 0 && count(fbk->myconfi_info[i]->confis) < 8){   
			fbk->myconfi_info[i]->this_node->isDelete = 1;  //标记已被删除
			fbk->myconfi_info[i]->this_node->isOptimized = 1; //标记要被全局优化
			fbk->myorder.push_back(i);
			tag = 1;
			break;
		}
	}
	if(tag == 0){  //未找到边小于8的点，则随便移除一个         
		for(int i = 0; i < fbk->myconfi_info.size(); i++){
			if(fbk->myconfi_info[i]->this_node->isDelete == 0 ){
				fbk->myconfi_info[i]->this_node->isDelete = 1;  //标记已被删除
				fbk->myconfi_info[i]->this_node->isOptimized = 0; //标记不被全局优化
				fbk->myorder.push_back(i);
				break;
			}
		}
	}
}


void build_node(fblock &fbk){
	for(int i = fbk->myorder.size() - 1; i >= 0; i--){
		int order_index = fbk->myorder[i];
		if(fbk->myconfi_info[order_index]->this_node->isOptimized == 1){  //只给优化的变量分配
			for(int reg = 0; reg < 8; reg++){  //尝试分配  
				int tag = 0; //标记是否有冲突
				for(int j = i; j < fbk->myorder.size(); j++){
					int order_index2 = fbk->myorder[j];
					if(fbk->myconfi_info[order_index2]->this_node->isOptimized 
						&& find(fbk->myconfi_info[order_index]->confis, fbk->myconfi_info[order_index2]->this_node)
						&& fbk->myconfi_info[order_index2]->this_node->reg == reg){ //查找是否与当前已建立的节点冲突
						tag = 1;
					}
				
				}
				if(tag == 0){  //无冲突则分配
					fbk->myconfi_info[order_index]->this_node->reg = reg;
					break;
				}
				
			
			}
		}
	}
}
void color_reg(fblock &fbk){   //s0-s7共8个全局寄存器
	while(exist_node(fbk)){
		remove_node(fbk);
	}
	//逆序建立图，并分配寄存器
	build_node(fbk);
}

void active_value(){  //活跃变量分析
	//计算in out集合
	use_def(); 
	for(unsigned int i = 0; i < fblocks.size(); i++){
		int cou = 0;
		do{
			updateRecord( fblocks[i], records);
			for(int j = fblocks[i]->myblocks.size() - 1; j >= 0; j--){
				if(j < fblocks[i]->myblocks.size()){ //求out
					next_Gunion(fblocks[i]->myblocks[j]);
				}
				fblocks[i]->myblocks[j];
				in_value(fblocks[i]->myblocks[j]);  //求in
			}
			cou++;
		}while(!isSame(fblocks[i], records) && cou < 10000);  
		
		//根据in、out找出各个变量的活跃位置
		for(int j = 0; j < fblocks[i]->myblocks.size(); j++){
			for(int k = 0; k < fblocks[i]->myblocks[j]->in.size(); k++){
				item temp = fblocks[i]->myblocks[j]->in[k];
				scan_var(temp, fblocks[i]);	
			}
		}

		//构建冲突图
		confi_graph(fblocks[i]);

		//染色分配寄存器
		color_reg(fblocks[i]);

	}

}





void printBlocks(){  //debug
	int q = 0;
	for(unsigned int i = 0; i < fblocks.size(); i++){
		cout << "function___"<< endl;
		cout<< "活跃变量：" << fblocks[i]->myactinfo.size() <<endl;
		for(unsigned int n = 0; n < fblocks[i]->myactinfo.size(); n++)
			cout<< fblocks[i]->myactinfo[n]->act_node->name << " ";

		cout<< "\n冲突图： "<< fblocks[i]->myconfi_info.size()<<endl;
		for(unsigned int n = 0; n < fblocks[i]->myconfi_info.size(); n++){
			cout<< "\n节点："<< fblocks[i]->myconfi_info[n]->this_node->name << " " <<"寄存器：" << fblocks[i]->myconfi_info[n]->this_node->reg << " "<<endl;
			cout<<"冲突：";
			for(unsigned int nn = 0; nn < fblocks[i]->myconfi_info[n]->confis.size(); nn++)
				cout<< fblocks[i]->myconfi_info[n]->confis[nn]->name << " ";
		}

		for(unsigned int j = 0; j < fblocks[i]->myblocks.size(); j++){
			cout <<"\n本块id："<< fblocks[i]->myblocks[j]->id << "_____________________________________________________________________"<<endl;
			cout << "前驱结点:" << endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->pre.size(); m++)
				cout << fblocks[i]->myblocks[j]->pre[m]->id << " ";
			cout << "\n后继结点:" << endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->next.size(); m++)
				cout << fblocks[i]->myblocks[j]->next[m]->id << " ";
			cout << "\nin:集合：" << fblocks[i]->myblocks[j]->in.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->in.size(); m++)
				cout << fblocks[i]->myblocks[j]->in[m]->name << " ";
			cout << "\nout:集合：" << fblocks[i]->myblocks[j]->out.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->out.size(); m++)
				cout << fblocks[i]->myblocks[j]->out[m]->name << " ";
			cout << "\nuse:集合：" << fblocks[i]->myblocks[j]->use.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->use.size(); m++)
				cout << fblocks[i]->myblocks[j]->use[m]->name << " ";
			cout << "\ndef:集合：" << fblocks[i]->myblocks[j]->def.size()<<endl;
			for(unsigned m = 0; m < fblocks[i]->myblocks[j]->def.size(); m++)
				cout << fblocks[i]->myblocks[j]->def[m]->name << " ";
			for(unsigned int k = 0; k < fblocks[i]->myblocks[j]->block_quter.size(); k++){
				cout << "\n式子" << q++ << endl;
				printAquayer(fblocks[i]->myblocks[j]->block_quter[k]);
			}
		}
	}
}