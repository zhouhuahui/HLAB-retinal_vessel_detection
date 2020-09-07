#pragma once

#include"pch.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>  
#include <vector>  
using namespace cv;
using namespace std;

std::vector<cv::Point> getendPoints(const cv::Mat &thinSrc);
