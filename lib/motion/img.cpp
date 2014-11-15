#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h> 
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <Windows.h>

void motion_processing(std::deque<int> &motion, int offset, int befo_motion, int past_motion, int ones_size, int zeros_size){
	int median_sum;
	unsigned int big_counter = offset;
	int small_counter = 0;
	int break_flag = 0;

	//////////////////////////////////////////////////////////////////////////////////// FILTRACJA MEDIANOWA
	for(int i=offset; i<motion.size()-offset-1; i++){
		median_sum = 0;
		for( int j=i-offset; j<=i+offset; j++ ){
			median_sum+=motion[j];
		}
		if( median_sum > offset+0.5 ){
			motion[i] = 1;
		} else {
			motion[i] = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////// USUWANIE ZBYT KRÓTKIEGO RUCHU
	while( true ){
		small_counter = 0;

		while( motion[big_counter] == 0 ){
			++big_counter;
			if( big_counter == motion.size()-offset ){
				break_flag = 1;
				break;
			}
		}

		if( break_flag == 1 ){
			break;
		}

		while( motion[big_counter] == 1 ){
			++small_counter;
			++big_counter;
		}

		if( small_counter < ones_size ){
			while( small_counter > 0 ){
				motion[big_counter-small_counter] = 0;
				--small_counter;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////// SCALANIE SEGMENTÓW RUCHU SKLASYFIKOWANYCH JAKO JEDEN
	big_counter = offset;
	small_counter = 0;
	break_flag = 0;

	while( true ){
		small_counter = 0;

		while( motion[big_counter] == 1 ){
			++big_counter;
		}

		while( motion[big_counter] == 0 ){
			++small_counter;
			++big_counter;
			if( big_counter == motion.size() - offset ){
				break_flag = 1;
				break;
			}
		}

		if( break_flag == 1 ){
			break;
		}

		if( small_counter < zeros_size ){
			while( small_counter > 0 ){
				motion[big_counter-small_counter] = 1;
				--small_counter;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////// USUNIĘCIE KONTEKSTU FILTRACJI MEDIANOWEJ
	for(int i=0; i<offset; i++){
		motion.pop_front();
		motion.pop_back();
	}

	//for( int i = 0; i<motion.size(); ++i){ std::cout<<motion[i];}
	
	//////////////////////////////////////////////////////////////////////////////////// WYPRZEDZANIE RUCHU
	big_counter = 0;
	small_counter = 0;
	break_flag = 0;
	while( true ){
		while( motion[big_counter] == 0 ){
			++big_counter;
			if( big_counter == motion.size() ){
				break_flag = 1;
				break;
			}
		}

		if( break_flag == 1 ){
			break;
		}

		
		small_counter = 1;
		while( (small_counter <= befo_motion) && ((signed int)big_counter-(signed int)small_counter > 0) ){
			motion[big_counter-small_counter] = 1;
			++small_counter;
		}//jfghjkl

		while( motion[big_counter] == 1 ){
			++big_counter;
			if( big_counter == motion.size() ){
				break_flag = 1;
				break;
			}
		}

		if( break_flag == 1 ){
			break;
		}
		
		small_counter = 0;
		while( (small_counter < past_motion) && (big_counter+small_counter < motion.size()) ){
			motion[big_counter+small_counter] = 1;
			++small_counter;
		}

		while( motion[big_counter] == 1 ){
			++big_counter;
			if( big_counter == motion.size() ){
				break_flag = 1;
				break;
			}
		}

		if( break_flag == 1 ){
			break;
		}
	}
}

void save_motion(std::deque<int> &motion, cv::VideoCapture &movie){
	int movies_count = 0;
	int break_flag = 0;
	unsigned int index = 0;
	double fps = movie.get(CV_CAP_PROP_FPS);/*                        PROBLEM!                      */
    int frame_width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
    std::string video_name;
	cv::VideoWriter video;
	cv::Mat frame;

	movie.set(CV_CAP_PROP_POS_AVI_RATIO,0);
	while( true ){
		++movies_count;
		while( motion[index] == 0 ){
			movie >> frame;
			++index;
			if( index == motion.size() ){
				break_flag = 1;
				break;
			}
		}
		if( break_flag ){
			break;
		}

		video_name = std::string("video_") + std::to_string((long double)movies_count) + std::string(".avi");
		video.open(video_name,CV_FOURCC('M','P','E','G'),fps, cv::Size(frame_width,frame_height),true);

		while( motion[index] == 1 ){
			movie >> frame;
			++index;
			video.write(frame);
			if( index == motion.size() ){
				break_flag = 1;
				break;
			}
		}
		video.release();
		if( break_flag){
			break;
		}
	}
}

void motion_detection(std::string path, std::map<std::string,double> parameters){
	int frame_skip = parameters["frame_skip"]; 
	int zeros_size = parameters["zeros_size"];
	int ones_size = parameters["ones_size"];
	int offset = parameters["offset"];
	int befo_motion = parameters["befo_motion"];
	int past_motion = parameters["past_motion"];
	float requested_area = parameters["area"];
	int history = parameters["history"];
	int nmixtures = parameters["nmixtures"];
	int method = parameters["method"];
	int thread = parameters["thread"];

	cv::Mat frame;
    cv::Mat fore;
	cv::VideoCapture movie;
	cv::Mat element = cv::getStructuringElement(0, cv::Size(5,5));
    cv::BackgroundSubtractorMOG2 bg;
	bg.set("detectShadows", true); //Rozroznianie obiektow i ich cieni
	bg.set("nShadowDetection", 0); //Ignorowanie cieni
	bg.set("history",history); //Ustawienie historii
	bg.set("nmixtures",nmixtures); //Nie wiem

	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> tmp;

	movie.open(path);
	int height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
	int area = height*width;
	int flag = 0;
	unsigned int current_no = 0;
	int perimeter = 100;

	std::deque<int> motion;

	for(int i=0; i<offset; i++){
		motion.push_back(0);
	}

	cv::namedWindow("Motion");
	if( method == 1 ){
		while( true ){
			movie >> frame;
			if(frame.empty()){
				break;
			}
			bg.operator()(frame,fore);
			cv::erode(fore,fore,element);
			cv::dilate(fore,fore,element);
			cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
			for(int i=0; i<contours.size(); i++){
				if( contours[i].size()>perimeter && contourArea(contours[i])>(area*requested_area) ){
					tmp.push_back(contours[i]);
				}
			}

			if( tmp.size() > 0 ){
				flag = 1;
			} else {
				flag = 0;
			}

			//cv::drawContours(frame,tmp,-1,cv::Scalar(0,0,255),2);
			//cv::imshow("Motion",frame);
			//if(cv::waitKey(1) >= 5) break;
			motion.push_back(flag);
			current_no++;
			tmp.clear();
			for(int i=0; i<frame_skip; i++){
				movie>>frame;
				if(frame.empty()){
					break;
				}
				motion.push_back(flag);
				current_no++;
			}
		}
	} else {
		motion[offset] = 0;
		unsigned int deq_index = 1;
		cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(3,3));
		cv::Mat element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(15,15));
		cv::Mat frame_2;
		cv::Mat frame_1;
		cv::Mat frame_0;
		cv::Mat diff_1;
		cv::Mat diff_2;
		movie >> frame_2;
		movie >> frame_1;
		movie >> frame_0;
		cvtColor(frame_2, frame_2, CV_BGR2GRAY);
		cvtColor(frame_1, frame_1, CV_BGR2GRAY);
		cvtColor(frame_0, frame_0, CV_BGR2GRAY);
		current_no += 3;
		while( true ){
			cv::absdiff(frame_2,frame_1,diff_1);
			cv::absdiff(frame_1,frame_0,diff_2);
			cv::bitwise_and(diff_1,diff_2,frame);
			threshold(frame,frame,20,255,0);
			cv::erode(frame,frame,element);
			cv::dilate(frame,frame,element);

			frame_2 = frame_1;
			frame_1 = frame_0;
			movie >> frame_0;
			if(frame_0.empty()){
				break;
			}
			cvtColor(frame_0, frame_0, CV_BGR2GRAY);
			//cv::imshow("Motion",frame);
			//if(cv::waitKey(1) >= 10) break;
			current_no++;
			int pixel_sum = 0;
			int pixel = 0;
			for(int y=0;y<frame.rows;y++){
				for(int x=0;x<frame.cols;x++){
					pixel = frame.at<uchar>(y,x);
						pixel_sum = pixel_sum + pixel;
				}
			}
			if( pixel_sum > 8000 ){
				motion.push_back(1);
			} else {
				motion.push_back(0);
			}
		}
	}
	/*for(int i=0; i<motion.size(); i++){
		std::cout<<motion[i];
	}*/
	for(int i=0; i<offset; i++){
		motion.push_back(0);
	}
	motion_processing(motion,offset,befo_motion,past_motion,ones_size,zeros_size);
	save_motion(motion,movie);
	cv::destroyWindow("Motion");
}

int main(int argc, char *argv[])
{
	std::map<std::string,double> parametry;
	parametry["frame_skip"] = 1; 
	parametry["zeros_size"] = 7;
	parametry["ones_size"] = 9;
	parametry["offset"] = 2;
	parametry["befo_motion"] = 10;
	parametry["past_motion"] = 10;
	parametry["area"] = 0.001;
	parametry["history"] = 100;
	parametry["nmixtures"] = 3;
	parametry["method"] = 1;
	parametry["thread"] = 1;

	std::string path = "C://MOV_0013.mp4";

	motion_detection(path,parametry);
	system("pause");
    return 0;
}
