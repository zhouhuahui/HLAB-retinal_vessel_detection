#include "pch.h"
#include"iterate_tree.h"
#include"vessel_depth.h"
#include"vessel_radius.h"
#include"thinning.h"
#include"crossPoint.h"
#include"endPoint.h"
#include"trans.h"
#include<vector>

using namespace cv;
using namespace std;

/*
为了方便，输出的without_visit.bmp和.txt默认为
pub_without_visit.bnp和pubVessel.txt

命令行模式：3个参数，第一个是exe，第二个是眼底图路径，第三个是深度图路径；并没有加入任何鲁棒性检验代码，默认输入参数是对的
*/

void computeNodesOnTree(SegNode* root, int &nodeNumOnTree) {
	if (root) {
		++nodeNumOnTree;
		computeNodesOnTree(root->left, nodeNumOnTree);
		computeNodesOnTree(root->right, nodeNumOnTree);
	}
}

/*由于需要，把在t中的点的像素值设为n
*/
void setTo(SegNode* t,Mat image,int n) {
	stack<SegNode*> stk = stack<SegNode*>();
LOOP:
	while (t != nullptr) {
		for (int j = 0; j < t->pixels.size(); ++j) {
			image.at<uchar>(t->pixels.at(j).y, t->pixels.at(j).x) = n;
		}
		stk.push(t);
		t = t->left;
	}
	if (stk.empty())
		return;
	else {
		t = stk.top(); stk.pop();
		t = t->right;
		goto LOOP;
	}
}

/*
算法功能：按一种字符，将一个字符串分割成多个字符串，放在vector中
*/
vector<string> splitStr(const string &s, const string &seperator)
{

	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}

	return result;
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

void outputSmallSegBinTree(SmallSeg* ss) {
	if (ss) {
		cout << "--------------" << endl;
		cout << ss->s->segName << ": ";
		vector<Point>::iterator it;
		int cnt = 0;
		for (it = ss->s->pixels.begin(); it != ss->s->pixels.end(); it++) {
			cout << (*it) << ss->rs.at(cnt) << " ";
			cnt++;
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

//int main(int argc, char* argv[]) {
int main(){
	///*析取命令行参数,没有加入任何鲁棒性检验代码，默认输入合格*/
	//string originImgPath; //由真实眼球图提取的血管图的路径
	//string depthPath = "";
	//if (argc == 2) {
	//	originImgPath = string(*(++argv));
	//}
	//else if (argc == 3) {
	//	originImgPath = string(*(++argv));
	//	depthPath = string(*(++argv));
	//}
	//else {
	//	return 0;
	//}
	//vector<string> results = splitStr(originImgPath, string("."));
	//string imgName = results.at(0);

	vector<string> originImgPaths, imgNames;
	originImgPaths.push_back("image\\CHASEDB1\\Image_01L_1stHO.png"); imgNames.push_back("Image_01L_1stHO");

	for (int img_i = 0; img_i < imgNames.size(); img_i++) {
		string imgName = imgNames[img_i];
		string originImgPath = originImgPaths[img_i];
		string depthPath = ""; //设为空字符串，表示不用深度图
		int writeRestoreStep = 20; //步长，1表示不downsample
		int solution = 0;
		string baseDir = "image\\CHASEDB1\\";

		Mat oriImg = imread(originImgPath);
		/*cout << oriImg.size() << endl;
		cout << oriImg << endl;*/

		/*为了计算半径而做的图片处理*/
		Mat srcImage1 = oriImg.clone(); //clone是深拷贝
		Mat dstImage, edge, grayImage;
		// 【1】创建与src同类型和大小的矩阵(dst)
		dstImage.create(srcImage1.size(), srcImage1.type());
		// 【2】将原图像转换为灰度图像
		cvtColor(srcImage1, grayImage, COLOR_BGR2GRAY); 
		//grayImage = srcImage1.clone();
		cout << grayImage.size() << endl;
		threshold(grayImage, grayImage, 50, 255, CV_THRESH_BINARY); /*转为二值图*/
		// 【3】先用使用 3x3内核来降噪
		blur(grayImage, edge, Size(3, 3));
		// 【4】运行Canny算子
		int MaxThreshold;
		MaxThreshold = MaxEntropy(edge);
		Canny(edge, edge, MaxThreshold / 3, MaxThreshold);
		imwrite(baseDir + imgName + "Canny.bmp", edge);

		/*START 为了遍历骨架图得到所有血管段而做的图片处理---------------------------------------------------*/
		Mat img = oriImg.clone();
		cvtColor(oriImg, img, CV_RGB2GRAY); /*转为灰度图*/
		threshold(img, img, 50, 255, CV_THRESH_BINARY); /*转为二值图*/
		if (solution == 0) {
			thinning(img, img);
			imwrite(baseDir + imgName + "Thinned.bmp", img);
			vector<Point> joints = getcrossPoints(img);
			vector<Point> terminates = getendPoints(img);

			//消除相邻的交叉点和端点，这种情况对算法有害，而且这种情况本就不应该出现
			int len = joints.size();
			/*for (int i = 0; i < len; ++i) {
				if (joints.at(i) == Point(237, 372)) {
					joints.erase(joints.begin() + i);
					break;
				}
			}*/
			vector<Point> delQue = vector<Point>(); //待删除的交叉点的序列
			for (int i = 0; i < len; ++i) {
				Point point = joints.at(i);
				bool flag = false; //正访问的点是否在delQue中
				for (Point p : delQue) {
					if (p == point) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					//看附近8个点是否有在joints或terminates中，如果有，就放进待删除队列中
					for (int j = -1; j <= 1; ++j) {
						for (int k = -1; k <= 1; ++k) {
							if (!(j == 0 && k == 0)) {
								int row = point.y + j;
								int col = point.x + k;
								Point temp = Point(col, row);
								if (inJoints(temp, joints) || inTerminates(temp, terminates))
									delQue.push_back(temp);
							}
						}
					}
				}
			}
			for (Point p : delQue) {
				for (int i = 0; i < joints.size(); ++i) {
					if (joints.at(i) == p) {
						joints.erase(joints.begin() + i);
						break;
					}
				}
			}

			len = terminates.size();
			delQue.clear();
			for (int i = 0; i < len; ++i) {
				Point point = terminates.at(i);
				bool flag = false; //正访问的点是否在delQue中
				for (Point p : delQue) {
					if (p == point) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					//看附近8个点是否有在terminates中，如果有，就放进待删除队列中
					for (int j = -1; j <= 1; ++j) {
						for (int k = -1; k <= 1; ++k) {
							if (!(j == 0 && k == 0)) {
								int row = point.y + j;
								int col = point.x + k;
								Point temp = Point(col, row);
								if (inTerminates(temp, terminates))
									delQue.push_back(temp);
							}
						}
					}
				}
			}
			for (Point p : delQue) {
				for (int i = 0; i < terminates.size(); ++i) {
					if (terminates.at(i) == p) {
						terminates.erase(terminates.begin() + i);
						break;
					}
				}
			}

			//将交叉点和端点体现在图像上
			Mat imgWithObvMark1 = img.clone();
			Mat imgWithObvMark2 = img.clone();

			/*START 调试createSegTree函数------------------------------------------------------------START*/
			Point begin = terminates.front();
			vector<SegNode*> segForest = vector<SegNode*>();

			createSegTree(img, joints, terminates, begin, segForest);
			//outputSegTree(segForest.at(0));
			cout << "共有" << segForest.size() << "个树" << endl;
			//imwrite("C:\\Users\\周华辉\\source\\repos\\iterate_tree\\iterate_tree\\pub_without_visit.bmp", img);
			//imwrite("D:\\pub_without_visit.bmp", img);

			/*START 写入到文件中--------------------------------------------*/
			//找到最大的树
			int nodeNumOnTree = 0;
			int maxNodeNumOnTree = 0;
			int indexOfMax = 0;
			for (int i = 0; i < segForest.size(); ++i) {
				computeNodesOnTree(segForest.at(i), nodeNumOnTree);
				if (nodeNumOnTree > maxNodeNumOnTree) {
					maxNodeNumOnTree = nodeNumOnTree;
					indexOfMax = i;
				}
				nodeNumOnTree = 0;
			}
			/*END 写入到文件中---------------------------------------------------------------------*/

			//把细化图中多余的点变为黑色
			setTo(segForest.at(indexOfMax), imgWithObvMark2, 200);
			for (int i = 0; i < imgWithObvMark2.rows; ++i) {
				for (int j = 0; j < imgWithObvMark2.cols; ++j) {
					if (imgWithObvMark2.at<uchar>(i, j) != 200)
						imgWithObvMark2.at<uchar>(i, j) = 0;
				}
			}
			vector<cv::Point>::iterator itJoints2 = joints.begin();
			while (itJoints2 != joints.end()) {
				if (imgWithObvMark2.at<uchar>((*itJoints2).y, (*itJoints2).x) != 200)
					itJoints2 = joints.erase(itJoints2);

				else
					itJoints2++;
			}
			vector<cv::Point>::iterator itTerminates2 = terminates.begin();
			while (itTerminates2 != terminates.end()) {
				if (imgWithObvMark2.at<uchar>((*itTerminates2).y, (*itTerminates2).x) != 200)
					itTerminates2 = terminates.erase(itTerminates2);

				else
					itTerminates2++;
			}


			setTo(segForest.at(indexOfMax), imgWithObvMark2, 255);
			//将交叉点和端点体现在图像上
			cvtColor(imgWithObvMark2, imgWithObvMark2, CV_GRAY2RGB); /*转为伪彩色图*/
			imwrite(baseDir + imgName + "Thinned2.bmp", imgWithObvMark2);
			itJoints2 = joints.begin();
			for (; itJoints2 != joints.end(); itJoints2++)
			{
				circle(imgWithObvMark2, *itJoints2, 1.0, cv::Scalar(255, 0, 0), -1);
			}
			itTerminates2 = terminates.begin();
			for (; itTerminates2 != terminates.end(); itTerminates2++)
			{
				circle(imgWithObvMark2, *itTerminates2, 1.0, cv::Scalar(0, 0, 255), -1);
			}
			imwrite(baseDir + imgName + "WithObvMark2.bmp", imgWithObvMark2);

			/*重排点并写入到文件中-----------------------------------------------------------------------------------*/
			vector<BigSeg*> vbs;
			SmallSeg* r = nullptr;
			Trans trans = Trans();
			trans.restore(segForest.at(indexOfMax), edge, vbs, r);
			trans.postRestore(vbs);

			ofstream outfile;
			outfile.open(string("image\\235\\").append(imgName).append("Restore.txt"));
			outfile << "";
			outfile.close();
			outfile.open(string("image\\235\\").append(imgName).append("Restore.txt"), ios::app);
			trans.writeRestoreToFile(vbs, writeRestoreStep, depthPath, outfile);
			/*重排点并写入到文件中-----------------------------------------------------------------------------------*/

			/*delete*/
			for (int i = 0; i < segForest.size(); ++i) {
				deleteSegTree(segForest.at(i));
			}
			/*deleteSmallSegTree(r);
			for (int i = 0; i < vbs.size(); ++i) {
				delete vbs.at(i);
			}*/
			/*END 调试createSegTree函数---------------------------------------------------------------END*/
		}
	}
	return 0;
}


