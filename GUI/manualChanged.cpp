#include <opencv2\opencv.hpp>
#include <vector>
#include <deque>
#include <map>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <fstream>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <QLabel>

extern void motion_processing(std::deque<int> &motion, int offset, int beforeMotion, int pastMotion, int onesSize, int zerosSize);
extern int find_char(std::string path, char c);
extern std::string find_filename(std::string path);

struct movieFragment {
	int begin;
	int end;
	std::string name;
};

void moveOneFrameEarlier(int &curFrame, std::deque<std::vector<uchar>> allFrames, int data_storage);
void moveFiveFramesEarlier(int &curFrame, std::deque<std::vector<uchar>> allFrames, int data_storage);
void moveOneFrameLater(int &curFrame, std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, int data_storage);
void moveFiveFramesLater(int &curFrame, std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, int data_storage);
void stopVideo(bool &esc);
void exitFromManualMode(bool &stop);
void setMovieTime(std::deque<int> motion, std::deque<std::vector<uchar>> allFrames, double &momentFilmu, int &curFrame, int data_storage);
cv::Mat playVideo(std::deque<int> motion, std::deque<std::vector<uchar>> allFrames, bool &stop, bool &esc, int &curFrame, double &momentFilmu, char &k, int mSec, int data_storage);
void setBegining(int &begin, int curFrame);
void setEnd(int &end, int curFrame);
void moveToBegining(int &curFrame, int begin, std::deque<std::vector<uchar>> allFrames, int data_storage);
void moveToEnd(int &curFrame, int end, std::deque<std::vector<uchar>> allFrames, int data_storage);
void takeNextFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame); 
void takePreviousFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame); 
void saveVideoToFile(cv::VideoWriter video, std::vector<movieFragment> fragmentList, int &curFragment, int &curFrame, int &begin, int &end, double fps, int width, int height, std::deque<std::vector<uchar>> allFrames, int data_storage);
void setBeginFromFrameNo(int &begin, int frameNo, std::deque<int> motion);
void setEndFromFrameNo(int &end, int frameNo, std::deque<int> motion);
//void manualMode(std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, cv::VideoCapture &movie, int width, int height, int data_storage, int counter);


void motion_detection(std::string path, std::map<std::string,double> parameters, std::deque<std::vector<uchar>> &allFrames, std::deque<int> &motion, cv::VideoCapture &movie, int &width, int &height, int &counter){
        //odczyt parametrów
    int frame_skip = parameters["frame_skip"];
    int zeros_size = parameters["zeros_size"];
    int ones_size = parameters["ones_size"];
    int offset = std::min(zeros_size,ones_size)/2;
    int befo_motion = parameters["befo_motion"];
    int past_motion = parameters["past_motion"];
    float requested_area = parameters["area"];
    int history = parameters["history"];
    int nmixtures = parameters["nmixtures"];
    int method = parameters["method"];
    int thread = parameters["thread"];
	int data_storage = parameters["data_storage"];
 
    cv::Mat frame;
	cv::Mat fore;
    //cv::VideoCapture movie;
    cv::Mat element = cv::getStructuringElement(0, cv::Size(5,5));
	cv::BackgroundSubtractorMOG2 bg;

	//zmienne do zapisywania na dysk
	//int counter = 0;
	std::string pic_name;
	cv::Mat src;

	//motion_detection(path,parametry, allFrames, motion, movie, width, height, data_storage, counter);

	// zmienne do przechowywania w RAMie
	//std::deque<std::vector<uchar>> allFrames;
	std::vector<uchar> buff; 
	std::vector<int> param = std::vector<int>(2);
    param[0] = CV_IMWRITE_JPEG_QUALITY;
    param[1] = 95;

	if (data_storage == 1)
	{
		mkdir("images");
		chdir("images");
	}
	else
	{

	}
       
    // ustawienie dodatkowych parametrów metody
    bg.set("detectShadows", true); // rozróżnianie obiektów i cieni
    bg.set("nShadowDetection", 0); // ignorowanie cieni
    bg.set("history",history); // ilość ramek z jakich wyznaczany jest model tła
    bg.set("nmixtures",nmixtures); // ilość mieszanin gaussowskich
 
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> tmp;
 
    movie.open(path);
	
	movie.set(CV_CAP_PROP_POS_AVI_RATIO,0);

    height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
    width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
    int area = height*width;
    int flag = 0;
    int perimeter = 100;
 
    // główny wektor ruchu
    //std::deque<int> motion;
	
    // tworzenie kontekstu dla filtracji medianowej
    for(int i=0; i<offset; i++){
            motion.push_back(0);
    }
 
    //cv::namedWindow("Motion");
    if( method == 1 ){ // metoda mieszanin gaussowskich
            // pętla odczytująca i przetwarzająca kolejne ramki
            while( true ){
                    if(!movie.read(frame)){
                            break;
                    }
					
					if (data_storage == 1)
					{
						pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
						if (!imwrite(pic_name,frame))
						{
							std::cout << "Nie udalo sie" << std::endl;
							system("pause");
						}
						counter++;
					}
					else
					{
						imencode(".jpg",frame,buff,param);
						allFrames.push_back(buff);
					}
                    // funkcja wyznaczjąca model tła
                    bg.operator()(frame,fore);
                    // eliminacja drobnych zakłóceń
                    cv::erode(fore,fore,element);
                    cv::dilate(fore,fore,element);
 
                    // eliminacja obiektów niespełniających wymogów detekcji (obiekty o małym obwodzie i polu - potencjalne zakłócenia)
                    cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
                    for(int i=0; i<contours.size(); i++){
                            if( contours[i].size()>perimeter && contourArea(contours[i])>(area*requested_area) ){
                                    tmp.push_back(contours[i]);
                            }
                    }
                       
                    // jeśli ilość obiektów spełniajacych założenia detekcj jest większa od zera to ruch występuje
                    if( tmp.size() > 0 ){
                            flag = 1;
                    } else {
                            flag = 0;
                    }
                       
                    // opcjonalne wyświetlanie ramki z zaznaczonym ruchem
                    //cv::drawContours(frame,tmp,-1,cv::Scalar(0,0,255),2);
                    //cv::imshow("Motion",frame);
                    //if(cv::waitKey(1) >= 5) break;
 
                    motion.push_back(flag);
                    tmp.clear();
                       
                    // opuszczanie zadanej liczby kolejnych ramek (celem przyspieszenia obliczeń kosztem precyzji)
                    for(int i=0; i<frame_skip; i++)
					{
						if(!movie.read(frame))
						{
							break;
						}
						motion.push_back(flag);
						if (data_storage == 1)
						{
							pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
							imwrite(pic_name,frame);
							counter++;
						}
						else
						{
							imencode(".jpg",frame,buff,param);
							allFrames.push_back(buff);
						}
                    }
            }
    } 
	else 
	{ // metoda odejmowania ramek
        unsigned int deq_index = 1;
        cv::Mat frame_2;
        cv::Mat frame_1;
        cv::Mat frame_0;
        cv::Mat diff_1;
        cv::Mat diff_2;
 
        // odczyt pierwszych 3 ramek z pominięciem zadanej ilości pomiędzy (celem przyspieszenia obliczeń kosztem precyzji)
        movie >> frame_2;
		motion.push_back(1);
		if (data_storage == 1)
		{
			pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
			imwrite(pic_name,frame_2);
			counter++;
		}
		else
		{
			imencode(".jpg",frame_2,buff,param);
			allFrames.push_back(buff);
		}
        for(int i=0; i<frame_skip; i++)
		{
            if(!movie.read(frame))
			{
				break;
            }
			motion.push_back(1);
			if (data_storage == 1)
			{
				pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg");
				imwrite(pic_name,frame);
				counter++;
			}
			else
			{
				imencode(".jpg",frame,buff,param);
				allFrames.push_back(buff);
			}
        }
        movie >> frame_1;
		motion.push_back(1);
		if (data_storage == 1)
		{
			pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg");
			imwrite(pic_name,frame_1);
			counter++;
		}
		else
		{
			imencode(".jpg",frame_1,buff,param);
			allFrames.push_back(buff);
		}
        for(int i=0; i<frame_skip; i++)
		{
            if(!movie.read(frame))
			{
				break;
            }
			motion.push_back(1);
			if (data_storage == 1)
			{
				pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
				imwrite(pic_name,frame);
				counter++;
			}
			else
			{
				imencode(".jpg",frame,buff,param);
				allFrames.push_back(buff);
			}
        }
        movie >> frame_0;
		motion.push_back(1);
		if (data_storage == 1)
		{
			pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
			imwrite(pic_name,frame_0);
			counter++;
		}
		else
		{
			imencode(".jpg",frame_0,buff,param);
			allFrames.push_back(buff);
		}
        for(int i=0; i<frame_skip; i++)
		{
			if(!movie.read(frame))
			{
				break;
			}
			motion.push_back(1);
			if (data_storage == 1)
			{
				pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
				imwrite(pic_name,frame);
				counter++;
			}
			else
			{
				imencode(".jpg",frame,buff,param);
				allFrames.push_back(buff);
			}
        }
               
        // kompresja do przestrzeni szarości
        cvtColor(frame_2, frame_2, CV_BGR2GRAY);
        cvtColor(frame_1, frame_1, CV_BGR2GRAY);
        cvtColor(frame_0, frame_0, CV_BGR2GRAY);
 
        // pętla iterująca i przetwarzająca kolejne ramki
        while( true )
		{
 
            // odjęcie bezwględne sąsiadujących ramek, wyznaczenie części wspólnej
            cv::absdiff(frame_2,frame_1,diff_1);
            cv::absdiff(frame_1,frame_0,diff_2);
            cv::bitwise_and(diff_1,diff_2,frame);
 
            // binaryzacja jednoprogowa celem wyróżnienia poruszających się obiektów
            threshold(frame,frame,20,255,0);
            cv::erode(frame,frame,cv::Mat());
            cv::dilate(frame,frame,cv::Mat());
 
            frame_2 = frame_1;
            frame_1 = frame_0;
                       
            // odczyt kolejnej ramki, kompresja do skali szarości
            if(!movie.read(frame_0)){
                    break;
            }
			if (data_storage == 1)
			{
				pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
				imwrite(pic_name,frame_0);
				counter++;
			}
			else
			{
				imencode(".jpg",frame_0,buff,param);
				allFrames.push_back(buff);
			}
            cvtColor(frame_0, frame_0, CV_BGR2GRAY);
            //opcjonalne wyświetlanie
            //cv::imshow("Motion",frame);
            //if(cv::waitKey(1) >= 10) break;
 
            // wyznaczenie pola obiektu
            int pixel_sum = 0;
            int pixel = 0;
            for(int y=0;y<frame.rows;y++){
                    for(int x=0;x<frame.cols;x++){
                            pixel = frame.at<uchar>(y,x);
                                    pixel_sum = pixel_sum + pixel;
                    }
            }
            // jeśli obiekt jest większy niż zadane minimum to ruch występuje, jeśli nie - potencjalne zakłócenia i szumy
            if( pixel_sum > requested_area*area/3 ){
                    flag = 1;
            } else {
                    flag = 0;
            }
            motion.push_back(flag);
            // pominięcie zadanej liczby ramek celem przyspieszenia obliczeń kosztem precyzji
            for(int i=0; i<frame_skip; i++)
			{
				if(!movie.read(frame))
				{
					break;
                }
                motion.push_back(flag);
				if (data_storage == 1)
				{
					pic_name = std::string("img") + std::to_string((long double)counter) + std::string(".jpg"); 
					imwrite(pic_name,frame);
					counter++;
				}
				else
				{
					imencode(".jpg",frame,buff,param);
					allFrames.push_back(buff);
				}
            }
        }
    }

	//Koniec metod

	for(int i=0; i<offset; i++)
	{
		motion.push_back(0);
	}


	motion_processing(motion,offset,befo_motion,past_motion,ones_size,zeros_size);

	// TRYB MANUALNY
	//cv::namedWindow("Motion");
	//manualMode(allFrames, motion, movie, width, height, data_storage, counter);

	//cv::destroyWindow("Motion");
}
 

// Funkcje do manuala

void moveOneFrameEarlier(int &curFrame, std::deque<std::vector<uchar>> allFrames, int data_storage)
{
	cv::Mat picToShow;
	if (curFrame - 1 > 0)
		curFrame = curFrame - 1;
	else
		curFrame = 0;

	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void moveFiveFramesEarlier(int &curFrame, std::deque<std::vector<uchar>> allFrames, int data_storage)
{
	cv::Mat picToShow;
	if (curFrame - 5 > 0)
		curFrame = curFrame - 5;
	else
		curFrame = 0;

	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void moveOneFrameLater(int &curFrame, std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, int data_storage)
{
	cv::Mat picToShow;
	if (curFrame + 1 < motion.size())
		curFrame = curFrame + 1;
	else
		curFrame = motion.size();

	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void moveFiveFramesLater(int &curFrame, std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, int data_storage)
{
	cv::Mat picToShow;
	if (curFrame + 5 < motion.size())
		curFrame = curFrame + 5;
	else
		curFrame = 0;
	
	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void stopVideo(bool &stop)
{
	stop = true;
}

void exitFromManualMode(bool &esc)
{
	esc = true;
}

void setMovieTime(std::deque<int> motion, std::deque<std::vector<uchar>> allFrames, double &momentFilmu, int &curFrame, int data_storage)
{
	cv::Mat picToShow;
	std::cout << "Wprowadz moment filmu" << std::endl;
	std::cin >> momentFilmu;
	curFrame = (int)(momentFilmu*motion.size());
	
	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

cv::Mat playVideo(std::deque<int> motion, std::deque<std::vector<uchar>> allFrames, bool &stop, bool &esc, int &curFrame, double &momentFilmu, char &k, int mSec, int data_storage)
{
cv::Mat picToShow;
//	for (int i=curFrame; i<motion.size(); i++)
//	{

		if (data_storage == 1)
		{
			std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
			picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
		}
		else
		{
			picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
		}

//		cv::imshow("Motion",picToShow);

        curFrame=curFrame + 1;

        //QImage qim((uchar*)picToShow.data, picToShow.cols, picToShow.rows, picToShow.step, QImage::Format_RGB888);
//		stop = false;
//		k = cv::waitKey(mSec);
//		if (k=='s') // stop odtwarzania
//		{
//			stopVideo(stop);
//		}
//		else if ((int)k==27) //ESC - wyjście
//		{
//			exitFromManualMode(esc);
//		}
//		else if (k=='x') //Wybór momentu filmu - docelowo suwaczek - teraz double [0;1]
//		{
//			setMovieTime(motion, allFrames, momentFilmu, curFrame, data_storage);
//		}

        //i = curFrame;
//        std::cout << curFrame << std::endl;
//		if (esc == true || stop == true)
//			break;
//	}

        return picToShow;
}

void setBegining(int &begin, int curFrame)
{
	begin = curFrame;
}

void setEnd(int &end, int curFrame)
{
	end = curFrame;
}

void moveToBegining(int &curFrame, int begin, std::deque<std::vector<uchar>> allFrames, int data_storage)
{
	cv::Mat picToShow;
	curFrame = begin;
	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void moveToEnd(int &curFrame, int end, std::deque<std::vector<uchar>> allFrames, int data_storage)
{
	cv::Mat picToShow;
	curFrame = end;
	if (data_storage == 1)
	{
		std::string pic_name = std::string("img") + std::to_string((long double)curFrame) + std::string(".jpg");
		picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		picToShow = cv::imdecode(cv::Mat(allFrames[curFrame]),CV_LOAD_IMAGE_COLOR);
	}

	cv::imshow("Motion",picToShow);
}

void takeNextFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame)
{
	if (curFragment + 1 != fragmentList.size())
	{
		curFragment = curFragment + 1;
		begin = fragmentList[curFragment].begin;
		end = fragmentList[curFragment].end;
		curFrame = begin;
	}
	std::cout << fragmentList[curFragment].name << std::endl;
}

void takePreviousFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame)
{
	if (curFragment>0)
	{
		curFragment = curFragment - 1;
		begin = fragmentList[curFragment].begin;
		end = fragmentList[curFragment].end;
		curFrame = begin;
	}
	std::cout << fragmentList[curFragment].name << std::endl;
}

void saveVideoToFile(cv::VideoWriter video, std::vector<movieFragment> fragmentList, int &curFragment, int &curFrame, int &begin, int &end, double fps, int width, int height, std::deque<std::vector<uchar>> allFrames, int data_storage)
{
	cv::Mat picToShow;
	if (data_storage == 1)
		chdir("..");

	video.open((fragmentList[curFragment].name),CV_FOURCC('M','P','E','G'),fps, cv::Size(width,height),true);
	for (int j=begin; j<=end; j++)
	{
		if (data_storage == 1)
		{
			chdir("images");
			std::string pic_name = std::string("img") + std::to_string((long double)j) + std::string(".jpg");
			picToShow = cv::imread(pic_name, CV_LOAD_IMAGE_COLOR);
			chdir("..");
		}
		else
		{
			picToShow = cv::imdecode(cv::Mat(allFrames[j]),CV_LOAD_IMAGE_COLOR);
		}
		video.write(picToShow);
	}
	video.release();

	// Przeskoczenie na następny fragment (jeśli jest) i ustawienie aktualnych informacji dotyczących fragmentu
	if (curFragment + 1 != fragmentList.size())
	{
		curFragment = curFragment + 1;
		begin = fragmentList[curFragment].begin;
		end = fragmentList[curFragment].end;
		curFrame = begin;
	}
	std::cout << fragmentList[curFragment].name << std::endl;
	if (data_storage == 1)
		chdir("images");
}

void setBeginFromFrameNo(int &begin, int frameNo, std::deque<int> motion)
{
	if (frameNo < motion.size())
		begin = frameNo;
	else
		begin = motion.size()-1;
}

void setEndFromFrameNo(int &end, int frameNo, std::deque<int> motion)
{
	if (frameNo < motion.size())
		end = frameNo;
	else
		end = motion.size()-1;
}

/*void manualMode(std::deque<std::vector<uchar>> allFrames, std::deque<int> motion, cv::VideoCapture &movie, int width, int height, int data_storage, int counter)
{
	
}*/
