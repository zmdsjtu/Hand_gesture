#pragma once  
#include<opencv2/opencv.hpp>
#include<vector>


cv::Mat RegionGrow(cv::Mat src, cv::Point2i pt, int th1, int th2, int th3, bool isornot);
