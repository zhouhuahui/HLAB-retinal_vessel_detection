/*�����صĵõ��˵�Ĵ���*/

#include"pch.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>  
#include <vector>  
using namespace cv;
using namespace std;



/**
* @brief �ӹ��˺�Ĺ�����ͼ����Ѱ�Ҷ˵�
*/
std::vector<cv::Point> getendPoints(const cv::Mat &thinSrc)
{
	assert(thinSrc.type() == CV_8UC1);
	cv::Mat tmp;
	thinSrc.copyTo(tmp);
	std::vector<cv::Point> points;
	int m = 0;
	int n = 0;
	int count = 0;
	uchar pix;
	//����for ��ͼ���ϱ������еĵ�
	int height = tmp.rows;   //��ʾͼƬ�߶�
	int width = tmp.cols;    //��ʾͼƬ���
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {

			// See what the pixel is at this location
			// �õ���һ��

			//������һ��Ҫע��ĵط���i��Ӧ���ǵ��y���꣬j��Ӧ���ǵ��x���꣬����������ϰ�ߵģ�x, y��
			pix = tmp.at<uchar>(i, j);

			// If not a skeleton point, skip
			//��һ��Ϊ0��˵����һ�㲻��ͼ����Ϣ
			if (pix == 0)
				continue;

			// count ��������
			count = 0;



			//��Ϊ�Ź����һ��λ��Ϊ1������λ�ã�Ҳ�������棨i��j��,��СΪ16����������Կ���������ÿһ��ѭ��t�������2

			int t = 1;


			//��������forѭ���ǣ�������Ϊ�㣬Ȼ��������Ź���
			//�Ҿ���Ҳ����������ж��Ƿ��е㣬�еĻ����������ƶ�
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {


					//ע������ i+y��Ӧ����y���꣬j+x��Ӧx����,�����൱�ڴӾŹ���ĵ�һ�е�һ�У���һ�еڶ��С����� ���α���
					pix = tmp.at<uchar>(i + y, j + x);

					// Count if non-zero

					if (pix != 0)
						count = count + t;
					t = t * 2;   //�������Ķ���

				}
			}
			//��Щ���Ƕ˵��Ӧ��ֵ�������ǽ�����Ӧ��ֵ�������ǽǵ㣡����
			if (count == 17 || count == 18 || count == 20 || count == 24 || count == 48 || count == 80 || count == 144 || count == 272)

			{

				//ע��ǰ�ģ���������Ҫ����������

				//i == height == Point.y    �൱��y����

				//j == width == Point.x     �൱��x����
				Point point(j, i);
				points.push_back(point);
			}
		}

	}

	return points;
}




//֮ǰ����stackoverflow ��˵�Ĵ���
/*for (int i = 1; i < im.rows - 1; i++) {
	for (int j = 1; j < im.cols - 1; j++) {

		// See what the pixel is at this location
		pix = im.at<uchar>(i, j);

		// If not a skeleton point, skip
		if (pix == 0)
			continue;

		// Reset counter
		count = 0;

		// For each pixel in the neighbourhood
		// centered at this skeleton location...
		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {

				// Get the pixel in the neighbourhood
				pix = im.at<uchar>(i + y, j + x);

				// Count if non-zero
				if (pix != 0)
					count++;
			}
		}

		// If count is exactly 2, add co-ordinates to vector
		if (count == 2) {                                            //�������Ǳ���9��������ֻ�������϶����Ƕ˵���
			coords.push_back(i);
			coords.push_back(j);
		}
	}
}
*/
