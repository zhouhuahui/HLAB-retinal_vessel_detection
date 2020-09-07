#include"pch.h"
#include"trans.h"
#include"vessel_radius.h"
#include"vessel_depth.h"
#include<cmath>
#include<algorithm>
#include<map>

/*
һ����Ұ��ֻ�������������һ��С��
*/

/*
������ǰ3����
	  ����㴦֧·�뾶��Ϊ0.7��ƽ���뾶
*/

/*��������С�Σ�����faiֵ��fai = -[cos(alpha)*cos(byta) + sin(alpha)*sin(byta)]
faiֵ��[0,2]������
*/
double Trans::cmptAngle(SmallSeg* a, SmallSeg* b, Point p) {
	double aa[4]; //��������С�εĵ�λ������
	int sizea = a->s->pixels.size();
	int sizeb = b->s->pixels.size();
	Point p1, p2;
	double sum1,sum2;
	double sin_alpha, cos_alpha, sin_byta, cos_byta, cos_thita;

	/*����a������*/
	if (sizea <= 12) { //���С�β�����
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
	/*����b������*/
	if (sizeb <= 12) { //���С�β�����
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

	/*��������seg�ļн�*/
	sin_alpha = aa[1] / sum1;
	cos_alpha = aa[0] / sum1;
	sin_byta = aa[3] / sum2;
	cos_byta = aa[2] / sum2;
	cos_thita = cos_alpha * cos_byta + sin_alpha * sin_byta;

	return -cos_thita;
};

/*�Ѵ��a�ʹ��b�ڽ����p���ϲ���a��
*/
void Trans::merge(BigSeg* a, BigSeg* b, Point p) {
	int sizea = a->v.size(); 
	int sizeb = b->v.size(); 
	int sizeav1 = a->v.at(0)->s->pixels.size();
	int sizeav2 = a->v.at(sizea-1)->s->pixels.size();
	int sizebv1 = b->v.at(0)->s->pixels.size();
	int sizebv2 = b->v.at(sizeb - 1)->s->pixels.size();
	int flaga = 0, flagb = 0; //flaga = 1,��ʾp��a��β��
	SmallSeg* s;

	if ( (a->v.at(sizea - 1)->direction == 1 && p == a->v.at(sizea - 1)->s->pixels.at(sizeav2 - 1)) || (a->v.at(sizea - 1)->direction == 0 && p == a->v.at(sizea - 1)->s->pixels.at(0)))
		flaga = 1;
	if ( (b->v.at(sizeb-1)->direction == 1 && p == b->v.at(sizeb - 1)->s->pixels.at(sizebv2 - 1)) || (b->v.at(sizeb - 1)->direction == 0 && p == b->v.at(sizeb - 1)->s->pixels.at(0)))
		flagb = 1;

	if (flaga) {
		//a->v.back()->rs.at(sizeav2 - 1) = a->v.back()->radius; //�������ӵ�İ뾶ֵ
		if (flagb) {
			//b->v.back()->rs.at(sizebv2 - 1) = b->v.back()->radius; //�������ӵ�İ뾶ֵ
			for (int i = sizeb - 1; i >= 0; --i) {
				s = b->v.at(i);
				s->direction = 1 - s->direction;
				s->bs = a;
				a->v.push_back(s);
			}
		}
		else {
			//�������ӵ�İ뾶ֵ
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
		//�������ӵ�İ뾶ֵ
		/*if (a->v.at(0)->direction == 1)
			a->v.at(0)->rs.at(0) = a->v.at(0)->radius;
		else
			a->v.at(0)->rs.at(sizeav1 - 1) = a->v.at(0)->radius;*/

		if (flagb) {
			//b->v.back()->rs.at(sizebv2 - 1) = b->v.back()->radius; //�������ӵ�İ뾶ֵ
			a->v.resize(sizea + sizeb);
			for (int i = sizea - 1; i >= 0; --i)
				a->v.at(i + sizeb) = a->v.at(i);
			for (int i = sizeb - 1; i >= 0; --i) {
				a->v.at(i) = b->v.at(i);
				b->v.at(i)->bs = a;
			}
		}
		else {
			//�������ӵ�İ뾶ֵ
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
������SegNode�����ĳɻ���SmallSeg���������ṹû�б䣬SmallSeg��SegNode�Ļ�����������һЩ��Ϣ
*/
SmallSeg* Trans::convert(SegNode* sn, Mat edge) {
	vector<SegNode*> vec1; //����SegNode*��ջ
	vector<SmallSeg*> vec2; //����SmallSeg*&��ջ
	SmallSeg* sss = nullptr, *ss_left, *ss_right, *ss;
	SegNode* t = sn, *t_left, *t_right;
	
	Point p;
	double rd;
	int size;

	if (t) {
		sss = new SmallSeg();
		sss->s = t;
		//���Ӱ뾶��Ϣ
		size = t->pixels.size();
		for (int i = 0; i < size; ++i) {
			p = t->pixels.at(i);
			rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
			sss->rs.push_back(rd);
		}
		//ƽ���뾶
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
			//���Ӱ뾶��Ϣ
			size = t_left->pixels.size();
			for (int i = 0; i < size; ++i) {
				p = t_left->pixels.at(i);
				rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
				ss_left->rs.push_back(rd);
			}
			//ƽ���뾶
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
			//���Ӱ뾶��Ϣ
			size = t_right->pixels.size();
			for (int i = 0; i < size; ++i) {
				p = t_right->pixels.at(i);
				rd = (edge.data) ? radius(edge, p.x, p.y) : 2;
				ss_right->rs.push_back(rd);
			}
			//ƽ���뾶
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
�ҵ�һ���εļ��壺�Լ��Լ����Ķ�����
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
p: ��ʾS������С�εĽ����
S: ���׶��Ӽ���
a: ���ɲ���1
b: ���ɲ���2
*/
bool Trans::selectMaster(Point p, vector<SmallSeg*>& S, SmallSeg*& a, SmallSeg*& b){
	vector<SmallSeg*> T;

	//��S��radius�Ӵ�С����
	sort(S.begin(), S.end(), comp);
	T.push_back(S.at(0));
	for (int i = 1; i < S.size(); ++i) {
		SmallSeg* s = S.at(i);
		if (i == 1 || (T.at(0)->radius - s->radius) / s->radius < 0.25)
			T.push_back(s);
	}
	//ѡ�����յ�����
	if (T.size() == 1)
		return false;
	else if (T.size() == 2) {
		a = T[0];
		b = T[1];
	}
	else { //���ݼн���ѡ�����ŵĶ�
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
��¼vbs�����д�ζ˵㣨һ����ε�ͷ��β������������Щ����к��ĸ��ˡ�
m: pair<int,int>����һ��int��ʾ�����vbs�е��±꣬�ڶ���intΪ0��ʾ��ͷ�ˣ���Ϊ1��ʾ��β��
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
����SegNode�е�С�κͰ뾶����һ����Ұ�ϣ�һ����Ұ��ֻ�������������һ��С�Σ�
r: ��SegNode��ת��ΪSmallSeg���ĸ����
�����ͬrestore���������ǿ��Խ�һ���ھ���Щ����������ɣ���Щ�������֧·����Щ��Ϣ
*/
void Trans::restore(SegNode* sn, Mat edge, vector<BigSeg*>& result, SmallSeg*& rr) {
	SmallSeg* r = convert(sn, edge); rr = r;
	vector<SmallSeg*> vec;
	BigSeg* bs = nullptr;

restore_L1:	
	while (r) {
		vec.push_back(r);
		vector<SmallSeg*> S; //����r�Լ����Ķ�����
		vector<SmallSeg*> T; //����뾶���������ļ���С��
		bool flag; //1��ʾ�ҵ�������
		SmallSeg *a = nullptr, *b = nullptr; //�����ϵĶ�

		findFamily(r, S);
		flag = selectMaster(r->s->pixels.back(), S, a, b); //ѡ����

		if (flag) { //����ҵ�������(ֻҪû���˵㣬�϶����ҵ�����)
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

			//���´��֮�������-֧·��ϵ
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

			//���Ŵ�ο�ʼ�ͽ�β���İ뾶
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

	/*�������⣺ͬһ������㱻�ܶ�����Ϊ��ʼ����β�㣬��������Ѫ��ͼ�л�·���µġ�*/
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
			vector<BigSeg*> vec1; //�����������Ľ������Χ�Ĵ�κͶ�Ӧ��С��
			vector<SmallSeg*> vec2; //�����������Ľ������Χ�Ĵ�κͶ�Ӧ��С��
			for (pit = it->second.begin(); pit != it->second.end(); pit++) {
				vec1.push_back(result.at((*pit).first));
				if ((*pit).second == 0)
					vec2.push_back(result.at((*pit).first)->v.front());
				else
					vec2.push_back(result.at((*pit).first)->v.back());
			}

			selectMaster(it->first, vec2, a, b);  //ѡ����

			//�ϲ����
			for (int i = 0; i < vec2.size(); ++i) {
				if (vec2.at(i) == a)
					aa = vec1.at(i);
				if (vec2.at(i) == b)
					bb = vec1.at(i);
			}
			merge(aa, bb, it->first);

			//��result��ɾ��bb
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
restore����������BigSeg�ļ��ϣ����ǻ�Ҫ������һЩ����������vector�����Ŵ�εĴ���
ע�⣺�����в�ֹһ������Ǹ���Σ�û�и��׵Ĵ�Σ�
*/
void Trans::postRestore(vector<BigSeg*>& vbs) {
	vector<BigSeg*> vbs1; //�����µ�vbs
	vector<BigSeg*> stk;
	vector<BigSeg*> vec1; //����δ�����ʵĸ���Σ����׶�Ϊnullptr�Ĵ�Σ�
	BigSeg* a = nullptr;

	//��ʼ��
	for (int i = 0; i < vbs.size(); ++i) {
		if (vbs.at(i)->parent == nullptr) {
			vec1.push_back(vbs.at(i));
		}
	}
	for (int i = 0; i < vec1.size(); ++i) { //������δ�vbs��ɾ��
		vector<BigSeg*>::iterator it;
		for (it = vbs.begin(); it != vbs.end(); it++) {
			if ((*it) == vec1.at(i)) {
				vbs.erase(it);
				break;
			}
		}
	}
	stk.push_back(vec1.at(0)); //��һ�������ѹջ
	vec1.erase(vec1.begin());
	
	while (!stk.empty()) {
		a = stk.back(); stk.pop_back(); //��ջ
		vbs1.push_back(a);

		vector<BigSeg*> temp; //������a�Ķ��ӵĴ��
		for (int i = 0; i < vbs.size(); ++i) {
			if (vbs.at(i)->parent == a) {
				stk.push_back(vbs.at(i));
				temp.push_back(vbs.at(i));
			}
		}
		for (int i = 0; i < temp.size(); ++i) { //��vbs��ɾ��temp�еĴ��
			vector<BigSeg*>::iterator it;
			for (it = vbs.begin(); it != vbs.end(); it++) {
				if ((*it) == temp.at(i)) {
					vbs.erase(it);
					break;
				}
			}
		}

		//ѡ�����a�Ķ˵��δ�����ʵĸ����ѹջ������vec1(δ�����ʵĸ����)��ɾ���˸����
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

		//Ѱ��vbs��ĳ����Σ����Ķ˵���a��
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
		for (int i = 0; i < temp.size(); ++i) { //��vbs��ɾ��temp�еĴ��
			vector<BigSeg*>::iterator it;
			for (it = vbs.begin(); it != vbs.end(); it++) {
				if ((*it) == temp.at(i)) {
					vbs.erase(it);
					break;
				}
			}
		}

		//Ѱ��a�ĸ���ѹջ
		if (a->parent) {
			if (count(vbs.begin(), vbs.end(), a->parent) > 0) {
				stk.push_back(a->parent);
				vector<BigSeg*>::iterator it;
				for (it = vbs.begin(); it != vbs.end(); it++) { //��vbs��ɾ��a->parent
					if ((*it) == a->parent) {
						vbs.erase(it);
						break;
					}
				}
			}
			if (count(vec1.begin(), vec1.end(), a->parent) > 0) {
				stk.push_back(a->parent);
				vector<BigSeg*>::iterator it;
				for (it = vec1.begin(); it != vec1.end(); it++) { //��vbs��ɾ��a->parent
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

//Ŀǰ�ò����������
void Trans::reorganise(vector<BigSeg*> &vbs) {
	/*����һ����Ұ������-------------------------------------------------------------------------*/
	/*
	�ҵ�ÿ����ε��м佻��㣬�������д�������������㿪ʼ���β�Ĵ�Σ����������������Ƿ���Ҫ����
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
			//�����������Ƿ��Ѿ����ʹ�
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
				else { //��ʱ��Ҫreorganise
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
					//��vbs��ɾ��bs2���
					vbs.erase(vbs.begin() + bs2_index);

					goto reorganise_L1;
				}
			}
		}
	}
	/*����һ����Ұ���������-------------------------------------------------------------------------*/


}

/*
��ÿ��С�ν��н����������������̶���
�뾶ƽ����
ȱ�㣺�޷���ȫ�����SV����е�contour��������
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
				//������
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

//downsample�����ǣ�������ĩβ�����С��step�ĵ㡣
//������postRestore������������writeRestoreToFile
void Trans::writeRestoreToFile(vector<BigSeg*>& vbs, int step, string depthPath, ofstream& outfile) {
	BigSeg* a;
	SmallSeg* ss;
	Mat depthImg;
	double x, y, deep;
	int i, j, k, size;

	if (depthPath != "")
		depthImg = imread(depthPath);

	for (int i = 0; i < vbs.size(); ++i) {
		//������
		for (j = 0; j < vbs.at(i)->v.size(); ++j) {
			ss = vbs.at(i)->v.at(j);
			k = 0;
			size = ss->s->pixels.size();
			
			if (j == 0) { //�����һ��С�εĵ�һ����
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



//Ŀǰ�ò����������
/*
���ݴ��������������Ҫ����������ǣ�������֧·����ƫ�ƣ����֧·Ƕ������ʧ�ܣ���ЩС�ΰ뾶ֵ�ǳ���һ����εİ뾶����ϴ�
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

				//�������Ϣ���ϵ�temp��
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
				//���潵�����Ĵ����Ϣ��vp��������
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

