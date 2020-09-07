/*何昆霖的得到交叉点的代码*/
#include"pch.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>  
#include <vector>  
using namespace cv;
using namespace std;

/**
* @brief 从过滤后的骨骼化图像中寻找交叉点
*/
std::vector<cv::Point> getcrossPoints(const cv::Mat &thinSrc)
{
	assert(thinSrc.type() == CV_8UC1);
	int width = thinSrc.cols;
	int height = thinSrc.rows;
	cv::Mat tmp;
	thinSrc.copyTo(tmp);
	std::vector<cv::Point> points;
	int m = 0;
	int n = 0;
	int count = 0;
	uchar pix;

	for (int i = 1; i < tmp.rows - 1; i++) {
		for (int j = 1; j < tmp.cols - 1; j++) {

			// See what the pixel is at this location
			pix = tmp.at<uchar>(i, j);

			// If not a skeleton point, skip
			if (pix == 0)
				continue;

			// Reset counter
			count = 0;
			int t = 1;
			// For each pixel in the neighbourhood
			// centered at this skeleton location...
			for (int y = -1; y <= 1; y++) {
				for (int x = -1; x <= 1; x++) {

					// Get the pixel in the neighbourhood
					pix = tmp.at<uchar>(i + y, j + x);

					// Count if non-zero
					if (pix != 0)
						count = count + t;
					t = t * 2;
				}
			}
			// If count is exactly 2, add co-ordinates to vector
			if (count == 58 || count == 85 || count == 114 || count == 115 || count == 117 || count == 122 || count == 154 || count == 156 || count == 157 || count == 158 || count == 177 || count == 178 || count == 179 || count == 181 \
				|| count == 184 || count == 185 || count == 186 || count == 188 || count == 189 || count == 213 || count == 241 || count == 242 || count == 243 || count == 245 || count == 277 || count == 282 || count == 285 || count == 286\
				|| count == 314 || count == 337 || count == 339 || count == 340 || count == 341 || count == 342 || count == 346 || count == 348 || count == 350 || count == 369 || count == 370 || count == 371 || count == 378 || count == 405 || count == 410\
				|| count == 412 || count == 413 || count == 414)

			{
				Point point(j, i);
				points.push_back(point);
			}
		}

	}

	return points;
}
