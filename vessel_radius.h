#pragma once

#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include <iostream>  
#include<fstream>

using namespace cv;
using namespace std;

float CalEntropy(Mat hist, int threshold);
int MaxEntropy(Mat inputImage);
float calcEuclideanDiatance(int x1, int y1, int x2, int y2);
float radius(Mat &tmp, int x, int y);
