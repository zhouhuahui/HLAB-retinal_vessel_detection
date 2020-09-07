#include"pch.h"
#include"trans.h"
#include"vessel_radius.h"
#include"vessel_depth.h"
#include<cmath>
#include<algorithm>
#include<map>

/*
一级视野：只考虑最多连续的一个小段
*/

/*
参数：前3个点
	  交叉点处支路半径设为0.7倍平均半径
*/

/*给定两个小段，计算fai值：fai = -[cos(alpha)*cos(byta) + sin(alpha)*sin(byta)]
fai值在[0,2]区间内
*/
double Trans::cmptAngle(SmallSeg* a, SmallSeg* b, Point p) {
	double aa[4]; //保存两条小段的单位化向量
	int sizea = a->s->pixels.size();
	int sizeb = b->s->pixels.size();
	Point p1, p2;
	double sum1,sum2;
	double sin_alpha, cos_alpha, sin_byta, cos_byta, cos_thita;

	/*计算a的向量*/
	if (sizea <= 12) { //如果小段不够长
		p1 = (p == a->s->pixels.at(0)) ? a->s->pixels.at(sizea - 1) : a->s->pixels.at(0);
		p2 = p;
	}
	else {
		p1 = (a->s->pixels.at(0) == p) ? a->s->pixels.at(12) : a->s->pixels.at(sizea - 13);
		p2 = (a->s->pixels.at(0) == p) ? a->s->pixels.at(3) : a->s->pixels.at(sizea - 4);
	}
	aa[0] = p1.x - p2.x;
	aa[1] = p1.y - p2.y;
	sum1 = sqrt(pow(aa[0], 2) + pow(aa[1], 2));
	/*计算b的向量*/
	if (sizeb <= 12) { //如果小段不够长
		p2 = (p == b->s->pixels.at(0)) ? b->s->pixels.at(sizeb - 1) : b->s->pixels.at(0);
		p1 = p;
	}
	else {
		p2 = (b->s->pixels.at(0) == p) ? b->s->pixels.at(12) : b->s->pixels.at(sizeb - 13);
		p1 = (b->s->pixels.at(0) == p) ? b->s->pixels.at(3) : b->s->pixels.at(sizeb - 4);
	}
	aa[2] = p1.x - p2.x;
	aa[3] = p1.y - p2.y;
	sum2 = sqrt(pow(aa[2], 2) + pow(aa[3], 2));

	/*计算两个seg的夹角*/
	sin_alpha = aa[1] / sum1;
	cos_alpha = aa[0] / sum1;
	sin_byta = aa[3] / sum2;
	cos_byta = aa[2] / sum2;
	cos_thita = cos_alpha * cos_byta + sin_alpha * sin_byta;

	return -cos_thita;
};

/*把大段a和大段b在交叉点p处合并到a中
*/
void Trans::merge(BigSeg* a, BigSeg* b, Point p) {
	int sizea = a->v.size(); 
	int sizeb = b->v.size(); 
	int sizeav1 = a->v.at(0)->s->pixels.size();
	int sizeav2 = a->v.at(sizea-1)->s->pixels.size();
	int sizebv1 = b->v.at(0)->s->pixels.size();
	int sizebv2 = b->v.at(sizeb - 1)->s->pixels.size();
	int flaga = 0, flagb = 0; //flaga = 1,表示p在a的尾部
	SmallSeg* s;

	if ( (a->v.at(sizea - 1)->direction == 1 && p == a->v.at(sizea - 1)->s->pixels.at(sizeav2 - 1)) || (a->v.at(sizea - 1)->direction == 0 && p == a->v.at(sizea - 1)->s->pixels.at(0)))
		flaga = 1;
	if ( (b->v.at(sizeb-1)->direction == 1 && p == b->v.at(sizeb - 1)->s->pixels.at(sizebv2 - 1)) || (b->v.at(sizeb - 1)->direction == 0 && p == b->v.at(sizeb - 1)->s->pixels.at(0)))
		flagb = 1;

	if (flaga) {
		//a->v.back()->rs.at(sizeav2 - 1) = a->v.back()->radius; //更改连接点的半径值
		if (flagb) {
			//b->v.back()->rs.at(sizebv2 - 1) = b->v.back()->radius; //更改连接点的半径值
			for (int i = sizeb - 1; i >= 0; --i) {
				s = b->v.at(i);
				s->direction = 1 - s->direction;
				s->bs = a;
				a->v.push_back(s);
			}
		}
		else {
			//更改连接点的半径值
			/*if (b->v.at(0)->direction == 1) 
				b->v.at(0)->rs.at(0) = b->v.at(0)->radius;
			else
				b->v.at(0)->rs.at(sizebv1 - 1) = b->v.at(0)->radius;*/

			for (int i = 0; i < sizeb; ++i) {
				s = b->v.at(i);
				s->bs = a;
				a->v.push_back(s);
			}
		}
	}
	else {
		//更改连接点的半径值
		/*if (a->v.at(0)->direction == 1)
			a->v.at(0)->rs.at(0) = a->v.at(0)->radius;
		else
			a->v.at(0)->rs.at(sizeav1 - 1) = a->v.at(0)->radius;*/

		if (flagb) {
			//b->v.back()->rs.at(sizebv2 - 1) = b->v.back()->radius; //更改连接点的半径值
			a->v.resize(sizea + sizeb);
			for (int i = sizea - 1; i >= 0; --i)
				a->v.at(i + sizeb) = a->v.at(i);
			for (int i = sizeb - 1; i >= 0; --i) {
				a->v.at(i) = b->v.at(i);
				b->v.at(i)->bs = a;
			}
		}
		else {
			//更改连接点的半径值
			/*if (b->v.at(0)->direction == 1)
				b->v.at(0)->rs.at(0) = b->v.at(0)->radius;
			else
				b->v.at(0)->rs.at(sizebv1 - 1) = b->v.at(0)->radius;*/

			a->v.resize(sizea + sizeb);
			for (int i = sizea - 1; i >= 0; --i)
				a->v.at(i + sizeb) = a->v.at(i);
			for (int i = 0; i < sizeb; ++i) {
				a->v.at(sizeb - i - 1) = b->v.at(i);
				b->v.at(i)->bs = a;
				b->v.at(i)->direction = 1 - b->v.at(i)->direction;
			}
		}
	}
}

/*
给基于SegNode的树改成基于SmallSeg的树，树结构没有变，SmallSeg在SegNode的基础上增加了一些信息
*/
SmallSeg* Trans::convert(SegNode* sn, Mat edge) {
	vector<SegNode*> vec1; //保存SegNode*的栈
	vector<SmallSeg*> vec2; //保存SmallSeg*&的栈
	SmallSeg* sss = nullptr, *ss_left, *ss_right, *ss;
	SegNode* t = sn, *t_left, *t_right;
	
	Point p;
	double rd;
	int size;

	if (t) {
		sss = new SmallSeg();
		sss->s = t;
		//增加半径信息
		size = t->pixels.size();
		for (int i = 0; i < size; ++i) {
			p = t->pixels.at(i);
			rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
			sss->rs.push_back(rd);
		}
		//平均半径
		if (size <= 6) {
			double sum = 0;
			for (int i = 0; i < size; ++i)
				sum += sss->rs.at(i);
			sss->radius = (size != 0) ? sum / size : 0;
		}
		else {
			double sum = 0;
			for (int i = 3; i < size - 3; ++i)
				sum += sss->rs.at(i);
			sss->radius = sum / (size - 6);
		}
	}
	else
		return nullptr;

	ss = sss;
convert_L1:
	while (t) {
		vec1.push_back(t);
		vec2.push_back(ss);
		
		t_left = t->left;
		if (t_left) {
			ss_left = new SmallSeg();
			ss_left->s = t_left;
			//增加半径信息
			size = t_left->pixels.size();
			for (int i = 0; i < size; ++i) {
				p = t_left->pixels.at(i);
				rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
				ss_left->rs.push_back(rd);
			}
			//平均半径
			if (size <= 6) {
				double sum = 0;
				for (int i = 0; i < size; ++i)
					sum += ss_left->rs.at(i);
				ss_left->radius = (size != 0) ? sum / size : 0;
			}
			else {
				double sum = 0;
				for (int i = 3; i < size - 3; ++i)
					sum += ss_left->rs.at(i);
				ss_left->radius = sum / (size - 6);
			}
			ss->left = ss_left;
		}
		else
			ss->left = nullptr;

		t_right = t->right;
		if (t_right) {
			ss_right = new SmallSeg();
			ss_right->s = t_right;
			//增加半径信息
			size = t_right->pixels.size();
			for (int i = 0; i < size; ++i) {
				p = t_right->pixels.at(i);
				rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
				ss_right->rs.push_back(rd);
			}
			//平均半径
			if (size <= 6) {
				double sum = 0;
				for (int i = 0; i < size; ++i)
					sum += ss_right->rs.at(i);
				ss_right->radius = (size != 0) ? sum / size : 0;
			}
			else {
				double sum = 0;
				for (int i = 3; i < size - 3; ++i)
					sum += ss_right->rs.at(i);
				ss_right->radius = sum / (size - 6);
			}
			ss->right = ss_right;
		}
		else
			ss->right = nullptr;

		t = t->left;
		ss = ss->left;
	}
	if (vec1.empty()) {
		return sss;
	}
	t = vec1.back(); vec1.pop_back();
	ss = vec2.back(); vec2.pop_back();
	t = t->right;
	ss = ss->right;
	goto convert_L1;
}

/*
找到一个段的家族：自己以及它的儿子们
*/
void Trans::findFamily(SmallSeg* ss, vector<SmallSeg*>& v) {
	if (!ss)
		return;
	v.push_back(ss);
	SmallSeg* s = ss->left;
	while (s) {
		v.push_back(s);
		s = s->right;
	}
}

/*
p: 表示S中所有小段的交叉点
S: 父亲儿子集合
a: 主干部分1
b: 主干部分2
*/
bool Trans::selectMaster(Point p, vector<SmallSeg*>& S, SmallSeg*& a, SmallSeg*& b){
	vector<SmallSeg*> T;

	//对S按radius从大到小排序
	sort(S.begin(), S.end(), comp);
	T.push_back(S.at(0));
	for (int i = 1; i < S.size(); ++i) {
		SmallSeg* s = S.at(i);
		if (i == 1 || (T.at(0)->radius - s->radius) / s->radius < 0.25)
			T.push_back(s);
	}
	//选出最终的主干
	if (T.size() == 1)
		return false;
	else if (T.size() == 2) {
		a = T[0];
		b = T[1];
	}
	else { //根据夹角来选出最优的段
		double fai = 3.0;
		for (int j = 0; j < T.size()-1; ++j) {
			for (int i = j+1; i < T.size(); ++i) {
				double angle = cmptAngle(T.at(j), T.at(i), p);
				if (angle < fai) {
					fai = angle;
					a = T.at(j);
					b = T.at(i);
				}
			}
		}
	}
	return true;
}

bool Trans::comp(const SmallSeg* a, const SmallSeg* b) {
	return a->radius > b->radius;
}

/*
记录vbs中所有大段端点（一个大段的头和尾），出现在哪些大段中和哪个端。
m: pair<int,int>：第一个int表示大段在vbs中的下标，第二个int为0表示在头端，若为1表示在尾端
*/
void Trans::countPoint(vector<BigSeg*>& vbs, map<Point, vector<pair<int,int>>, ptr_less>& m) {
	vector<BigSeg*>::iterator vit;
	map<Point, vector<pair<int, int>>, ptr_less>::iterator it_find;
	Point p1, p2;
	int cnt = 0;

	for (vit = vbs.begin(); vit != vbs.end(); vit++) {
		p1 = ((*vit)->v.at(0)->direction == 1) ? (*vit)->v.at(0)->s->pixels.front() : (*vit)->v.at(0)->s->pixels.back();
		p2 = ((*vit)->v.back()->direction == 1) ? (*vit)->v.back()->s->pixels.back() : (*vit)->v.back()->s->pixels.front();

		cout << "p1=" << p1 << " p2=" << p2 << endl;

		it_find = m.find(p1);
		if (it_find != m.end()) {
			it_find->second.push_back(make_pair(cnt,0));
		}
		else {
			vector<pair<int,int>> vec;
			vec.push_back(make_pair(cnt,0));
			m.insert(make_pair(p1, vec));
		}
		it_find = m.find(p2);
		if (it_find != m.end()) {
			it_find->second.push_back(make_pair(cnt, 1));
		}
		else {
			vector<pair<int, int>> vec;
			vec.push_back(make_pair(cnt, 1));
			m.insert(make_pair(p2, vec));
		}

		cnt++;
	}
}

bool Trans::bigsegContainsPoint(BigSeg* bs, Point p) {
	for (int i = 0; i < bs->v.size(); ++i) {
		vector<Point>& vp = bs->v.at(i)->s->pixels;
		for (int j = 0; j < vp.size(); ++j) {
			if (p == vp.at(j))
				return true;
		}
	}
	return false;
}



/*
重排SegNode中的小段和半径，在一级视野上（一级视野：只考虑最多连续的一个小段）
r: 将SegNode树转换为SmallSeg树的根结点
结果：同restore函数，我们可以进一步挖掘哪些段是相对主干，哪些段是相对支路，这些信息
*/
void Trans::restore(SegNode* sn, Mat edge, vector<BigSeg*>& result, SmallSeg*& rr) {
	SmallSeg* r = convert(sn, edge); rr = r;
	vector<SmallSeg*> vec;
	BigSeg* bs = nullptr;

restore_L1:	
	while (r) {
		vec.push_back(r);
		vector<SmallSeg*> S; //保存r以及它的儿子们
		vector<SmallSeg*> T; //保存半径最大且相近的几个小段
		bool flag; //1表示找到了主干
		SmallSeg *a = nullptr, *b = nullptr; //主干上的段

		findFamily(r, S);
		flag = selectMaster(r->s->pixels.back(), S, a, b); //选主干

		if (flag) { //如果找到了主干(只要没到端点，肯定能找到主干)
			SmallSeg* c = nullptr;
			if (a == r)
				c = b;
			if (b == r)
				c = a;
			if (c) {
				bs = r->bs;
				if (!bs) {
					bs = new BigSeg(); result.push_back(bs);
					bs->v.push_back(r);
					bs->v.push_back(c);
					r->bs = c->bs = bs;
					r->direction = c->direction = 1;
				}
				else {
					c->bs = bs;
					c->direction = r->direction;
					if (c->direction == 0)
						bs->v.insert(bs->v.begin(), c);
					else
						bs->v.push_back(c);
				}
			}
			else {
				bs = new BigSeg(); result.push_back(bs);
				a->bs = b->bs = bs;
				a->direction = 1;
				bs->v.push_back(a);
				b->direction = 0;
				bs->v.insert(bs->v.begin(), b);
			}

			//更新大段之间的主干-支路关系
			for (int i = 0; i < S.size(); ++i) {
				SmallSeg* s = S.at(i);
				if (s != a && s != b) {
					if (!s->bs) {
						s->bs = new BigSeg(); result.push_back(s->bs);
						s->bs->v.push_back(s);
						s->bs->parent = bs;
						s->direction = 1;
					}
					else
						s->bs->parent = bs;
				}
			}

			//重排大段开始和结尾处的半径
			//int sizer = r->s->pixels.size();
			//int sizes;
			///*if (sizer > 6) {
			//	for (int i = sizer - 1; i >= sizer - 3; --i) {
			//		r->rs.at(i) = r->radius;
			//	}
			//}
			//else {
			//	for (int i = 0; i < sizer; ++i)
			//		r->rs.at(i) = r->radius;
			//}*/
			///*if (r != a && r != b)
			//	r->rs.at(sizer - 1) = 0.7*r->radius;*/
			//for (int i = 0; i < S.size(); ++i) {
			//	SmallSeg* s = S.at(i);
			//	if (s != r) {
			//		sizes = s->s->pixels.size();
			//		if (sizes > 6) {
			//			for (int j = 0; j < 3; ++j)
			//				s->rs.at(j) = s->radius;
			//		}
			//		else {
			//			for (int j = 0; j < sizes - 1; ++j)
			//				s->rs.at(j) = s->radius;
			//		}
			//		/*if (s != a && s != b) {
			//			s->rs.at(0) = 0.7*s->radius;
			//		}*/
			//	}
			//}
		}
		else {
			/*r->rs.at(r->rs.size() - 1) = r->radius*0.7;*/
		}

		r = r->left;
	}
	if (vec.empty())
		return;
	SmallSeg* temp = vec.back(); vec.pop_back();
	r = temp->right;
	goto restore_L1;

	/*处理问题：同一个交叉点被很多大段作为开始点或结尾点，这是由于血管图有环路导致的。*/
	map<Point, vector<pair<int,int>>, ptr_less> m;
	map<Point, vector<pair<int, int>>, ptr_less>::iterator it;
	vector<pair<int, int>>::iterator pit;
	SmallSeg *a, *b;
	BigSeg *aa, *bb;
	vector<BigSeg*>::iterator res_it;
restore_L2:
	m.clear();
	countPoint(result, m);
	for (it = m.begin(); it != m.end(); it++) {
		if (it->second.size() > 1) {
			vector<BigSeg*> vec1; //保存出现问题的交叉点周围的大段和对应的小段
			vector<SmallSeg*> vec2; //保存出现问题的交叉点周围的大段和对应的小段
			for (pit = it->second.begin(); pit != it->second.end(); pit++) {
				vec1.push_back(result.at((*pit).first));
				if ((*pit).second == 0)
					vec2.push_back(result.at((*pit).first)->v.front());
				else
					vec2.push_back(result.at((*pit).first)->v.back());
			}

			selectMaster(it->first, vec2, a, b);  //选主干

			//合并大段
			for (int i = 0; i < vec2.size(); ++i) {
				if (vec2.at(i) == a)
					aa = vec1.at(i);
				if (vec2.at(i) == b)
					bb = vec1.at(i);
			}
			merge(aa, bb, it->first);

			//从result中删除bb
			for (res_it = result.begin(); res_it != result.end(); res_it++) {
				if ((*res_it) == bb) {
					result.erase(res_it);
					break;
				}
			}

			goto restore_L2;
		}
	}
}

/*
restore函数产生了BigSeg的集合，但是还要对它做一些处理。比如在vector中重排大段的次序
注意：可能有不止一个大段是根大段（没有父亲的大段）
*/
void Trans::postRestore(vector<BigSeg*>& vbs) {
	vector<BigSeg*> vbs1; //保存新的vbs
	vector<BigSeg*> stk;
	vector<BigSeg*> vec1; //保存未被访问的根大段（父亲段为nullptr的大段）
	BigSeg* a = nullptr;

	//初始化
	for (int i = 0; i < vbs.size(); ++i) {
		if (vbs.at(i)->parent == nullptr) {
			vec1.push_back(vbs.at(i));
		}
	}
	for (int i = 0; i < vec1.size(); ++i) { //将根大段从vbs中删除
		vector<BigSeg*>::iterator it;
		for (it = vbs.begin(); it != vbs.end(); it++) {
			if ((*it) == vec1.at(i)) {
				vbs.erase(it);
				break;
			}
		}
	}
	stk.push_back(vec1.at(0)); //将一个根大段压栈
	vec1.erase(vec1.begin());
	
	while (!stk.empty()) {
		a = stk.back(); stk.pop_back(); //弹栈
		vbs1.push_back(a);

		vector<BigSeg*> temp; //保存是a的儿子的大段
		for (int i = 0; i < vbs.size(); ++i) {
			if (vbs.at(i)->parent == a) {
				stk.push_back(vbs.at(i));
				temp.push_back(vbs.at(i));
			}
		}
		for (int i = 0; i < temp.size(); ++i) { //从vbs中删除temp中的大段
			vector<BigSeg*>::iterator it;
			for (it = vbs.begin(); it != vbs.end(); it++) {
				if ((*it) == temp.at(i)) {
					vbs.erase(it);
					break;
				}
			}
		}

		//选择包含a的端点的未被访问的根大段压栈，并从vec1(未被访问的根大段)中删除此根大段
		vector<BigSeg*>::iterator it;
		for (it = vec1.begin(); it != vec1.end(); ++it) {
			Point p1, p2;
			p1 = (a->v.at(0)->direction == 1) ? a->v.at(0)->s->pixels.at(0) : a->v.at(0)->s->pixels.back();
			p2 = (a->v.back()->direction == 1) ? a->v.back()->s->pixels.back() : a->v.back()->s->pixels.at(0);
			if (bigsegContainsPoint((*it), p1) || bigsegContainsPoint((*it), p2)) {
				stk.push_back((*it));
				vec1.erase(it);
				break;
			}
		}

		//寻找vbs中某个大段，它的端点在a中
		temp.clear();
		for (int i = 0; i < vbs.size(); ++i) {
			Point p1, p2;
			p1 = (vbs.at(i)->v.at(0)->direction == 1) ? vbs.at(i)->v.at(0)->s->pixels.at(0) : vbs.at(i)->v.at(0)->s->pixels.back();
			p2 = (vbs.at(i)->v.back()->direction == 1) ? vbs.at(i)->v.back()->s->pixels.back() : vbs.at(i)->v.back()->s->pixels.at(0);
			if (bigsegContainsPoint(a, p1) || bigsegContainsPoint(a, p2)) {
				stk.push_back(vbs.at(i));
				temp.push_back(vbs.at(i));
			}
		}
		for (int i = 0; i < temp.size(); ++i) { //从vbs中删除temp中的大段
			vector<BigSeg*>::iterator it;
			for (it = vbs.begin(); it != vbs.end(); it++) {
				if ((*it) == temp.at(i)) {
					vbs.erase(it);
					break;
				}
			}
		}

		//寻找a的父亲压栈
		if (a->parent) {
			if (count(vbs.begin(), vbs.end(), a->parent) > 0) {
				stk.push_back(a->parent);
				vector<BigSeg*>::iterator it;
				for (it = vbs.begin(); it != vbs.end(); it++) { //从vbs中删除a->parent
					if ((*it) == a->parent) {
						vbs.erase(it);
						break;
					}
				}
			}
			if (count(vec1.begin(), vec1.end(), a->parent) > 0) {
				stk.push_back(a->parent);
				vector<BigSeg*>::iterator it;
				for (it = vec1.begin(); it != vec1.end(); it++) { //从vbs中删除a->parent
					if ((*it) == a->parent) {
						vec1.erase(it);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < vbs1.size(); ++i) {
		vbs.push_back(vbs1.at(i));
	}
	
}

//目前用不到这个函数
void Trans::reorganise(vector<BigSeg*> &vbs) {
	/*基于一级视野的重组-------------------------------------------------------------------------*/
	/*
	找到每个大段的中间交叉点，再找所有大段中以这个交叉点开始或结尾的大段，分析这个两个大段是否需要重组
	*/
	int i, j, k, size, bs2_index;;
	BigSeg* bs, *bs2;
	map<Point, int, ptr_less> mp;
	map<Point, int, ptr_less>::iterator it_find;
	Point cross;
	SmallSeg *ss1, *ss2, *ss3, *a, *b;
	vector<SmallSeg*> vss;

reorganise_L1:
	for (i = 0; i < vbs.size(); ++i) {
		bs = vbs.at(i);
		bs2 = nullptr; 
		for (j = 0; j < bs->v.size()-1; ++j) {
			
			ss1 = bs->v.at(j);
			cross = (ss1->direction == 1) ? ss1->s->pixels.back() : ss1->s->pixels.front();
			//看这个交叉点是否已经访问过
			it_find = mp.find(cross);
			if (it_find != mp.end())
				continue;

			mp.insert(make_pair(cross, 1));

			ss2 = bs->v.at(j + 1);
			ss3 = nullptr;
			for (k = 0; k < vbs.size(); ++k) {
				bs2 = vbs.at(k);
				Point p1, p2;
				p1 = (bs2->v.front()->direction == 1) ? bs2->v.front()->s->pixels.front() : bs2->v.front()->s->pixels.back();
				p2 = (bs2->v.back()->direction == 1) ? bs2->v.back()->s->pixels.back() : bs2->v.back()->s->pixels.front();
				if (p1 == cross) {
					ss3 = bs2->v.front();
					bs2_index = k;
					break;
				}
				else if (p2 == cross) {
					ss3 = bs2->v.back();
					bs2_index = k;
					break;
				}
				else
					continue;
			}

			if (ss3) {
				vss.clear();
				vss.push_back(ss1);
				vss.push_back(ss2);
				vss.push_back(ss3);

				selectMaster(cross, vss, a, b);

				if ((a == ss1 && b == ss2) || (a == ss2 && b == ss1)) {
					continue;
				} 
				else { //这时需要reorganise
					BigSeg* newbs = new BigSeg();
					if (ss1 == a || ss1 == b) {
						size = bs->v.size();
						for (k = j + 1; k < size; ++k) {
							newbs->v.push_back(bs->v.at(j+1));
							bs->v.at(j+1)->bs = newbs;

							vector<SmallSeg*>::iterator it = bs->v.begin();
							bs->v.erase(it + j + 1);
						}
						newbs->parent = bs;

						merge(bs, bs2, cross);
						delete bs2;
					}
					else {
						size = bs->v.size();
						for (k = 0; k <= j; ++k) {
							newbs->v.push_back(bs->v.at(0));
							bs->v.at(0)->bs = newbs;

							vector<SmallSeg*>::iterator it = bs->v.begin();
							bs->v.erase(it);
						}
						newbs->parent = bs;

						merge(bs, bs2, cross);
						delete bs2;
					}
					vbs.push_back(newbs);
					//从vbs中删除bs2大段
					vbs.erase(vbs.begin() + bs2_index);

					goto reorganise_L1;
				}
			}
		}
	}
	/*基于一级视野的重组结束-------------------------------------------------------------------------*/


}

/*
对每个小段进行降采样，采样步长固定。
半径平均化
缺点：无法完全解决在SV软件中的contour交叉问题
*/
/*void Trans::writeRestoreToFile(vector<BigSeg*>& vbs, int step, string depthPath, ofstream& outfile) {
	vector<BigSeg*> stk;
	BigSeg* cur = nullptr;
	BigSeg* a;
	SmallSeg* ss;
	int i, j, k, size;
	Mat depthImg;
	double x, y, deep;

	if (depthPath != "")
		depthImg = imread(depthPath);

	stk.push_back(cur);
	while (!stk.empty()) {
		cur = stk.back(); stk.pop_back();
		for (i = 0; i < vbs.size(); ++i) {
			if ((a = vbs.at(i))->parent == cur) {
				stk.push_back(a);
				//输出大段
				for (j = 0; j < vbs.at(i)->v.size(); ++j) {
					ss = vbs.at(i)->v.at(j);
					k = (j == 0) ? 0 : step;
					size = ss->s->pixels.size();
					for (k; k < size; k = k + step) {
						if (k + step < size) {
							if (ss->direction == 1) {
								x = ss->s->pixels.at(k).x;
								y = ss->s->pixels.at(k).y;
								deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
								outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
							}
							else {
								x = ss->s->pixels.at(size - 1 - k).x;
								y = ss->s->pixels.at(size - 1 - k).y;
								deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
								outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
							}
						}
					}
					if (ss->direction == 1) {
						x = ss->s->pixels.back().x;
						y = ss->s->pixels.back().y;
						deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
						outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
					}
					else {
						x = ss->s->pixels.front().x;
						y = ss->s->pixels.front().y;
						deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
						outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
					}
				}
				outfile << "\n";
			}
		}
	}
}*/

//downsample策略是：舍弃离末尾点距离小于step的点。
//先运行postRestore函数，再运行writeRestoreToFile
void Trans::writeRestoreToFile(vector<BigSeg*>& vbs, int step, string depthPath, ofstream& outfile) {
	BigSeg* a;
	SmallSeg* ss;
	Mat depthImg;
	double x, y, deep;
	int i, j, k, size;

	if (depthPath != "")
		depthImg = imread(depthPath);

	for (int i = 0; i < vbs.size(); ++i) {
		//输出大段
		for (j = 0; j < vbs.at(i)->v.size(); ++j) {
			ss = vbs.at(i)->v.at(j);
			k = 0;
			size = ss->s->pixels.size();
			
			if (j == 0) { //输出第一个小段的第一个点
				if (ss->direction == 1) {
					x = ss->s->pixels.at(k).x;
					y = ss->s->pixels.at(k).y;
					deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
					outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
				}
				else {
					x = ss->s->pixels.at(size-1-k).x;
					y = ss->s->pixels.at(size-1-k).y;
					deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
					outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
				}
			}
			k = k + step;
			for (k; k < size; k = k + step) {
				if (k + step < size) {
					if (ss->direction == 1) {
						x = ss->s->pixels.at(k).x;
						y = ss->s->pixels.at(k).y;
						deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
						outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
					}
					else {
						x = ss->s->pixels.at(size - 1 - k).x;
						y = ss->s->pixels.at(size - 1 - k).y;
						deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
						outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
					}
				}
			}
			if (ss->direction == 1) {
				x = ss->s->pixels.back().x;
				y = ss->s->pixels.back().y;
				deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
				outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
			}
			else {
				x = ss->s->pixels.front().x;
				y = ss->s->pixels.front().y;
				deep = (depthPath == "") ? 0 : depth(depthImg, x, y);
				outfile << x << " " << y << " " << deep << " " << ss->radius << " ";
			}
		}

		outfile << "\n";
	}
}



//目前用不到这个函数
/*
根据大段来降采样，需要解决的问题是：主干与支路可能偏移，造成支路嵌入主干失败；有些小段半径值非常大；一个大段的半径起伏较大；
*/
void Trans::writeRestoreToFile2(vector<BigSeg*>& vbs, ofstream& outfile) {
	vector<BigSeg*> stk;
	vector<vector<Point4f>*> stk2;
	BigSeg *cur = nullptr, *a;
	vector<Point4f> *vp = nullptr, *vp2 = nullptr;
	SmallSeg* ss;
	int i, j, k, step, size;

	stk.push_back(cur);
	while (!stk.empty()) {
		cur = stk.back(); stk.pop_back();
		if (cur) {
			vp2 = stk2.back(); stk2.pop_back();
		}
		for (i = 0; i < vbs.size(); ++i) {
			step = 0;
			if ((a = vbs.at(i))->parent == cur) {
				stk.push_back(a); 
				vp = new vector<Point4f>;
				stk2.push_back(vp);

				//将点的信息集合到temp中
				vector<Point4f> temp;
				for (j = 0; j < a->v.size(); ++j) {
					ss = vbs.at(i)->v.at(j);
					k = (j == 0) ? 0 : 1;
					size = ss->s->pixels.size();
					for (k; k < size; k++) {
						if (ss->direction == 1) {
							temp.push_back(Point4f(double(ss->s->pixels.at(k).x), double(ss->s->pixels.at(k).y), 0, double(ss->rs.at(k))));
						}
						else {
							temp.push_back(Point4f(double(ss->s->pixels.at(size - 1 - k).x), double(ss->s->pixels.at(size - 1 - k).y), 0, double(ss->rs.at(size - 1 - k))));
						}
					}
				}
				//保存降采样的大段信息到vp，输出大段
				for (j = 0; j < temp.size(); j = j + step) {
					if (j == 0 || j + step < temp.size()) {
						outfile << temp.at(j).x << " " << temp.at(j).y << " " << temp.at(j).z << " " << temp.at(j).r << " ";
						vp->push_back(Point4f(temp.at(j).x, temp.at(j).y, temp.at(j).z, temp.at(j).r));
						step = ceil(temp.at(j).r) + 1;
					}
				}
				outfile << temp.back().x << " " << temp.back().y << " " << temp.back().z << " " << temp.back().r << " ";
				outfile << "\n";
			}
		}
		if (cur)
			delete vp2;
	}
}

