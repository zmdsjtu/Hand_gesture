

#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <iostream>
#include <fstream>
#include "dlib\image_io.h"
#include<opencv2/opencv.hpp>


using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------
static cv::Rect dlibRectangleToOpenCV(dlib::rectangle r)
{
	return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}
static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
	return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
int main(int argc, char** argv)
{

	try
	{
		//一、preprocessing
		//1. 载入训练集，测试集
		const std::string faces_directory = "C:\\Users\\zhumingde\\Desktop\\采集\\thumb\\";
		const std::string test_directory = "C:\\Users\\zhumingde\\Desktop\\采集\\palm\\";
		dlib::array<array2d<unsigned char> > images_train, images_test;
		std::vector<std::vector<rectangle> > face_boxes_train, face_boxes_test;

		load_image_dataset(images_train, face_boxes_train, faces_directory + "train.xml");
		load_image_dataset(images_test, face_boxes_test, test_directory + "train.xml");
		/*
		double 倍数 = 2;
		for (auto&reclist : face_boxes_train)
		{
			for (auto&rec : reclist)
			{
				cv::Rect temp = dlibRectangleToOpenCV(rec);
				temp.width *= 倍数;
				temp.height *= 倍数;
				rec = openCVRectToDlib(temp);
			}
		}*/

		//2.图片上采样
		upsample_image_dataset<pyramid_down<8> >(images_train, face_boxes_train);
		upsample_image_dataset<pyramid_down<8> >(images_test, face_boxes_test);

		//3.训练图片做镜像处理，扩充训练集
		//add_image_left_right_flips(images_train, face_boxes_train);

		//二、training
		//1.定义scanner类型，用于扫描图片并提取特征（HOG）
		typedef scan_fhog_pyramid<pyramid_down<8> > image_scanner_type;
		image_scanner_type scanner;

		//2. 设置scanner扫描窗口大小
		scanner.set_detection_window_size(40,64);
		
		//scanner.set_cell_size(5);
		//3.定义trainer类型（SVM），用于训练检测器                
		structural_object_detection_trainer<image_scanner_type> trainer(scanner);
		// Set this to the number of processing cores on your machine.
		trainer.set_num_threads(4);
		// 设置SVM的参数C，C越大表示更好地去拟合训练集，当然也有可能造成过拟合。通过尝试不同C在测试集上的效果得到最佳值
		trainer.set_c(20);
		trainer.be_verbose();
		//设置训练结束条件，"risk gap"<0.01时训练结束，值越小表示SVM优化问题越精确，训练时间也会越久。
		//通常取0.1-0.01.在verbose模式下每一轮的risk gap都会打印出来。
		trainer.set_epsilon(0.01);

		//4.训练，生成object_detector
		object_detector<image_scanner_type> detector = trainer.train(images_train, face_boxes_train);

		//三、测试
		// 输出precision, recall, average precision.
		cout << "training results: " << test_object_detection_function(detector, images_train, face_boxes_train) << endl;
		cout << "testing results:  " << test_object_detection_function(detector, images_test, face_boxes_test) << endl;

		//显示hog
		image_window hogwin(draw_fhog(detector), "Learned fHOG detector");



		//四、模型存储
		serialize(faces_directory+"jieguo.svm") << detector;
		// you can recall it using the deserialize() function.
	//	object_detector<image_scanner_type> detector2;
	//	deserialize("face_detector.svm") >> detector2;
		// 显示测试集的人脸检测结果
		image_window win;
		for (unsigned long i = 0; i < images_test.size(); ++i)
		{
			// Run the detector and get the face detections.
			std::vector<rectangle> dets = detector(images_test[i]);
			win.clear_overlay();
			win.set_image(images_test[i]);
			win.add_overlay(dets, rgb_pixel(255, 0, 0));
			cout << "Hit enter to process the next image..." << endl;
			cin.get();
		}

	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
		cout << 233;
		system("pause");
	}
}