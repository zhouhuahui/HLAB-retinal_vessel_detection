#include "pch.h"
#include "trans.h"
#include<iostream>

using namespace std;

//for 测试cmptAngle函数
SmallSeg* createSS();
//for 测试merge()函数
BigSeg* createBS(int x1, int y1, int x2, int y2, int direction1, double r1, double r2, double radius1, int x3, int y3, int x4, int y4, int direction3, double r3, double r4, double radius3);
void outputBigSeg(BigSeg* bs);
void deleteBS(BigSeg* bs);
//for 测试convert函数
void outputSmallSegBinTree(SmallSeg* ss);
void deleteSegNodeTree(SegNode* sn);
void deleteSmallSegTree(SmallSeg* ss);



int main() {
	Trans* trans = new Trans();

	/*测试cmptAngle函数---------------------------------------------------------------------*/
	//第一次测试
	/*SmallSeg *a = createSS(), *b = createSS();
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(1, 1));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(0, 0));
	cout << "a和b的fai值是:" << trans->cmptAngle(a, b,Point(0,0));
	delete a; delete b; delete trans;*/
	//第二次测试
	/*SmallSeg *a = createSS(), *b = createSS();
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(0, 0));
	a->s->pixels.push_back(Point(1, 1));

	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(-1, 0));
	b->s->pixels.push_back(Point(0, 0));
	b->s->pixels.push_back(Point(0, 0));
	b->s->pixels.push_back(Point(0, 0));
	b->s->pixels.push_back(Point(0, 0));
	cout << "a和b的fai值是:" << trans->cmptAngle(a, b, Point(0, 0));
	delete a; delete b; delete trans;*/
	/*测试cmptAngle函数完毕---------------------------------------------------------------------------------*/

	/*测试merge()函数------------------------------------------------------------------------------------------*/
	//BigSeg *bsa, *bsb;
	////测试1
	///*bsa = createBS(303, 25, 300, 36, 1, 1, 2, 1.5, 300, 36, 283, 87, 1, 1, 2, 1.5);
	//bsb = createBS(312, 105, 296, 97, 1, 1, 2, 1.5, 296, 97, 283, 87, 1, 1, 2, 1.5);
	//trans->merge(bsa, bsb, Point(283, 87));
	//outputBigSeg(bsa);*/
	////测试2
	///*bsa = createBS(303, 25, 300, 36, 1, 1, 2, 1.5, 300, 36, 283, 87, 1, 1, 2, 1.5);
	//bsb = createBS(283, 87, 296, 97, 1, 1, 2, 1.5, 296, 97, 312, 105, 1, 1, 2, 1.5);
	//trans->merge(bsa, bsb, Point(283, 87));
	//outputBigSeg(bsa);*/
	////测试3,4
	///*bsa = createBS(303, 25, 300, 36, 1, 1, 2, 1.5, 300, 36, 283, 87, 1, 1, 2, 1.5);
	//bsb = createBS(296, 97, 312, 105, 0, 1, 2, 1.5, 296, 97, 283, 87, 1, 1, 2, 1.5);
	//trans->merge(bsa, bsb, Point(283, 87));
	//outputBigSeg(bsa);
	//deleteBS(bsa);*/
	////测试5
	///*bsa = createBS(283, 87, 300, 36, 1, 1, 2, 1.5, 300, 36, 303, 25, 1, 1, 2, 1.5);
	//bsb = createBS(312, 105, 296, 97, 1, 1, 2, 1.5, 296, 97, 283, 87, 1, 1, 2, 1.5);
	//trans->merge(bsa, bsb, Point(283, 87));
	//outputBigSeg(bsa);
	//deleteBS(bsa);*/
	////测试6
	//bsa = createBS(283, 87, 300, 36, 1, 1, 2, 1.5, 300, 36, 303, 25, 1, 1, 2, 1.5);
	//bsb = createBS(283, 87, 296, 97, 1, 1, 2, 1.5, 296, 97, 312, 105, 1, 1, 2, 1.5);
	//trans->merge(bsa, bsb, Point(283, 87));
	//outputBigSeg(bsa);
	//deleteBS(bsa);
	/*测试merge()函数结束----------------------------------------------------------------------------------------------*/

	/*测试convert()函数-----------------------------------------------------------------------------------------------*/
	/*SegNode *sn, *sn1, *sn2;
	SmallSeg* ss;
	sn = sn1 = new SegNode(0);
	sn2 = new SegNode(1); sn1->left = sn2; 
	sn2 = new SegNode(2); sn1->right = sn2; sn1 = sn2;
	sn2 = new SegNode(3); sn1->left = sn2;
	sn2 = new SegNode(4); sn1->right = sn2;
	ss = trans->convert(sn, Mat());
	outputSmallSegBinTree(ss);
	deleteSegNodeTree(sn);
	deleteSmallSegTree(ss);*/
	/*测试convert()函数结束----------------------------------------------------------------------------------*/

	/*测试findFamily()函数-------------------------------------------------------------------------*/
	//SegNode *sn, *sn1, *sn2;
	//SmallSeg* ss;
	//sn = sn1 = new SegNode(0);
	//sn2 = new SegNode(1); sn1->left = sn2; sn1 = sn2;
	//sn2 = new SegNode(2); sn1->right = sn2; sn1 = sn2;
	//sn2 = new SegNode(3); sn1->right = sn2;
	//ss = trans->convert(sn, Mat());
	//vector<SmallSeg*> vss;
	//trans->findFamily(ss, vss);
	////output
	//vector<SmallSeg*>::iterator it;
	//for (it = vss.begin(); it != vss.end(); it++) {
	//	cout << "--------------" << endl;
	//	cout << (*it)->s->segName << ": ";
	//	vector<Point>::iterator pit;
	//	for (pit = (*it)->s->pixels.begin(); pit != (*it)->s->pixels.end(); pit++) {
	//		cout << (*pit) << " ";
	//	}
	//	cout << endl;
	//}
	////delete
	//deleteSegNodeTree(sn);
	//deleteSmallSegTree(ss);
	/*测试findFamily()函数结束------------------------------------------------------------------------------------*/

	/*测试selectMaster()函数-------------------------------------------------------------------------------------------------------*/
	////测试1
	////SmallSeg *f = new SmallSeg(), *ch1 = new SmallSeg(), *ch2 = new SmallSeg(), *a, *b;
	////SegNode* s;
	////vector<SmallSeg*> v;
	////s = new SegNode(0);
	////s->pixels.push_back(Point(303, 25)); s->pixels.push_back(Point(303, 27)); s->pixels.push_back(Point(302,29)); s->pixels.push_back(Point(301, 31)); s->pixels.push_back(Point(300, 33)); s->pixels.push_back(Point(300, 36));
	////f->s = s; f->radius = 1.1; 
	////s = new SegNode(0);
	////s->pixels.push_back(Point(300, 36)); s->pixels.push_back(Point(298,37)); s->pixels.push_back(Point(296,39)); s->pixels.push_back(Point(295,41)); s->pixels.push_back(Point(294,43)); s->pixels.push_back(Point(292,45)); s->pixels.push_back(Point(291,47)); s->pixels.push_back(Point(290,49)); s->pixels.push_back(Point(289,51)); s->pixels.push_back(Point(289,53)); s->pixels.push_back(Point(289,55)); s->pixels.push_back(Point(288,57)); s->pixels.push_back(Point(288,59)); s->pixels.push_back(Point(287,61)); s->pixels.push_back(Point(283,87));
	////ch1->s = s; ch1->radius = 1;
	////s = new SegNode(0);
	////s->pixels.push_back(Point(300, 36)); s->pixels.push_back(Point(303,38)); s->pixels.push_back(Point(303,40)); s->pixels.push_back(Point(305,41)); s->pixels.push_back(Point(306,44));
	////ch2->s = s; ch2->radius = 1;
	////v.push_back(f); v.push_back(ch1); v.push_back(ch2);
	////
	////cout << "test angle = " << trans->cmptAngle(f, ch1, Point(300, 36));

	////trans->selectMaster(Point(300, 36), v, a, b);
	////outputSmallSegBinTree(a);
	////outputSmallSegBinTree(b);
	////
	//////delete
	////vector<SmallSeg*>::iterator it;
	////for (it = v.begin(); it != v.end(); it++) {
	////	delete (*it)->s;
	////	delete (*it);
	////}

	////测试2
	//SmallSeg *f = new SmallSeg(), *ch1 = new SmallSeg(), *ch2 = new SmallSeg(), *a, *b;
	//SegNode* s;
	//vector<SmallSeg*> v;
	//s = new SegNode(0);
	//s->pixels.push_back(Point(300, 36)); s->pixels.push_back(Point(298, 37)); s->pixels.push_back(Point(296, 39)); s->pixels.push_back(Point(295, 41)); s->pixels.push_back(Point(294, 43)); s->pixels.push_back(Point(292, 45)); s->pixels.push_back(Point(291, 47)); s->pixels.push_back(Point(290, 49)); s->pixels.push_back(Point(289, 51)); s->pixels.push_back(Point(289, 53)); s->pixels.push_back(Point(289, 55)); s->pixels.push_back(Point(288, 57)); s->pixels.push_back(Point(288, 59)); s->pixels.push_back(Point(287, 61)); s->pixels.push_back(Point(287,63)); s->pixels.push_back(Point(287, 65)); s->pixels.push_back(Point(287,67)); s->pixels.push_back(Point(286,71)); s->pixels.push_back(Point(286,73)); s->pixels.push_back(Point(286, 75)); s->pixels.push_back(Point(286, 77)); s->pixels.push_back(Point(286, 79)); s->pixels.push_back(Point(286, 81)); s->pixels.push_back(Point(286, 84)); s->pixels.push_back(Point(285,85)); s->pixels.push_back(Point(283,87));
	//f->s = s; f->radius = 2;
	//s = new SegNode(0);
	//s->pixels.push_back(Point(283, 87)); s->pixels.push_back(Point(281,86)); s->pixels.push_back(Point(279,85)); s->pixels.push_back(Point(277,84)); s->pixels.push_back(Point(275, 83));
	//ch1->s = s; ch1->radius = 3;
	//s = new SegNode(0);
	//s->pixels.push_back(Point(283, 89)); s->pixels.push_back(Point(283,88)); s->pixels.push_back(Point(283, 87));
	//ch2->s = s; ch2->radius = 5.5;
	//v.push_back(f); v.push_back(ch1); v.push_back(ch2);

	//trans->selectMaster(Point(300, 36), v, a, b);
	//outputSmallSegBinTree(a);
	//outputSmallSegBinTree(b);

	////delete
	//vector<SmallSeg*>::iterator it;
	//for (it = v.begin(); it != v.end(); it++) {
	//	delete (*it)->s;
	//	delete (*it);
	//}

	/*测试selectMaster()函数结束-------------------------------------------------------------------------------------*/

	/*测试countPoint()函数-------------------------------------------------------------------------------------------*/
	//BigSeg *a, *b, *c;
	//vector<BigSeg*> vbs;
	//map<Point, vector<pair<int, int>>, ptr_less> m;
	//map<Point, vector<pair<int, int>>, ptr_less>::iterator mit;
	//vector<pair<int, int>>::iterator vit;
	//a = createBS(434, 276, 443, 275, 1, 1, 1, 1, 443, 275, 447, 276, 1, 1, 1, 1);
	//b = createBS(461, 257, 453, 263, 1, 1, 1, 1, 453, 263, 447, 276, 1, 1, 1, 1);
	//c = createBS(453, 302, 449, 287, 1, 1, 1, 1, 449, 287, 447, 276, 1, 1, 1, 1);
	//vbs.push_back(a);
	//vbs.push_back(b);
	//vbs.push_back(c);
	//trans->countPoint(vbs, m);

	//for (mit = m.begin(); mit != m.end(); mit++) {
	//	cout << "------------------------" << endl;
	//	cout << (*mit).first << endl;
	//	for (vit = (*mit).second.begin(); vit != (*mit).second.end(); vit++) {
	//		cout << "(" << (*vit).first << "," << (*vit).second << ")";
	//	}
	//	cout << endl;
	//}
	////delete 
	//deleteBS(a);
	//deleteBS(b);
	//deleteBS(c);
	
	/*测试countPoint()函数结束--------------------------------------------------------------------------------------*/
	
	return 0;
};

//------------------------------------------------------------------------------------------------
SmallSeg* createSS() {
	SmallSeg* s = new SmallSeg();
	s->s = new SegNode(0);
	return s;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BigSeg* createBS(int x1, int y1, int x2, int y2, int direction1, double r1, double r2, double radius1, int x3, int y3, int x4, int y4, int direction3, double r3, double r4, double radius3) {
	BigSeg* bsa;
	SmallSeg* ss;
	SegNode* sn;
	bsa = new BigSeg();
	ss = new SmallSeg();
	sn = new SegNode(0);
	sn->pixels.push_back(Point(x1, y1));
	sn->pixels.push_back(Point(x2, y2));
	ss->s = sn;
	ss->bs = bsa;
	ss->direction = direction1;
	ss->rs.push_back(r1);
	ss->rs.push_back(r2);
	ss->radius = radius1;
	bsa->v.push_back(ss);
	ss = new SmallSeg();
	sn = new SegNode(0);
	sn->pixels.push_back(Point(x3, y3));
	sn->pixels.push_back(Point(x4, y4));
	ss->s = sn;
	ss->bs = bsa;
	ss->direction = direction3;
	ss->rs.push_back(r3);
	ss->rs.push_back(r4);
	ss->radius = radius3;
	bsa->v.push_back(ss);

	return bsa;
}

/*
输出BigSeg的每个小段的点，每个小段的平均半径值
*/
void outputBigSeg(BigSeg* bs) {
	vector<SmallSeg*>::iterator it;
	vector<Point>::iterator it2;
	for (it = bs->v.begin(); it != bs->v.end(); it++) {
		SegNode* s = (*it)->s;
		cout << "----------------------" << endl;
		for (it2 = s->pixels.begin(); it2 != s->pixels.end(); it2++) {
			cout << "(" << (*it2).x << "," << (*it2).y << ") ";
		}
		cout << "direction=" << (*it)->direction << " ";
		cout << "小段开头处r=" << (*it)->rs.front() << " 小段结尾处半径r=" << (*it)->rs.back() << " ";
		cout << "radius=" << (*it)->radius << endl;
	}
}

/*
若bigseg中的小段无树状结构，只是为了测试，则执行此delete
*/
void deleteBS(BigSeg* bs) {
	for (int i = 0; i < bs->v.size(); ++i) {
		delete bs->v.at(i)->s;
		delete bs->v.at(i);
	}
	delete bs;
}
//-------------------------------------------------------------------------------------------------------------

void outputSmallSegBinTree(SmallSeg* ss) {
	if (ss) {
		cout << "--------------" << endl;
		cout << ss->s->segName << ": ";
		vector<Point>::iterator it;
		for (it = ss->s->pixels.begin(); it != ss->s->pixels.end(); it++) {
			cout << (*it) << " ";
		}
		cout << endl;
		outputSmallSegBinTree(ss->left);
		outputSmallSegBinTree(ss->right);
	}
	else {
		cout << "--------------" << endl;
		cout << "#" << endl;
	}
	
}
void deleteSegNodeTree(SegNode* sn) {
	if (sn) {
		if (!sn->left)
			deleteSegNodeTree(sn->left);
		if (!sn->right)
			deleteSegNodeTree(sn->right);
		delete sn;
	}
}
void deleteSmallSegTree(SmallSeg* ss) {
	if (ss) {
		if (!ss->left)
			deleteSmallSegTree(ss->left);
		if (!ss->right)
			deleteSmallSegTree(ss->right);
		delete ss;
	}
}
