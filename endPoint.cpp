/*何昆霖的得到端点的代码*/

#include"pch.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>  
#include <vector>  
using namespace cv;
using namespace std;



/**
* @brief 从过滤后的骨骼化图像中寻找端点
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
	//两个for 在图像上遍历所有的点
	int height = tmp.rows;   //表示图片高度
	int width = tmp.cols;    //表示图片宽度
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {

			// See what the pixel is at this location
			// 得到这一点

			//其中有一个要注意的地方是i对应的是点的y坐标，j对应的是点的x坐标，而不是我们习惯的（x, y）
			pix = tmp.at<uchar>(i, j);

			// If not a skeleton point, skip
			//这一点为0，说明这一点不含图像信息
			if (pix == 0)
				continue;

			// count 用来计数
			count = 0;



			//因为九宫格第一个位置为1，中心位置，也就是上面（i，j）,大小为16，所以你可以看到我这里每一次循环t都会乘以2

			int t = 1;


			//下面两个for循环是，以中心为点，然后依次求九宫格
			//我觉得也可以用这个判断是否还有点，有的话就往那里移动
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {


					//注意这里 i+y对应的是y坐标，j+x对应x坐标,所以相当于从九宫格的第一行第一列，第一行第二列。。。 依次遍历
					pix = tmp.at<uchar>(i + y, j + x);

					// Count if non-zero

					if (pix != 0)
						count = count + t;
					t = t * 2;   //根据论文而来

				}
			}
			//这些都是端点对应数值，后面是交叉点对应数值，不考虑角点！！！
			if (count == 17 || count == 18 || count == 20 || count == 24 || count == 48 || count == 80 || count == 144 || count == 272)

			{

				//注意前文，故这里需要这样放坐标

				//i == height == Point.y    相当于y坐标

				//j == width == Point.x     相当于x坐标
				Point point(j, i);
				points.push_back(point);
			}
		}

	}

	return points;
}




//之前看的stackoverflow 求端点的代码
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
		if (count == 2) {                                            //精华就是遍历9个点后，如果只有两个肯定就是端点了
			coords.push_back(i);
			coords.push_back(j);
		}
	}
}
*/
