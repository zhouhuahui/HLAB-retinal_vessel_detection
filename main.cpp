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
Ϊ�˷��㣬�����without_visit.bmp��.txtĬ��Ϊ
pub_without_visit.bnp��pubVessel.txt

������ģʽ��3����������һ����exe���ڶ������۵�ͼ·���������������ͼ·������û�м����κ�³���Լ�����룬Ĭ����������ǶԵ�
*/

void computeNodesOnTree(SegNode* root, int &nodeNumOnTree) {
	if (root) {
		++nodeNumOnTree;
		computeNodesOnTree(root->left, nodeNumOnTree);
		computeNodesOnTree(root->right, nodeNumOnTree);
	}
}

/*������Ҫ������t�еĵ������ֵ��Ϊn
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
�㷨���ܣ���һ���ַ�����һ���ַ����ָ�ɶ���ַ���������vector��
*/
vector<string> splitStr(const string &s, const string &seperator)
{

	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//�ҵ��ַ������׸������ڷָ�������ĸ��
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

		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
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
	///*��ȡ�����в���,û�м����κ�³���Լ�����룬Ĭ������ϸ�*/
	//string originImgPath; //����ʵ����ͼ��ȡ��Ѫ��ͼ��·��
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
		string depthPath = ""; //��Ϊ���ַ�������ʾ�������ͼ
		int writeRestoreStep = 20; //������1��ʾ��downsample
		int solution = 0;
		string baseDir = "image\\CHASEDB1\\";

		Mat oriImg = imread(originImgPath);
		/*cout << oriImg.size() << endl;
		cout << oriImg << endl;*/

		/*Ϊ�˼���뾶������ͼƬ����*/
		Mat srcImage1 = oriImg.clone(); //clone�����
		Mat dstImage, edge, grayImage;
		// ��1��������srcͬ���ͺʹ�С�ľ���(dst)
		dstImage.create(srcImage1.size(), srcImage1.type());
		// ��2����ԭͼ��ת��Ϊ�Ҷ�ͼ��
		cvtColor(srcImage1, grayImage, COLOR_BGR2GRAY); 
		//grayImage = srcImage1.clone();
		cout << grayImage.size() << endl;
		threshold(grayImage, grayImage, 50, 255, CV_THRESH_BINARY); /*תΪ��ֵͼ*/
		// ��3������ʹ�� 3x3�ں�������
		blur(grayImage, edge, Size(3, 3));
		// ��4������Canny����
		int MaxThreshold;
		MaxThreshold = MaxEntropy(edge);
		Canny(edge, edge, MaxThreshold / 3, MaxThreshold);
		imwrite(baseDir + imgName + "Canny.bmp", edge);

		/*START Ϊ�˱����Ǽ�ͼ�õ�����Ѫ�ܶζ�����ͼƬ����---------------------------------------------------*/
		Mat img = oriImg.clone();
		cvtColor(oriImg, img, CV_RGB2GRAY); /*תΪ�Ҷ�ͼ*/
		threshold(img, img, 50, 255, CV_THRESH_BINARY); /*תΪ��ֵͼ*/
		if (solution == 0) {
			thinning(img, img);
			imwrite(baseDir + imgName + "Thinned.bmp", img);
			vector<Point> joints = getcrossPoints(img);
			vector<Point> terminates = getendPoints(img);

			//�������ڵĽ����Ͷ˵㣬����������㷨�к�����������������Ͳ�Ӧ�ó���
			int len = joints.size();
			/*for (int i = 0; i < len; ++i) {
				if (joints.at(i) == Point(237, 372)) {
					joints.erase(joints.begin() + i);
					break;
				}
			}*/
			vector<Point> delQue = vector<Point>(); //��ɾ���Ľ���������
			for (int i = 0; i < len; ++i) {
				Point point = joints.at(i);
				bool flag = false; //�����ʵĵ��Ƿ���delQue��
				for (Point p : delQue) {
					if (p == point) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					//������8�����Ƿ�����joints��terminates�У�����У��ͷŽ���ɾ��������
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
				bool flag = false; //�����ʵĵ��Ƿ���delQue��
				for (Point p : delQue) {
					if (p == point) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					//������8�����Ƿ�����terminates�У�����У��ͷŽ���ɾ��������
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

			//�������Ͷ˵�������ͼ����
			Mat imgWithObvMark1 = img.clone();
			Mat imgWithObvMark2 = img.clone();

			/*START ����createSegTree����------------------------------------------------------------START*/
			Point begin = terminates.front();
			vector<SegNode*> segForest = vector<SegNode*>();

			createSegTree(img, joints, terminates, begin, segForest);
			//outputSegTree(segForest.at(0));
			cout << "����" << segForest.size() << "����" << endl;
			//imwrite("C:\\Users\\�ܻ���\\source\\repos\\iterate_tree\\iterate_tree\\pub_without_visit.bmp", img);
			//imwrite("D:\\pub_without_visit.bmp", img);

			/*START д�뵽�ļ���--------------------------------------------*/
			//�ҵ�������
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
			/*END д�뵽�ļ���---------------------------------------------------------------------*/

			//��ϸ��ͼ�ж���ĵ��Ϊ��ɫ
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
			//�������Ͷ˵�������ͼ����
			cvtColor(imgWithObvMark2, imgWithObvMark2, CV_GRAY2RGB); /*תΪα��ɫͼ*/
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

			/*���ŵ㲢д�뵽�ļ���-----------------------------------------------------------------------------------*/
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
			/*���ŵ㲢д�뵽�ļ���-----------------------------------------------------------------------------------*/

			/*delete*/
			for (int i = 0; i < segForest.size(); ++i) {
				deleteSegTree(segForest.at(i));
			}
			/*deleteSmallSegTree(r);
			for (int i = 0; i < vbs.size(); ++i) {
				delete vbs.at(i);
			}*/
			/*END ����createSegTree����---------------------------------------------------------------END*/
		}
	}
	return 0;
}


