#include <opencv2/opencv.hpp>
#include <dlib/svm_threaded.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>  
//#include <Windows.h>
#include<vector>
#include<string>
#include<algorithm>
#include<iostream>
#include <opencv2/tracking.hpp>  
#include<unordered_map>
//#include "Time_count.h"
#include "RegionGrow.h"
#include "cvui.h"
#include "Find_contours.h"
#include "jigsaw.h"
#define WINDOW_NAME "Control_panel"  
#define SHOW_RESULT
#define REGION_GROW
//#define FINDCOUNTER
//#define SHOW_POINTS
//#define SHOW_TIMECOUNT
#define SCALE 3

using namespace std;
using namespace dlib;

static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
	return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}
static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
	return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
string whichgesture(cv::Rect temp) {
	double ratio = 1.0*(temp.width) / (temp.height);
	if (ratio < 1.1&&ratio>0.9)return "palm";
	else if (ratio > 1)return "fist";
	else if (ratio < 0.6)return "thumb";
	else return "scissor";
}
int which_num(string result) {
	if (result == "palm")return 0;
	else if (result == "thumb")return 1;
	else if (result == "scissor")return 2;
	else return 3;
}
double palm_threshold = 0, fist_threshold = 0.6, scissor_threshold = 0.6, thumb_threshold = 0.6;
double threshold(string temp) {
	if (temp == "palm")return palm_threshold;
	else if (temp == "fist")return fist_threshold;
	else if (temp == "scissor")return scissor_threshold;
	else if (temp == "thumb")return thumb_threshold;
}
void sort_confidence(std::vector<rect_detection>&dets) {
	if (dets.size() < 2)return;
	double num_1 = dets[0].detection_confidence+threshold(whichgesture(dlibRectangleToOpenCV(dets[0].rect)));
	double num_2 = dets[1].detection_confidence+ threshold(whichgesture(dlibRectangleToOpenCV(dets[1].rect)));
	if (num_1 < num_2) {
		rect_detection temp = dets[0];
		dets[0] = dets[1];
		dets[1] = temp;
	}
	return;
}
cv::Rect overlap(cv::Rect rect1, cv::Rect rect2) {
	int x1 = rect1.tl().x, x2 = rect2.tl().x, y1 = rect1.tl().y, y2 = rect2.tl().y;
	int x3 = x1 + rect1.width, x4 = x2 + rect2.width, y3 = y1 + rect1.height, y4 = y2 + rect2.height;
	int width = min(x3, x4) - max(x1, x2);
	int height = min(y3, y4) - max(y1, y2);
	return cv::Rect(max(x1, x2), max(y1, y2), width, height);
}
bool compare_bbox(cv::Rect rect1, cv::Rect rect2) {
	//to determin wheter the track function is going on well
	double scale_1 = rect1.width*1.0 / rect1.height;
	double scale_2 = rect2.width*1.0 / rect2.height;
	if (abs(scale_1 - scale_2) > 0.1)return false;
	/*
	double distance_1 = abs(rect1.tl().x-rect2.tl().x);
	double distance_2 = abs(rect1.tl().x - rect2.tl().x + rect1.width - rect2.width);
	if (distance_1 > 30 || distance_2 > 30)return false;
	*/
	int x1 = rect1.tl().x, x2 = rect2.tl().x, y1 = rect1.tl().y, y2 = rect2.tl().y;
	int x3 = x1 + rect1.width, x4 = x2 + rect2.width, y3 = y1 + rect1.height, y4 = y2 + rect2.height;
	int width = min(x3, x4) - max(x1, x2);
	int height = min(y3, y4) - max(y1, y2);
	int area = width*height;
	cout << area*1.0 / rect1.area() << endl;
	if (area*1.0 / rect1.area() < 0.75)return false;

	return true;
}
void draw_mask(cv::Mat &test,cv::Rect bbox, std::vector<shape_predictor>pose_model,cv::Mat &mask) {
	
	string result = whichgesture(bbox);
	int NUM_gesture = which_num(result);

	std::vector<std::vector<int>>num_count = {
	{0, 1, 12, 23, 30, 31, 32, 33, 34, 35, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29  },//palm
		{ 0,1,6,7,8,9,10,11,12,13,2,3,4,5 },//thumb
		{ 0,1,6,7,8,9,10,11,12,13,2,3,4,5 },//scissor
		{ 0,1,6,7,8,9,10,11,12,13,2,3,4,5 }//fist
	};

		// Find the pose of each hand.  
		std::vector<full_object_detection> shapes;
			shapes.push_back(pose_model[NUM_gesture](cv_image<bgr_pixel>(test),openCVRectToDlib(bbox)));

		if (!shapes.empty()) {
			std::vector<cv::Point>pointset_(num_count[NUM_gesture].size());// pointset(36);
			for (int i = 0; i <num_count[NUM_gesture].size(); i++) {
				pointset_[i] = cv::Point(shapes[0].part(num_count[NUM_gesture][i]).x(), shapes[0].part(num_count[NUM_gesture][i]).y());
				//pointset[i]= cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y());
#ifdef SHOW_POINTS
				circle(test, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
				cv::putText(test, to_string(i), cvPoint(shapes[0].part(num_count[NUM_gesture][i]).x(), shapes[0].part(num_count[NUM_gesture][i]).y()), 1, 1, cv::Scalar(255, 0, 0));
#endif // SHOW_POINTS

			}
			std::vector<std::vector<cv::Point>>pointset_temp(1);
			pointset_temp[0]=pointset_;


			drawContours(mask, pointset_temp, 0, cv::Scalar(255, 0, 0),-1);
			
			//addWeighted(resImage, 0.3, test, 0.7, 0, test);
				//contours, index, Scalar::all(index + 1), -1, 8, hierarchy, INT_MAX);


			/*  //for palm
			cv::line(test, pointset[3], pointset[2], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[3], pointset[4], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[4], pointset[5], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[5], pointset[6], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[6], pointset[7], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[7], pointset[14], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[14], pointset[20], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[20], pointset[27], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[27], pointset[23], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[23], pointset[30], cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[2], pointset[30], cv::Scalar(255, 0, 0), 2);

			cv::line(test, (pointset[7] + pointset[14]) / 2, (pointset[8] + pointset[13]) / 2, cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[10], (pointset[8] + pointset[13]) / 2, cv::Scalar(255, 0, 0), 2);

			cv::line(test, (pointset[20] + pointset[14]) / 2, (pointset[15] + pointset[19]) / 2, cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[17], (pointset[15] + pointset[19]) / 2, cv::Scalar(255, 0, 0), 2);

			cv::line(test, (pointset[20] + pointset[27]) / 2, (pointset[21] + pointset[26]) / 2, cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[24], (pointset[21] + pointset[26]) / 2, cv::Scalar(255, 0, 0), 2);

			cv::line(test, (pointset[27] + pointset[23]) / 2, (pointset[28] + pointset[12]) / 2, cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[0], (pointset[28] + pointset[12]) / 2, cv::Scalar(255, 0, 0), 2);

			cv::line(test, (pointset[30] + pointset[2]) / 2, (pointset[31] + pointset[35]) / 2, cv::Scalar(255, 0, 0), 2);
			cv::line(test, pointset[33], (pointset[31] + pointset[35]) / 2, cv::Scalar(255, 0, 0), 2);

			*/


	}
}

int main(int argc, char *argv[]) {

	double detect_shreshold = -0.3;	
	cv::Mat frame(250, 250, CV_8UC3);
	cv::namedWindow(WINDOW_NAME);
	cvui::init(WINDOW_NAME);

	typedef scan_fhog_pyramid<pyramid_down<2> > image_scanner_type;
	typedef scan_fhog_pyramid<pyramid_down<8> > image_scanner_type2;
	typedef scan_fhog_pyramid<pyramid_down<16> > image_scanner_type3;

	object_detector < image_scanner_type2>   palm, fist, scissor, thumb;
	std::vector<object_detector<image_scanner_type2> > my_detectors;

	//TIMECOUNT Time_count;
	//palm 60*60
	//thumb 40*64
	//scissor 40*60
	//fist 60*50
	deserialize("thumb.svm") >> thumb;
	my_detectors.push_back(thumb);
	deserialize("palm.svm") >> palm;
	my_detectors.push_back(palm);
	deserialize("scissor.svm") >> scissor;
	my_detectors.push_back(scissor);
	deserialize("fist.svm") >> fist;
	my_detectors.push_back(fist);
	std::vector<shape_predictor> pose_model(4);

	deserialize("sp_palm.dat") >> pose_model[0];
	deserialize("sp_thumb.dat") >> pose_model[1];
	deserialize("sp_scissor.dat") >> pose_model[2];
	deserialize("sp_fist.dat") >> pose_model[3];
	cv::VideoCapture cap;
	
	if (argc == 1)cap=cv::VideoCapture(0);
	
	else {
		string video_path = argv[1];
		if (video_path.size()<2)
			cap = cv::VideoCapture(atoi(video_path.c_str()));
		else
			cap = cv::VideoCapture(video_path);
	}

	
	double Minification =SCALE;
	cv::VideoWriter writer;
	bool video_start = false;

	cv::Ptr<cv::TrackerMedianFlow> tracker = cv::TrackerMedianFlow::create();

	bool track_state = false;
	cv::Rect2d bbox;

	std::string show_result;
	double low_threshold = 50, high_threshold = 150;
	while (char c=cv::waitKey(10)) {
		//Time_count.START();

		cvui::window(frame, 0, 0, 250, 230, "Setting");
		cvui::text(frame, 10, 35, "threshold", 0.4);
		cvui::trackbar(frame,60, 20, 165, &detect_shreshold, -1.0, 1.0);
		cvui::text(frame, 10, 75, "palm", 0.4);
		cvui::trackbar(frame, 60, 60, 165, &palm_threshold, -1.0, 1.0);
		cvui::text(frame, 10, 115, "fist", 0.4);
		cvui::trackbar(frame, 60, 100, 165, &fist_threshold, -1.0, 1.0);
		cvui::text(frame, 10, 155, "scissor", 0.4);
		cvui::trackbar(frame, 60, 140, 165, &scissor_threshold, -1.0, 1.0);//scissor_threshold
		cvui::text(frame, 10, 195, "thumb", 0.4);
		cvui::trackbar(frame, 60, 180, 165, &thumb_threshold, -1.0, 1.0);//thumb_threshold

		if (c == 27)break;
		/*
		else if (c == 'v') {
			video_start = !video_start;
			if (video_start)
				writer = cv::VideoWriter("Test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, cv::Size(640, 480));
		}*/
		
		cv::Mat test;
		cap >> test;
		cv::Mat image_show=test.clone();
		if (video_start)writer << test;
		if (test.empty())break;

		if(track_state){
			track_state = tracker->update(test, bbox);

#ifdef FINDCOUNTER
			cv::Mat HSV_frame = test(overlap(bbox, cv::Rect(0, 0, test.cols, test.rows))).clone();
			help_findhandcounters(HSV_frame);
			cv::imshow("HSV_segmentation", HSV_frame);
#endif // FINDCOUNTER		

#ifdef SHOW_RESULT
			cv::rectangle(image_show, bbox, cv::Scalar(0, 0, 255));
			cv::putText(image_show, show_result,
				bbox.tl(), 2, 1, cv::Scalar(0, 0, 255));
#endif 
			
			cv::Mat mask(test.size(), CV_8UC3, cv::Scalar(0, 0, 0));
			draw_mask(image_show, bbox,pose_model,mask);
			montage(frame, mask, cv::Rect(0, 240, 240, 240), cv::Scalar(0));
			//imshow("crop", mask);
	
		}
		else {
			//to disable the mask
			cv::Mat mask(test.size(), CV_8UC3, cv::Scalar(0, 0, 0));
			montage(frame, mask, cv::Rect(0, 240, 240, 240), cv::Scalar(0));
		}
	

		cv::Mat test2 = test.clone();
		cv::resize(test2, test2, cv::Size(640 / Minification, 480 / Minification));
		cv_image<bgr_pixel> cimg(test2);


		std::vector<rect_detection> dets;
		evaluate_detectors(my_detectors, cimg, dets,detect_shreshold);

		//sort the confidence
		sort_confidence(dets);

		std::vector<dlib::rectangle>hands;// = evaluate_detectors(my_detectors, cimg, 0);
		for (unsigned long i = 0; i < dets.size(); ++i)
			hands.push_back(dets[i].rect);

		for (int i = 0; i <(hands.size()?1:0 ); ++i) {

			hands[i].set_top(hands[i].top()*Minification);
			hands[i].set_bottom(hands[i].bottom() * Minification);
			hands[i].set_left(hands[i].left()*Minification);
			hands[i].set_right(hands[i].right()*Minification);
			cv::Rect temp = dlibRectangleToOpenCV(hands[i]);

			if (!track_state) {
				bbox = temp;
				*tracker = *(cv::TrackerMedianFlow::create());
				tracker->init(test,bbox);
				track_state = true;
			}
			if (track_state) {
				track_state = compare_bbox(bbox, temp);
			}

			
#ifndef REGION_GROW
			//Try the region_grow method... bad,sometimes...
			Mat grow_result = RegionGrow(test(temp), Point2i(test(temp).cols / 2, test(temp).rows / 2), R, G, B,true);
			imshow("grow_result", grow_result);
#endif // !REGION_GROW

			string result = whichgesture(temp);	
		//	cv::rectangle(test, temp, cv::Scalar(255, 0, 0), 2);
			show_result = result;
		//	cv::putText(test, result + " " + to_string(dets[i].detection_confidence),
		//	temp.tl(), 2, 1, cv::Scalar(0, 0, 255));
		//	cv::putText(test, to_string(hands[i].width())+" "+ to_string(hands[i].height()), dlibRectangleToOpenCV(hands[i]).tl(), 1, 1, cv::Scalar(255));		
		}
		//Time_count.END();


#ifdef SHOW_TIMECOUNT
		//cv::putText(image_show, to_string((Time_count.TIME * 1000)) + "ms", cv::Point(50, 50), 2, 1, cv::Scalar(255));	
#endif // SHOW_TIMECOUNT

		//cv::imshow("test", image_show);
		montage(frame, image_show, cv::Rect(250,0,640,480), cv::Scalar(0));
		cvui::update();
		cv::imshow(WINDOW_NAME, frame);

	}
	//cv::waitKey(-1);
	return 0;

}
