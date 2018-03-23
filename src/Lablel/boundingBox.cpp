#include <opencv2/opencv.hpp>
// selectROI is part of tracking API
#include <opencv2/tracking.hpp>
#include<iostream>  
#include<fstream>    
#include<string>  
#include<direct.h>
using namespace std;
using namespace cv;
void 写入TXT(string 名字, string 内容, bool 是否覆盖) {
	ofstream 写入(名字, 是否覆盖 ? ios::trunc : ios::app);

	if (写入.is_open()) {//如果成功的话       
		写入 << 内容 << endl;
		写入.close();
	}
}
int main(int argc, char **arv)
{
	fstream 读文件;
	string 目录 = "C:\\Users\\zhumingde\\Desktop\\采集\\thumb";
	读文件.open(目录+"\\1.txt");
	_mkdir((目录+"\\Crop").c_str());
	// Read image
	while (waitKey(30) != 27&& !读文件.eof()) {

			string 读入的东西;
			读文件 >> 读入的东西;
			if (读入的东西.empty())break;
			写入TXT(目录+"\\Rect.txt", 读入的东西, 0);

		Mat im = imread(目录 + "\\"+读入的东西);

		// Select ROI
		bool showCrosshair = false;
		bool fromCenter = false;
		Rect2d r = selectROI("Image", im, fromCenter, showCrosshair);
		cout << r.x << " " << r.y << " " << r.width << " " << r.height << endl;
		// Crop image
		Mat imCrop = im(r);
		string temp = to_string(int(r.x)) + " " + to_string(int(r.y)) + " " + to_string(int(r.width)) + " " + to_string(int(r.height));
		写入TXT(目录 + "\\Rect.txt", temp, 0);
		// Display Cropped Image
		imshow("Image_crop", imCrop);
		imwrite(目录 + "\\crop\\" + 读入的东西, imCrop);
		//waitKey(0);
	}
	读文件.close();//关闭是个好习惯  

	return 0;
}

