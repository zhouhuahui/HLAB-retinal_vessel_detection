#pragma once
#include "pch.h"
#include <iostream>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>
#include<queue>
#include<stack>
#include<string>

using namespace cv;
using namespace std;

typedef struct pixelNode {
	Point pixel; 
	struct pixelNode* left; 
	struct pixelNode* right;

	pixelNode(Point p) {
		pixel = p;
		left = right = nullptr;
	}
}pixelNode;

typedef struct SegNode {
	int segName; 
	vector<Point> pixels;
	struct SegNode* left; 
	struct SegNode* right;
	SegNode(int name) {
		segName = name;
		pixels = vector<Point>();
		left = right = nullptr;
	}
}SegNode;
typedef struct Point4f {
	double x;
	double y;
	double z;
	double r;
	Point4f(double a, double b, double c, double d) {
		x = a;
		y = b;
		z = c;
		r = d;
	}
	Point4f() {};
}Point4f;

bool inJoints(Point &p, vector<Point> &joints);
bool inTerminates(Point &p, vector<Point> &terminates);
void outputPixelTreeGraphedly(queue<pixelNode*> que);
void createTree(Mat img, queue<pixelNode*>* que, vector<Point> &joints, vector<Point> &terminates, Point ancient,
	pixelNode* &node, int** visit);
void findRoutine(pixelNode* root, vector<Point> &joints, vector<Point> &terminates, int level,int** visit,
	vector<vector<Point>> &segments, vector<Point> &nextBeginQue, vector<Point> &stk, int &lastLevel);

void deletePixelTree(pixelNode* root);
void findSeg(Mat img, vector<Point> &joints, vector<Point> &terminates, Point begin,
	vector<vector<Point>> &segments, vector<Point> &nextBeginQue, int** visit);

void deleteSegTree(SegNode* root);
void createSegTree(Mat img, vector<Point> &joints, vector<Point> &terminates, Point begin,
	vector<SegNode*> &segForest);

void outputPixelTree(pixelNode* root);
void outputSegTree(SegNode* root);

void writeToFile(SegNode* father, SegNode* ego, Mat edge, string depthPath, ofstream& outfile);

void writeControlPointToPaths(SegNode* ego, string depthPath, ofstream& outfile);
void writeAllPointsToPaths(SegNode* ego, string depthPath, ofstream& outfile);
void writeToPaths(SegNode* root, string depthPath, ofstream& outfile);
void writeRadiusToFile(SegNode* root, Mat edge, ofstream& outfile);

void findMasterBranch(SegNode* root, string depthPath, Mat edge, vector<vector<Point4f>>& R);
void writeMasterBranchToFile(vector<vector<Point4f>>& R, ofstream& outfile);