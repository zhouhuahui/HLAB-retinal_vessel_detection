#pragma once
#include"iterate_tree.h"
#include<vector>
#include<map>

typedef struct BigSeg BigSeg;
/*
С�Σ��������������֮��Ķ�
*/
typedef struct SmallSeg {
	SegNode* s;
	BigSeg* bs; /*���С�δ����ĸ����*/
	int direction; /*ȡֵΪ{0��1}��0��ʾ��תSegNode�е��˳��*/
	double radius;
	vector<double> rs;
	struct SmallSeg* left;
	struct SmallSeg* right;
	SmallSeg() {
		s = nullptr;
		direction = -1;
		bs = nullptr;
		left = right = nullptr;
	}
}SmallSeg;

/*
�ܶ�������С����ɵĴ�Σ���Ѫ��ͼ�д��¶�Ӧһ��������Ѫ��
*/
struct BigSeg {
	vector<SmallSeg*> v;
	BigSeg* parent;
	BigSeg() {
		v = vector<SmallSeg*>();
		parent = nullptr;
	}
};

struct ptr_less : public binary_function<Point, Point, bool>
{
	bool operator()(const Point& a, const Point& b) const
	{
		if (a.x > b.x)
			return true;
		else if (a.x == b.x && a.y > b.y)
			return true;
		else return false;
	}
};

/*
���ܣ�Ϊ��ת��SegTree��ʹ������SimVascular�������Ҳ�����
*/
class Trans {
public:
	double cmptAngle(SmallSeg* a, SmallSeg* b, Point p);
	void merge(BigSeg* a, BigSeg* b, Point p);
	SmallSeg* convert(SegNode* sn, Mat edge);
	void findFamily(SmallSeg* ss, vector<SmallSeg*>& v);
	bool selectMaster(Point p, vector<SmallSeg*>& S, SmallSeg*& a, SmallSeg*& b);
	static bool comp(const SmallSeg* a, const SmallSeg* b);
	void countPoint(vector<BigSeg*>& vbs, map<Point, vector<pair<int,int>>, ptr_less>& m);
	bool bigsegContainsPoint(BigSeg* bs, Point p);
public:
	void restore(SegNode* sn, Mat edge, vector<BigSeg*>& result, SmallSeg*& r);
	void postRestore(vector<BigSeg*>& vbs);
	void reorganise(vector<BigSeg*> &vbs);
	void writeRestoreToFile(vector<BigSeg*>& vbs, int step, string depthPath, ofstream& outfile);
	void writeRestoreToFile2(vector<BigSeg*>& vbs, ofstream& outfile);
};


