#include "pch.h"
#include"vessel_depth.h"


int depth(Mat img, int x, int y)
{
	Vec3b pix = img.at<Vec3b>(y, x);
	//获取到RGB分量的值。


	/*uchar B = pix[0];
	uchar G = pix[1];
	uchar R = pix[2];
	int W1 = pix[0];
	int B1 = pix[0]*255;
	int G1 = pix[1]*255;
	int R1 = pix[2]*255;
	cout << "B   " << pix[0]<< endl;
	cout << "G   " << pix[1] << endl;
	cout << "R   " << pix[2]<<endl;
	cout << "W   " << W1 << endl;
	cout << "B   " << B1 << endl;
	cout << "G   " << G1 << endl;
	cout << "R   " << R1 << endl;
	int t = B + G + R;
	cout << "像素值为" << t<<endl;*/

	uchar B = pix[0];
	int d = B;
	return -d;
}
