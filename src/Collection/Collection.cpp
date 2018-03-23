//Time:2018-2-4
//Author:Mingde Zhu, zmdsjtu@163.com
//Function: to collect the pictures/videos for gesture training

#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
using namespace std;
using namespace cv;
void put_helptxt(Mat &frame);
int main() {
	Mat frame;
	VideoCapture cap(0);
	int label = 0;
	char c;
	bool show_helptext = true;
	string name = "";
	cin >> name;
	name += "_";
	VideoWriter writer;
	bool video_start = false;
	while (c = waitKey(1)) {
		cap >> frame;
		if (video_start)writer << frame;
		if (c == 27)break;
		//巴掌
		else if (c == '1')imwrite("palm//" +name+ (to_string(++label) + ".jpg"), frame);
		//剪刀
		else if (c == '2')imwrite("scissor//" + name + (to_string(++label) + ".jpg"), frame);
		//拳头
		else if (c == '3')imwrite("fist//" + name + (to_string(++label) + ".jpg"), frame);
		//大拇指
		else if (c == '4')imwrite("thumb//" + name + (to_string(++label) + ".jpg"), frame);
		//rock
		else if (c == '5')imwrite("rock//" + name + (to_string(++label) + ".jpg"), frame);
		//OK
		else if (c == '6')imwrite("OK//" + name + (to_string(++label) + ".jpg"), frame);
		//控制是否显示帮助信息
		else if (c == 'h')show_helptext = !show_helptext;
	    //变更前缀
		else if (c == 'c') { cin >> name; name += "_"; }

		else if (c == 'v') { 
			video_start = !video_start;
			if(video_start)
			writer= VideoWriter(name+".avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(640, 480)); 
		}
		

		if (show_helptext)put_helptxt(frame);

		imshow("采集", frame);
	}
	return 0;
}
void put_helptxt(Mat &frame) {
	putText(frame, "1--palm", Point(10, 20), 2, 1, Scalar(0, 0, 255));
	putText(frame, "2--scissor", Point(10, 60), 2, 1, Scalar(0, 0, 255));
	putText(frame, "3--fist", Point(10, 100), 2, 1, Scalar(0, 0, 255));
	putText(frame, "4--thumb", Point(10, 140), 2, 1, Scalar(0, 0, 255));
	putText(frame, "5--rock", Point(10, 180), 2, 1, Scalar(0, 0, 255));
	putText(frame, "6--OK", Point(10, 220), 2, 1, Scalar(0, 0, 255));
}