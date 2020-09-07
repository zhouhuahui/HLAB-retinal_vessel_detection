#include "pch.h"
#include"vessel_radius.h"

//���㵱ǰ��
float CalEntropy(Mat hist, int threshold)
{
	float BackgroundSum = 0, targetSum = 0;
	const float* pDataHist = (float*)hist.ptr<float>(0);
	for (int i = 0; i < 256; i++)
	{
		//�ۼƱ���ֵ
		if (i < threshold)
		{
			BackgroundSum += pDataHist[i];
		}
		//�ۼ�Ŀ��ֵ
		else
		{
			targetSum += pDataHist[i];
		}
	}

	float BackgroundEntropy = 0, targetEntropy = 0;
	for (int i = 0; i < 256; i++)
	{
		//���㱳����
		if (i < threshold)
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio1 = pDataHist[i] / BackgroundSum;//p[i]
														//���㵱ǰ������
			BackgroundEntropy += -ratio1 * logf(ratio1);
		}
		else  //����Ŀ����
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio2 = pDataHist[i] / targetSum;
			targetEntropy += -ratio2 * logf(ratio2);
		}
	}
	return (targetEntropy + BackgroundEntropy);
}

//Ѱ���������ֵ���ָ�
int MaxEntropy(Mat inputImage)
{
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0,256 };
	const float* ranges[1] = { pranges };
	MatND hist;
	calcHist(&inputImage, 1, channels, Mat(), hist, 1, histSize, ranges);
	float maxentropy = 0;
	int max_index = 0;
	Mat result;
	for (int i = 0; i < 256; i++)
	{
		float cur_entropy = CalEntropy(hist, i);
		if (cur_entropy > maxentropy)
		{
			maxentropy = cur_entropy;
			max_index = i;
		}
	}
	return max_index;
}

float calcEuclideanDiatance(int x1, int y1, int x2, int y2)//����ŷʽ����
{
	return sqrt(float((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}
float radius(Mat &tmp, int x, int y)
{
	int height = tmp.rows;   //��ʾͼƬ�߶�
	int width = tmp.cols;    //��ʾͼƬ���
	uchar pix;
	float min = 0;
	float r = 10;
	int flag = 0;
	//i  ��ʾ�߶� j��ʾ���
	for (int i = 1; i <= 6; i++)
	{
		for (int j = -i; j <= i; j++)
		{


			if (!((y - i) < 0 || (y - i) > (height - 1) || (x + j) < 0 || (x + j) > (width - 1)))
			{


				pix = tmp.at<uchar>(y - i, x + j);
				if (pix != 0)
				{
					min = calcEuclideanDiatance(x, y, x + j, y - i);
					if (min < r)
					{
						r = min;
					}
					flag = 1;
				}
			}

			if (!((y + j) < 0 || (y + j) > (height - 1) || (x - i) < 0 || (x - i) > (width - 1)))
			{


				pix = tmp.at<uchar>(y + j, x - i);
				if (pix != 0)
				{
					min = calcEuclideanDiatance(x, y, x - i, y + j);
					if (min < r)
					{
						r = min;
					}
					flag = 1;
				}
			}
			if (!((y + i) < 0 || (y + i) > (height - 1) || (x + j) < 0 || (x + j) > (width - 1)))
			{


				pix = tmp.at<uchar>(y + i, x + j);
				if (pix != 0)
				{
					min = calcEuclideanDiatance(x, y, x + j, y + i);
					if (min < r)
					{
						r = min;
					}
					flag = 1;
				}
			}
			if (!((y + j) < 0 || (y + j) > (height - 1) || (x + i) < 0 || (x + i) > (width - 1)))
			{

				pix = tmp.at<uchar>(y + j, x + i);
				if (pix != 0)
				{
					min = calcEuclideanDiatance(x, y, x + i, y + j);
					if (min < r)
					{
						r = min;
					}
					flag = 1;
				}
			}
		}
		if (flag == 1)
		{
			break;
		}

	}
	return r;
}
