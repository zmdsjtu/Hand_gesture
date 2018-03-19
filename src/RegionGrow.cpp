#include "RegionGrow.h"

cv::Mat RegionGrow(cv::Mat src, cv::Point2i pt, int R, int G, int B, bool isornot)
{
	cv::Point2i ptGrowing;                      //待生长点位置  
	int nGrowLable = 0;                             //标记是否生长过  
	std::vector<int> nSrcValue = { 0,0,0 };                             //生长起点灰度值  
	std::vector<int> nCurValue = { 0,0,0 };                              //当前生长点灰度值  
	cv::Mat matDst = cv::Mat::zeros(src.size(), CV_8UC1);   //创建一个空白区域，填充为黑色  
													//生长方向顺序数据  
	int DIR[8][2] = { { 0,-1 },{ 0,1 },{ 1,0 },{ -1,0 },{ 1,1 },{ -1,1 },{ -1,-1 },{ 1,-1 } };
	//{ { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
	std::vector<cv::Point2i> vcGrowPt;                     //生长点栈  
	vcGrowPt.push_back(pt);                         //将生长点压入栈中  
	matDst.at<uchar>(pt.y, pt.x) = 255;               //标记生长点  
	for (int i = 0; i < 3; ++i)
		nSrcValue[i] = src.at<cv::Vec3b>(pt.y, pt.x)[i];
	while (!vcGrowPt.empty())                       //生长栈不为空则生长  
	{
		pt = vcGrowPt.back();                       //取出一个生长点  
		vcGrowPt.pop_back();

		//分别对八个方向上的点进行生长  
		for (int i = 0; i<4; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];
			ptGrowing.y = pt.y + DIR[i][1];
			//检查是否是边缘点  
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);      //当前待生长点的灰度值  

			if (nGrowLable == 0)                    //如果标记点还没有被生长  
			{
				for (int i = 0; i<3; ++i)
					nCurValue[i] = src.at<cv::Vec3b>(pt.y, pt.x)[i];

				if (isornot) {
					if ((abs(nSrcValue[0] - nCurValue[0]) < R) &&
						(abs(nSrcValue[1] - nCurValue[1]) < G) &&
						(abs(nSrcValue[2] - nCurValue[2]) < B)
						)                 //在阈值范围内则生长  
					{
						matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;     //标记为白色  
						vcGrowPt.push_back(ptGrowing);                  //将下一个生长点压入栈中  
					}
				}
				else {
					if ((abs(nSrcValue[0] - nCurValue[0]) < R) ||
						(abs(nSrcValue[1] - nCurValue[1]) < G) ||
						(abs(nSrcValue[2] - nCurValue[2]) < B)
						)                 //在阈值范围内则生长  
					{
						matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;     //标记为白色  
						vcGrowPt.push_back(ptGrowing);                  //将下一个生长点压入栈中  
					}
				}

			}
		}
	}
	return matDst.clone();
}
