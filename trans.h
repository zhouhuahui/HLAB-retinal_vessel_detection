#pragma once
#include"iterate_tree.h"
#include<vector>
#include<map>

typedef struct BigSeg BigSeg;
/*
小段，相邻两个交叉点之间的段
*/
typedef struct SmallSeg {
	SegNode* s;
	BigSeg* bs; /*这个小段处于哪个大段*/
	int direction; /*取值为{0，1}，0表示倒转SegNode中点的顺序*/
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
很多连续的小段组成的大段，在血管图中大致对应一条完整的血管
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
功能：为了转换SegTree，使它符合SimVascular的输入且不出错
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


