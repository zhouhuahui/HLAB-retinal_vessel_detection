#include "pch.h"
#include"iterate_tree.h"
#include"vessel_depth.h"
#include"vessel_radius.h"
#include <cmath>
#include<set>



/*函数功能：判断Point p是否在joints或terminates中*/
bool inJoints(Point &p, vector<Point> &joints) {
	for (int i = 0; i < joints.size(); ++i) {
		if (p.x == joints.at(i).x && p.y == joints.at(i).y)
			return true;
	}
	return false;
}
bool inTerminates(Point &p, vector<Point> &terminates) {
	for (int i = 0; i < terminates.size(); ++i) {
		if (p.x == terminates.at(i).x && p.y == terminates.at(i).y)
			return true;
	}
	return false;
}
/*为测试createTree而写的函数，可视化输出树*/
/*que: 初始化为根结点
*/
void outputPixelTreeGraphedly(queue<pixelNode*> que)
{
	queue<pixelNode*> que2 = queue<pixelNode*>();
	int nodeCount = 0;
	while (!que.empty()) {
		pixelNode* node = que.front(); que.pop();
		if (node) {
			cout << "(" << node->pixel.y << "," << node->pixel.x << ")" << ";";
			pixelNode* temp = node->left;
			while (temp) {
				que2.push(temp);
				temp = temp->right;
				nodeCount++;
			}
			que2.push(temp);
		}
		else
			cout << "#------$";
	}
	cout << endl;
	if (nodeCount)
		outputPixelTreeGraphedly(que2);
}
//START createTree $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void createTree(Mat img, queue<pixelNode*>* que, vector<Point> &joints, vector<Point> &terminates, Point ancient,
	pixelNode* &node, int** visit)
{
	pixelNode* a = que->front(); que->pop(); //弹出一个待查找的pixelNode
	visit[a->pixel.y][a->pixel.x] = 1; img.at<uchar>(a->pixel.y, a->pixel.x) = 0;

	/*START 寻找a的所有满足条件的邻居点--------------------------------------------------START*/
	vector<Point> vec;
	bool flag = false;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i != 0 || j != 0) {
				int row = a->pixel.y + i; //行号,从0开始
				int col = a->pixel.x + j;
				Point p = Point(col, row);
				bool flag1 = inTerminates(p, terminates);
				bool flag2 = inJoints(p, joints);
				if ((flag1 || flag2) && (p != ancient)) {
					flag = true; 
					vec.push_back(p);
					img.at<uchar>(row, col) = 0;
					if (flag1)
						visit[row][col] = 1;
					break;
				}
			}
		}
		if (flag)
			break;
	}
	if (!flag) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (i != 0 || j != 0) {
					int row = a->pixel.y + i; //行号,从0开始
					int col = a->pixel.x + j;
					if (row >= 0 && row < img.rows && col >= 0 && col < img.cols) {
						uchar value = img.at<uchar>(row, col);
						if (value == 255 && !visit[row][col]) {
							Point p = Point(col, row);
							vec.push_back(p);
							visit[row][col] = 1;
							img.at<uchar>(row, col) = 0;
						}
					}
				}
			}
		}
	}
	/*END 寻找a的所有满足条件的邻居点-----------------------------------------------------------END*/
	node = (node == nullptr) ? a : node; //若node为空，则为根结点，否则，不变。这个语句不要放到了retun语句之后了，不然会发生错误
	if (vec.empty()) { //若找不到，则返回(递归出口)
		return;
	}


	/*START 更新node的所有子结点和que*/
	pixelNode* temp = node;
	int count = 0;
	for (Point p : vec) {
		pixelNode* child = new pixelNode(p);
		bool inJoint = inJoints(child->pixel, joints);
		bool inTerminate = inTerminates(child->pixel, terminates);
		if (!inJoint && !inTerminate)
			que->push(child);
		if (count == 0)
			temp->left = child;
		else {
			temp->right = child;
		}
		temp = child;
		++count;
	}
	/*END 更新node的所有子结点和que*/

	while (!que->empty()) {
		queue<pixelNode*>* que2 = new queue<pixelNode*>();
		pixelNode* b = que->front(); que->pop();
		que2->push(b);
		createTree(img, que2, joints, terminates, ancient, b, visit);
	}
}
//END createTree $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//START findRoutine $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


/*函数功能：根据一个树，找到所有从根结点到端点或交叉点（端点或交叉点都是叶结点）的路径，并记录所有的交叉点，以便调用函数使用
*/
void findRoutine(pixelNode* root, vector<Point> &joints, vector<Point> &terminates, int level, int** visit,
	vector<vector<Point>> &segments, vector<Point> &nextBeginQue, vector<Point> &stk, int &lastLevel)
{
	for (int i = 0; i < lastLevel - level + 1; ++i)
		stk.pop_back();
	stk.push_back(root->pixel);
	lastLevel = level;
	if (!root->left) { /*递归出口*/
		vector<Point> aRoad = vector<Point>();
		for (int i = 0; i < stk.size(); ++i)
			aRoad.push_back(stk.at(i));

		bool flag = inJoints(root->pixel, joints);
		if (aRoad.size() > 1) { //抛弃只有一个点的血管段
			if (inTerminates(root->pixel, terminates) || flag) {
				segments.push_back(aRoad);
			}
		}
	}
	else {
		pixelNode* node = root->left;
		while (node) {
			findRoutine(node, joints, terminates, level + 1, visit, segments, nextBeginQue, stk, lastLevel);
			node = node->right;
		}
	}
}
//END findRoutine $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//START findSeg $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void deletePixelTree(pixelNode* root) {
	if (root) {
		if (root->left)
			deletePixelTree(root->left);
		if (root->right)
			deletePixelTree(root->right);
		delete root;
	}
}
void findSeg(Mat img, vector<Point> &joints, vector<Point> &terminates, Point begin,
	vector<vector<Point>> &segments, vector<Point> &nextBeginQue, int** visit)
{
	pixelNode* root = nullptr;
	queue<pixelNode*>* que = new queue<pixelNode*>();
	pixelNode* beginNode = new pixelNode(begin);
	que->push(beginNode);
	createTree(img, que, joints, terminates, begin, root, visit);
	vector<Point> stk = vector<Point>();
	int lastLevel = -1;
	findRoutine(root, joints, terminates, 0, visit, segments, nextBeginQue, stk, lastLevel);

	delete que;
	deletePixelTree(root);
}
//END findSeg $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

void deleteSegTree(SegNode* root) {
	if (root) {
		if (root->left)
			deleteSegTree(root->left);
		if (root->right)
			deleteSegTree(root->right);
		delete root;
	}
}
//START createSegTree ##################################################################################################################
void createSegTree(Mat img, vector<Point> &joints, vector<Point> &terminates, Point begin,
	vector<SegNode*> &segForest)
{
	SegNode* segTree = nullptr; /*正在遍历的SegTree的根结点*/

	/*START 声明并初始化所有要用到的数据*/
	int name = 0;
	vector<Point> nextBeginQue = vector<Point>(); nextBeginQue.push_back(begin);
	vector<SegNode*> fatherQue = vector<SegNode*>(); 
	int** visit = new int*[img.rows];
	for (int i = 0; i < img.rows; ++i) {
		visit[i] = new int[img.cols];
		for (int j = 0; j < img.cols; ++j)
			visit[i][j] = 0;
	}
	/*END 声明并初始化所有要用到的数据*/

	while (true) {
		if (nextBeginQue.empty()) {
			/*terminates中还有未被访问的端点吗*/
			for (Point p : terminates) {
				if (visit[p.y][p.x] == 0) {
					nextBeginQue.push_back(p);
					break;
				}
			}
			if (nextBeginQue.empty()) 
				break;
			else
				continue;
		}
		Point segBegin = nextBeginQue.front(); nextBeginQue.erase(nextBeginQue.begin()); //pop掉队首元素


		vector<vector<Point>> segments = vector<vector<Point>>();
		findSeg(img, joints, terminates, segBegin, segments, nextBeginQue, visit);

		if (fatherQue.empty()) {
			int branch = segments.size(); 
			if (branch == 1) {
				SegNode* root = new SegNode(name++);
				root->pixels.assign(segments.back().begin(), segments.back().end()); //复制两个vector的值
				segTree = root;
				segForest.push_back(segTree);
				if (inJoints(root->pixels.back(), joints) && root->pixels.size() > 1 && visit[root->pixels.back().y][root->pixels.back().x] == 0) { 
					fatherQue.push_back(root);
					nextBeginQue.push_back(segments.at(0).back());
				}
			}
			if (branch > 1) { 
				/*解决连续多个交叉点检测不到的问题,并在这里更新fatherQue,nextBeginQue*/
				vector<SegNode*> fathers;
			A1:
				if (!fatherQue.empty()) {
					fathers.push_back(fatherQue.front());
					fatherQue.erase(fatherQue.begin());
				}
				do {
					//比较segments中的几个段，找出相同的部分
					int* s = new int[segments.size()]; 
					vector<int> lone;
					vector<SegNode*> intersecs; 
					for (int i = 0; i < segments.size(); ++i)s[i] = -1; //初始化
					for (int i = 0; i < segments.size() - 1; ++i) {
						if (s[i] == -1) {
							for (int j = i + 1; j < segments.size(); ++j) {
								if (segments.at(i).at(1).x == segments.at(j).at(1).x && segments.at(i).at(1).y == segments.at(j).at(1).y) {
									int b = i;
									while (s[b] != -1)
										b = s[b];
									s[b] = j;
								}
							}
							if (s[i] != -1) { 
								SegNode* f = new SegNode(name++);
								int count = segments.at(i).size();
								for (int j = 0; j < count; ++j) {
									int index = (j == 0) ? 0 : 1;
									int flag = true; //判断是否在这个并集的段在这个点都相等
									int c = s[i];
									while (c != -1) {
										if (segments.at(i).at(index) != segments.at(c).at(index)) {
											flag = false;
											break;
										}
										c = s[c];
									}
									if (flag) {
										f->pixels.push_back(segments.at(i).at(index));
										if (j != 0) {
											int b = i;
											while (b != -1) {
												segments.at(b).erase(segments.at(b).begin()); 
												b = s[b];
											}
										}
									}
									else
										break;
								}
								intersecs.push_back(f); 
							}
						}
					}
					//判断segments中哪些段是孤独段
					bool flagLone = true;
					for (int i = 0; i < segments.size(); ++i) {
						if (s[i] == -1) {
							bool flagLone = true; 
							for (int j = 0; j < i; ++j) {
								if (s[j] == i)
									flagLone = false;
							}
							if (flagLone)
								lone.push_back(i);
						}
					}

					//更新数据
					int cnt = fathers.size();
					if (cnt == 0) { 
						SegNode* root = new SegNode(name++);
						root = intersecs.at(0); //复制两个vector的值
						segTree = root;
						segForest.push_back(segTree);
						fatherQue.push_back(segTree);
						intersecs.erase(intersecs.begin());
						goto A1;
					}
					else {
						for (int i = 0; i < cnt; ++i) {
							SegNode* father = fathers.at(i);
							for (int j = 0; j < intersecs.size(); ++j) { 
								if (father->pixels.back() == intersecs.at(j)->pixels.front()) { //如果具有父子关系
									if (!father->left)
										father->left = intersecs.at(j);
									else {
										SegNode* f = father->left;
										while (f->right)
											f = f->right;
										f->right = intersecs.at(j);
									}
									fathers.push_back(intersecs.at(j));
								}
							}

							for (int j = 0; j < lone.size(); ++j) {
								if (father->pixels.back() == segments.at(lone.at(j)).front()) { //如果具有父子关系
									if (!father->left) {
										SegNode* child = new SegNode(name++);
										child->pixels.assign(segments.at(lone.at(j)).begin(), segments.at(lone.at(j)).end());
										father->left = child;
										//更新
										Point endP = child->pixels.back();
										if (inJoints(endP, joints) && child->pixels.size() > 1 && visit[endP.y][endP.x] == 0) {
											fatherQue.push_back(child);
											nextBeginQue.push_back(endP);
										}
									}
									else {
										SegNode* f = father->left;
										bool flag = false; 
										if (f->pixels.back() == segments.at(lone.at(j)).back())
											flag = true;
										while (f->right) {
											if (f->right->pixels.back() == segments.at(lone.at(j)).back())
												flag = true;
											f = f->right;
										}
										if (!flag) { 
											SegNode* child = new SegNode(name++);
											child->pixels.assign(segments.at(lone.at(j)).begin(), segments.at(lone.at(j)).end());
											f->right = child;
											//更新
											Point endP = child->pixels.back();
											if (inJoints(endP, joints) && child->pixels.size() > 1 && visit[endP.y][endP.x] == 0) {
												fatherQue.push_back(child);
												nextBeginQue.push_back(endP);
											}
										}
									}
								}
							}
						}
					}
					
					for (int i = 0; i < cnt; ++i) {
						fathers.erase(fathers.begin());
					}
					
					for (int i = lone.size() - 1; i >= 0; i--) { 
						segments.erase(segments.begin() + lone.at(i));
					}

					delete[] s;
				} while (!fathers.empty());
			}
		}
		else {
			if (true) {
				if (segments.size() >= 2) {
					vector<SegNode*> fathers;
					fathers.push_back(fatherQue.front()); fatherQue.erase(fatherQue.begin());
					do {
						int* s = new int[segments.size()]; 
						vector<int> lone; 
						vector<SegNode*> intersecs; 
						for (int i = 0; i < segments.size(); ++i)s[i] = -1; //初始化
						for (int i = 0; i < segments.size() - 1; ++i) {
							if (s[i] == -1) {
								for (int j = i + 1; j < segments.size(); ++j) {
									if (segments.at(i).at(1).x == segments.at(j).at(1).x && segments.at(i).at(1).y == segments.at(j).at(1).y) {
										int b = i;
										while (s[b] != -1)
											b = s[b];
										s[b] = j;
									}
								}
								if (s[i] != -1) {
									SegNode* f = new SegNode(name++);
									int count = segments.at(i).size();
									for (int j = 0; j < count; ++j) {
										int index = (j == 0) ? 0 : 1;
										int flag = true; 
										int c = s[i];
										while (c != -1) {
											if (segments.at(i).at(index) != segments.at(c).at(index)) {
												flag = false;
												break;
											}
											c = s[c];
										}
										if (flag) {
											f->pixels.push_back(segments.at(i).at(index));
											if (j != 0) {
												int b = i;
												while (b != -1) {
													segments.at(b).erase(segments.at(b).begin()); 
													b = s[b];
												}
											}
										}
										else
											break;
									}
									intersecs.push_back(f); 
								}
							}
						}
						//判断segments中哪些段是孤独段
						bool flagLone = true;
						for (int i = 0; i < segments.size(); ++i) {
							if (s[i] == -1) {
								bool flagLone = true; 
								for (int j = 0; j < i; ++j) {
									if (s[j] == i)
										flagLone = false;
								}
								if (flagLone)
									lone.push_back(i);
							}
						}

						//更新
						int cnt = fathers.size();
						for (int i = 0; i < cnt; ++i) {
							SegNode* father = fathers.at(i);
							for (int j = 0; j < intersecs.size(); ++j) { 
								if (father->pixels.back() == intersecs.at(j)->pixels.front()) { 
									if (!father->left)
										father->left = intersecs.at(j);
									else {
										SegNode* f = father->left;
										while (f->right)
											f = f->right;
										f->right = intersecs.at(j);
									}
									fathers.push_back(intersecs.at(j));
								}
							}

							for (int j = 0; j < lone.size(); ++j) {
								if (father->pixels.back() == segments.at(lone.at(j)).front()) { //如果具有父子关系
									if (!father->left) {
										SegNode* child = new SegNode(name++);
										child->pixels.assign(segments.at(lone.at(j)).begin(), segments.at(lone.at(j)).end());
										father->left = child;
										//更新
										Point endP = child->pixels.back();
										if (inJoints(endP, joints) && child->pixels.size() > 1 && visit[endP.y][endP.x] == 0) {
											fatherQue.push_back(child);
											nextBeginQue.push_back(endP);
										}
									}
									else {
										SegNode* f = father->left;
										bool flag = false; 
										if (f->pixels.back() == segments.at(lone.at(j)).back())
											flag = true;
										while (f->right) {
											if (f->right->pixels.back() == segments.at(lone.at(j)).back())
												flag = true;
											f = f->right;
										}
										if (!flag) { 
											SegNode* child = new SegNode(name++);
											child->pixels.assign(segments.at(lone.at(j)).begin(), segments.at(lone.at(j)).end());
											f->right = child;
											//更新
											Point endP = child->pixels.back();
											if (inJoints(endP, joints) && child->pixels.size() > 1 && visit[endP.y][endP.x] == 0) {
												fatherQue.push_back(child);
												nextBeginQue.push_back(endP);
											}
										}
									}
								}
							}
						}

						for (int i = 0; i < cnt; ++i) {
							fathers.erase(fathers.begin());
						}
						
						for (int i = lone.size() - 1; i >= 0; i--) { 
							segments.erase(segments.begin() + lone.at(i));
						}

						delete[] s;
					} while (!fathers.empty());
				}
				else {
					/*为father连接子结点*/
					SegNode* father = fatherQue.front(); fatherQue.erase(fatherQue.begin());
					for (int i = 0; i < segments.size(); ++i) {
						SegNode* child = new SegNode(name);
						child->pixels.assign(segments.at(i).begin(), segments.at(i).end());
						if (i == 0)
							father->left = child;
						else
							father->right = child;
						father = child;

						Point endP = child->pixels.back();
						if (inJoints(endP, joints) && child->pixels.size() > 1 && visit[child->pixels.back().y][child->pixels.back().x] == 0) {
							fatherQue.push_back(child);
							nextBeginQue.push_back(endP);
						}
						name++;
					}
				}
			}
		}
	}

	for (int i = 0; i < img.rows; ++i) {
		delete[] visit[i];
	}
	delete[] visit;
}
//END createSegTree $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

/*START writeToFiLe $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
//downsample策略是：未舍弃离末尾点距离小于step的点。
void writeToFile(SegNode* father, SegNode* ego, Mat edge, string depthPath, ofstream& outfile)
{
	int step = 1; 

	outfile << "g " << ego->segName << "\n";
	if (father)
		outfile << "p 1 " << father->segName << "\n";
	else
		outfile << "p 0\n";

	int childCount = 0; 
	vector<int> childs = vector<int>(); 
	vector<SegNode*> childNodes = vector<SegNode*>(); 
	SegNode* child = ego->left;
	while (child) {
		childCount++;
		childs.push_back(child->segName);
		childNodes.push_back(child);
		child = child->right;
	}
	outfile << "c " << childCount;
	for (int i = 0; i < childCount; ++i) {
		outfile << " " << childs.at(i);
	}
	outfile << "\n";

	int n = ego->pixels.size();
	outfile << "n " << (2+(n-2)/step) << "\n";

	Mat depthImg = imread(depthPath);
	int i;
	double sum = 0;
	int count = 0;
	double r = 0;
	for (i = 0; i < n; i = i + step) {
		Point p = ego->pixels.at(i);
		sum += radius(edge, p.x, p.y);
		++count;
	}
	r = sum / count;

	for (i = 0; i < n; i = i+step) {
		Point p = ego->pixels.at(i);

		if (depthPath == "") {
			outfile << "v " << p.x << " " << p.y << " " << 0 << " " << r << "\n";
		}
		else {
			int deep = depth(depthImg, p.x, p.y);
			outfile << "v " << p.x << " " << p.y << " " << deep << " " << r << "\n"; 
		}
	}
	if (i < n - 1 + step) {
		Point p = ego->pixels.back();
		float r = radius(edge, p.x, p.y);
		//r = 0.1;

		if (depthPath == "") {
			outfile << "v " << p.x << " " << p.y << " " << 0 << " " << r << "\n";
		}
		else {
			int deep = depth(depthImg, p.x, p.y);
			outfile << "v " << p.x << " " << p.y << " " << deep << " " << r << "\n";
		}
	}
	outfile << "\n";

	for (i = 0; i < childNodes.size(); ++i) {
		writeToFile(ego, childNodes.at(i), edge, depthPath, outfile);
	}
}
/*END writeToFile $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


/*START 输出到.paths文件中（适合SimVascular的输入文件）$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/*将所有段的control point输出到.paths文件中*/
void writeControlPointToPaths(SegNode* ego, string depthPath, ofstream& outfile) {
	//找到ego的所有子结点
	vector<SegNode*> childNodes = vector<SegNode*>(); /*保存所有孩子结点*/
	SegNode* child = ego->left;
	while (child) {
		childNodes.push_back(child);
		child = child->right;
	}

	Point begin = ego->pixels.front();
	Point end = ego->pixels.back();
	Mat depthImg = imread(depthPath);

	outfile << "set gPathPoints(" << ego->segName << ",name) {" << ego->segName << "}\n";
	outfile << "set gPathPoints(" << ego->segName << ",0)" << " { " << begin.x << " " << begin.y << " ";
	if (depthPath != "") {
		int deep = depth(depthImg, begin.x, begin.y);
		outfile << deep << "}\n";
	}
	else {
		outfile << "0}\n";
	}

	outfile << "set gPathPoints(" << ego->segName << ",1)" << " { " << end.x << " " << end.y << " ";
	if (depthPath != "") {
		int deep = depth(depthImg, end.x, end.y);
		outfile << deep << "}\n";
	}
	else {
		outfile << "0}\n";
	}

	for (int i = 0; i < childNodes.size(); ++i) {
		writeControlPointToPaths(childNodes.at(i), depthPath, outfile);
	}
}


void writeAllPointsToPaths(SegNode* ego, string depthPath, ofstream& outfile) {
	//找到ego的所有子结点
	vector<SegNode*> childNodes = vector<SegNode*>(); /*保存所有孩子结点*/
	SegNode* child = ego->left;
	while (child) {
		childNodes.push_back(child);
		child = child->right;
	}

	int segName = ego->segName;
	int nodeCount = ego->pixels.size();
	int step = 1;
	outfile << "set gPathPoints(" << segName << ",numSplinePts) {" << (((nodeCount - 1) % step != 0) ? (2 + (nodeCount - 1) / step) : (1 + (nodeCount - 1) / step)) << "}\n";
	outfile << "set gPathPoints(" << segName << ",splinePts) [list \\\n";
	if (depthPath != "") {
		Mat depthImg = imread(depthPath);
		Point3f preT;
		Point3f preR;
		for (int i = 0; i < nodeCount; i = i + step) {
			Point p = ego->pixels.at(i);
			int deep = depth(depthImg, p.x, p.y);
			outfile << "	{p (" << p.x << "," << p.y << "," << deep << ") ";
			outfile << "t(0,0,0) tx(0,0,0)} \\\n";

			////计算切线向量（3维）单位化后的向量
			//if (i + step >= nodeCount && i != nodeCount - 1) {
			//	Point postP = ego->pixels.at(nodeCount - 1);
			//	float postD = depth(depthImg, postP.x, postP.y);
			//	float a = sqrt(pow(postP.x - p.x, 2) + pow(postP.y - p.y, 2) + pow(postD - deep, 2));
			//	preT.x = (postP.x - p.x) / float(a);
			//	preT.y = (postP.y - p.y) / float(a);
			//	preT.z = (postD - deep) / float(a);
			//	preR.x = 0;
			//	preR.y = sqrt(pow(preT.z, 2) + pow(preT.x, 2)*0.67);
			//	preR.z = sqrt(pow(preT.y, 2) + pow(preT.x, 2)*0.33);
			//	outfile << "t (" << preT.x << "," << preT.y << "," << preT.z << ") ";
			//	outfile << "tx (" << preR.x << "," << preR.y << "," << preR.z << ")} \\\n";

			//	//输出最后一个path point
			//	p = ego->pixels.at(nodeCount - 1);
			//	deep = depth(depthImg, p.x, p.y);
			//	outfile << "	{p (" << p.x << "," << p.y << "," << deep << ") ";
			//	outfile << "t (" << preT.x << "," << preT.y << "," << preT.z << ") ";
			//	outfile << "tx (" << preR.x << "," << preR.y << "," << preR.z << ")} \\\n";
			//}
			//else if (i == nodeCount - 1) {
			//	outfile << "t (" << preT.x << "," << preT.y << "," << preT.z << ") ";
			//	outfile << "tx (" << preR.x << "," << preR.y << "," << preR.z << ")} \\\n";
			//}
			//else {
			//	Point postP = ego->pixels.at(i + step);
			//	float postD = depth(depthImg, postP.x, postP.y);
			//	float a = sqrt(pow(postP.x - p.x, 2) + pow(postP.y - p.y, 2) + pow(postD - deep, 2));
			//	preT.x = (postP.x - p.x) / float(a);
			//	preT.y = (postP.y - p.y) / float(a);
			//	preT.z = (postD - deep) / float(a);
			//	preR.x = 0;
			//	preR.y = sqrt(pow(preT.z, 2) + pow(preT.x, 2)*0.67);
			//	preR.z = sqrt(pow(preT.y, 2) + pow(preT.x, 2)*0.33);

			//	outfile << "t (" << preT.x << "," << preT.y << "," << preT.z << ") ";
			//	outfile << "tx (" << preR.x << "," << preR.y << "," << preR.z << ")} \\\n";
			//}
		}
	}
	else {
		for (int i = 0; i < nodeCount; ++i) {
			Point p = ego->pixels.at(i);
			outfile << "	{p (" << p.x << " " << p.y << " " << 0 << ") ";
			outfile << "t(0,0,0) tx(0,0,0)} \\\n";
		}
	}
	outfile << "	]\n";

	for (int i = 0; i < childNodes.size(); ++i) {
		writeAllPointsToPaths(childNodes.at(i), depthPath, outfile);
	}
}

void writeToPaths(SegNode* root, string depthPath, ofstream& outfile) {
	outfile << "#  Geodesic path file version 2.0 (from guiPP)\n";
	outfile << "global gPathPoints\n";

	if (root) {
		writeControlPointToPaths(root, depthPath, outfile);
		writeAllPointsToPaths(root, depthPath, outfile);
	}
}

void writeRadiusToFile(SegNode* root, Mat edge, ofstream& outfile) {
	vector<SegNode*> childNodes = vector<SegNode*>(); 
	SegNode* child = root->left;
	while (child) {
		childNodes.push_back(child);
		child = child->right;
	}

	outfile << "segName " << root->segName << "\n";
	int nodeCount = root->pixels.size();
	for (int i = 0; i < nodeCount; i = i + 5) {
		Point p = root->pixels.at(i);
		if (i + 5 >= nodeCount && i != nodeCount - 1) {
			outfile << radius(edge, p.x, p.y) << "\n";

			p = root->pixels.at(nodeCount - 1);
			outfile << radius(edge, p.x, p.y) << "\n";
		}
		else {
			outfile << radius(edge, p.x, p.y) << "\n";
		}
	}

	for (int i = 0; i < childNodes.size(); ++i) {
		writeRadiusToFile(childNodes.at(i), edge, outfile);
	}
}

/*END 输出到.paths文件中（适合SimVascular的输入文件）$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/*为了符合SimVascular的输入要求，将树状结构处理成鱼骨状结构，或主干-分支结构 *****************************************/
void findMasterBranch(SegNode* root, string depthPath, Mat edge, vector<vector<Point4f>>& R) {
	int step = 5; 

	vector<SegNode*> vec;
	vector<Point4f> a;
	SegNode* t = root;
	int count;
	double deep, r;

	Mat depthImg;
	if (depthPath != "")
		depthImg = imread(depthPath);
L:
	count = 0;
	while (t) {
		vec.push_back(t);
		//更新a
		int i = (count == 0) ? 0 : 1;
		for (i; i < t->pixels.size(); i=i+step) {
			Point p = t->pixels.at(i);
			deep = (depthPath == "") ? 0 : depth(depthImg, p.x, p.y);
			r = radius(edge, p.x, p.y);

			Point4f p4;
			p4.x = p.x;
			p4.y = p.y;
			p4.z = deep;
			p4.r = r;
			a.push_back(p4);
		}
		if (i < t->pixels.size() - 1 + step) {
			Point p = t->pixels.back();
			deep = (depthPath == "") ? 0 : depth(depthImg, p.x, p.y);
			r = radius(edge, p.x, p.y);

			Point4f p4;
			p4.x = p.x;
			p4.y = p.y;
			p4.z = deep;
			p4.r = r;
			a.push_back(p4);
		}

		t = t->left;
		count++;
	}
	if (!a.empty())
		R.push_back(a);
	if (vec.empty())
		return;
	t = vec.back();
	vec.pop_back();
	t = t->right;
	a.clear();
	goto L;
}
void writeMasterBranchToFile(vector<vector<Point4f>>& R, ofstream& outfile) {
	for (int i = 0; i < R.size(); ++i) {
		for (int j = 0; j < R.at(i).size(); ++j) {
			Point4f p = R.at(i).at(j);
			outfile << p.x << " " << p.y << " " << p.z << " " << p.r << " ";
		}
		outfile << "\n";
	}
}
/* ************************************************************************************************************/

void outputPixelTree(pixelNode* root) {
	cout << "(" << root->pixel.y << "," << root->pixel.x << ")" << ",";
	if (root->left)
		outputPixelTree(root->left);
	else
		cout << "?,";
	if (root->right)
		outputPixelTree(root->right);
	else
		cout << "?,";
}

void outputSegTree(SegNode* root) {
	cout << root->segName << endl;
	if (root->left)
		outputSegTree(root->left);
	if (root->right)
		outputSegTree(root->right);
}
