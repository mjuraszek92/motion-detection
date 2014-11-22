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
#define NOMINMAX
#include <Windows.h>
#include <fstream>


struct movieFragment {
	int begin;
	int end;
	std::string name;
};

void moveOneFrameEarlier(int &curFrame, std::deque<cv::Mat> allFrames);
void moveFiveFramesEarlier(int &curFrame, std::deque<cv::Mat> allFrames);
void moveOneFrameLater(int &curFrame, std::deque<cv::Mat> allFrames, std::deque<int> motion);
void moveFiveFramesLater(int &curFrame, std::deque<cv::Mat> allFrames, std::deque<int> motion);
void stopVideo(bool &esc);
void exitFromManualMode(bool &stop);
void setMovieTime(std::deque<int> motion, std::deque<cv::Mat> allFrames, double &momentFilmu, int &curFrame);
void playVideo(std::deque<int> motion, std::deque<cv::Mat> allFrames, bool &stop, bool &esc, int &curFrame, double &momentFilmu, char &k, int mSec);
void setBegining(int &begin, int curFrame);
void setEnd(int &end, int curFrame);
void moveToBegining(int &curFrame, int begin, std::deque<cv::Mat> allFrames);
void moveToEnd(int &curFrame, int end, std::deque<cv::Mat> allFrames);
void takeNextFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame); 
void takePreviousFragment(int &curFragment, int &begin, int &end, std::vector<movieFragment> fragmentList, int &curFrame); 
void saveVideoToFile(cv::VideoWriter video, std::vector<movieFragment> fragmentList, int &curFragment, int &curFrame, int &begin, int &end, double fps, int width, int height, std::deque<cv::Mat> allFrames);
void manualMode(std::deque<cv::Mat> allFrames, std::deque<int> motion, cv::VideoCapture &movie, int width, int height);

 
// funkcja przetwarzająca wektor ruchu dostarczony przez algorytm detekcji zgodnie z parametrami użytkownika (celem usunięcia szumu, zakłóceń, łączenia framgnetów)
void motion_processing(std::deque<int> &motion, int offset, int befo_motion, int past_motion, int ones_size, int zeros_size){
        int median_sum;
        unsigned int big_counter = offset;
        int small_counter = 0;
        int break_flag = 0;
 
        // Filtracja medianowa wektora ruchu
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
 
        // Pętla usuwająca ruch trwający krócej niż zadana długość (prawdopodobny szum, zakłócenia)
        while( true ){
                small_counter = 0;
 
                while( motion[big_counter] == 0 ){
                        ++big_counter;
                        if( big_counter > motion.size()-offset-1 ){
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
 
        // Pętla scalająca fragmenty ruchu przedzielone niewielkimi (mniejszymi niż zadane) przerwami (zakłócenia)
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
                        if( big_counter > motion.size() - offset - 1 ){
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
 
        // Usunięcie dodatkowego konteksu wykorzystywanego przez filtracje medianową
        for(int i=0; i<offset; i++){
                motion.pop_front();
                motion.pop_back();
        }
       
        // Pętla umożliwiająca zapisanie dodatkowych N ramek przed wystąpieniem ruchu oraz M ramek po jego wystąpnieniu
        big_counter = 0;
        small_counter = 0;
        break_flag = 0;
        while( true ){
                while( motion[big_counter] == 0 ){
                        ++big_counter;
                        if( big_counter >= motion.size() ){
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
                }
 
                while( motion[big_counter] == 1 ){
                        ++big_counter;
                        if( big_counter >= motion.size() ){
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
                        if( big_counter >= motion.size() ){
                                break_flag = 1;
                                break;
                        }
                }
 
                if( break_flag == 1 ){
                        break;
                }
        }
}

int find_char(std::string path, char c){
        int position = 0;
        for(int i=path.length()-1; i>=0; i--){
                if( path[i] == c ){
                        position = i;
                        break;
                }
        }
        return position+1;
}
 
// funkcja wyznaczjąca nazwę pliku bez rozszerzenia z pełnej ścieżki
std::string find_filename(std::string path){
        std::string file_name = path.substr(find_char(path,'/'),path.length());
        file_name = file_name.substr(0,find_char(file_name,'.')-1);
        return file_name;
}

void save_motion(std::deque<int> &motion, cv::VideoCapture &movie, std::string path){
        int movies_count = 0;
        int break_flag = 0;
        unsigned int index = 0;
        double fps = movie.get(CV_CAP_PROP_FPS);
    int frame_width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
    std::string video_name;
        std::string directory_path;
        cv::VideoWriter video;
        cv::Mat frame;
        std::fstream log_file;
 
        directory_path = path.substr(0,find_char(path,'/')) + find_filename(path);
        CreateDirectory(directory_path.c_str(),NULL); // tworzenie katalogu odpowiadającemu nazwie przetwarzanego pliku wideo
 
        log_file.open(directory_path + std::string("//") + std::string("log_file.txt"), std::ios::out ); //tworzenie pliku, do którego zapisane zostaną logi z pracy algorytmu
       
        // pętla iterująca po filmie i wektorze ruchu, umożliwiająca zapis odpowiednich ramek
        movie.set(CV_CAP_PROP_POS_AVI_RATIO,0);
        while( true ){
                ++movies_count;
 
                // pomijanie ramek nie zawierających ruchu
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
 
                // otwarcie nowego pliku wideo, zapis informacji do pliku z logami
                video_name = std::string("video_") + std::to_string((long double)movies_count) + std::string(".avi");
                video.open(directory_path + std::string("//") + video_name,CV_FOURCC('M','P','E','G'),fps, cv::Size(frame_width,frame_height),true);
                log_file << "No.: " << movies_count << "   Video Name: " << video_name << "   Motion Start Sec (Frame): " << std::floor(index/fps) << " (" << index << ")" << "   Motion End Sec (Frame): ";
               
                // zapisywanie ramek zawierających ruch
                while( motion[index] == 1 ){
                        movie >> frame;
                        ++index;
                        video.write(frame);
                        if( index == motion.size() ){
                                break_flag = 1;
                                break;
                        }
                }
               
                //zapis dodatkowych informacji do pliku z logami, zamknięcie utworzonego pliku wideo
                log_file << std::floor(index/fps) << " (" << index << ")" << "\n";
                video.release();
               
                if( break_flag){
                        break;
                }
        }
        log_file.close();
}
 
void motion_detection(std::string path, std::map<std::string,double> parameters){
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
 
    cv::Mat frame;
	cv::Mat fore;
    cv::VideoCapture movie;
    cv::Mat element = cv::getStructuringElement(0, cv::Size(5,5));
	cv::BackgroundSubtractorMOG2 bg;
       
    // ustawienie dodatkowych parametrów metody
    bg.set("detectShadows", true); // rozróżnianie obiektów i cieni
    bg.set("nShadowDetection", 0); // ignorowanie cieni
    bg.set("history",history); // ilość ramek z jakich wyznaczany jest model tła
    bg.set("nmixtures",nmixtures); // ilość mieszanin gaussowskich
 
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> tmp;
 
    movie.open(path);
	
	movie.set(CV_CAP_PROP_POS_AVI_RATIO,0);
	std::deque<cv::Mat> allFrames;

    int height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
    int width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
    int area = height*width;
    int flag = 0;
    int perimeter = 100;
 
    // główny wektor ruchu
    std::deque<int> motion;
	
    // tworzenie kontekstu dla filtracji medianowej
    for(int i=0; i<offset; i++){
            motion.push_back(0);
    }
 
    cv::namedWindow("Motion");
    if( method == 1 ){ // metoda mieszanin gaussowskich
            // pętla odczytująca i przetwarzająca kolejne ramki
            while( true ){
                    if(!movie.read(frame)){
                            break;
                    }
					allFrames.push_back(frame);
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
                    cv::drawContours(frame,tmp,-1,cv::Scalar(0,0,255),2);
                    cv::imshow("Motion",frame);
                    if(cv::waitKey(1) >= 5) break;
 
                    motion.push_back(flag);
                    tmp.clear();
                       
                    // opuszczanie zadanej liczby kolejnych ramek (celem przyspieszenia obliczeń kosztem precyzji)
                    for(int i=0; i<frame_skip; i++){
                            if(!movie.read(frame)){
                                    break;
                            }
                            motion.push_back(flag);
							allFrames.push_back(frame);
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
		allFrames.push_back(frame_2);
        for(int i=0; i<frame_skip; i++)
		{
            if(!movie.read(frame))
			{
				break;
            }
			motion.push_back(1);
			allFrames.push_back(frame);
        }
        movie >> frame_1;
		motion.push_back(0);
		allFrames.push_back(frame_1);
        for(int i=0; i<frame_skip; i++)
		{
            if(!movie.read(frame))
			{
				break;
            }
			motion.push_back(0);
			allFrames.push_back(frame);
        }
        movie >> frame_0;
		motion.push_back(1);
		allFrames.push_back(frame_0);
        for(int i=0; i<frame_skip; i++)
		{
			if(!movie.read(frame))
			{
				break;
			}
			motion.push_back(1);
			allFrames.push_back(frame);
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
			allFrames.push_back(frame_0);
            cvtColor(frame_0, frame_0, CV_BGR2GRAY);
            //opcjonalne wyświetlanie
            cv::imshow("Motion",frame);
            if(cv::waitKey(1) >= 10) break;
 
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
            if( pixel_sum > requested_area*area/2 ){
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
				allFrames.push_back(frame);
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
	manualMode(allFrames, motion, movie, width, height);
	cv::destroyWindow("Motion");
}
 
int main(int argc, char *argv[])
{
    std::map<std::string,double> parametry;
    parametry["frame_skip"] = 1;
    parametry["zeros_size"] = 10;
    parametry["ones_size"] = 10;
    parametry["befo_motion"] = 5;
    parametry["past_motion"] = 5;
    parametry["area"] = 0.0005;
    parametry["history"] = 100;
    parametry["nmixtures"] = 3;
    parametry["method"] = 0;
    parametry["thread"] = 1;
 
    std::string path = "C://Users//Mirek-ja//Desktop//Test//MOV_0013.mp4";
 
    motion_detection(path,parametry);
    system("pause");
    return 0;
}


// Funkcje do manuala

void moveOneFrameEarlier(int &curFrame, std::deque<cv::Mat> allFrames)
{
	if (curFrame - 1 > 0)
		curFrame = curFrame - 1;
	else
		curFrame = 0;
	cv::imshow("Motion",allFrames[curFrame]);
}

void moveFiveFramesEarlier(int &curFrame, std::deque<cv::Mat> allFrames)
{
	if (curFrame - 5 > 0)
		curFrame = curFrame - 5;
	else
		curFrame = 0;
	cv::imshow("Motion",allFrames[curFrame]);
}

void moveOneFrameLater(int &curFrame, std::deque<cv::Mat> allFrames, std::deque<int> motion)
{
	if (curFrame + 1 < motion.size())
		curFrame = curFrame + 1;
	else
		curFrame = motion.size();
	cv::imshow("Motion",allFrames[curFrame]);
}

void moveFiveFramesLater(int &curFrame, std::deque<cv::Mat> allFrames, std::deque<int> motion)
{
	if (curFrame + 5 < motion.size())
		curFrame = curFrame + 5;
	else
		curFrame = 0;
	cv::imshow("Motion",allFrames[curFrame]);
}

void stopVideo(bool &stop)
{
	stop = true;
}

void exitFromManualMode(bool &esc)
{
	esc = true;
}

void setMovieTime(std::deque<int> motion, std::deque<cv::Mat> allFrames, double &momentFilmu, int &curFrame)
{
	std::cout << "Wprowadz moment filmu" << std::endl;
	std::cin >> momentFilmu;
	curFrame = (int)(momentFilmu*motion.size());
	cv::imshow("Motion",allFrames[curFrame]);
}

void playVideo(std::deque<int> motion, std::deque<cv::Mat> allFrames, bool &stop, bool &esc, int &curFrame, double &momentFilmu, char &k, int mSec)
{
	for (int i=curFrame; i<motion.size(); i++)
	{
		cv::imshow("Motion",allFrames[i]);
		curFrame=i;
		stop = false;
		k = cv::waitKey(mSec);
		if (k=='s') // stop odtwarzania
		{
			stopVideo(stop);
		}
		else if (k=='b') // -1 ramka
		{
			moveOneFrameEarlier(curFrame, allFrames);
		}
		else if (k=='v') // -5 ramek
		{
			moveFiveFramesEarlier(curFrame, allFrames);
		}
		else if (k=='n') // +1 ramka
		{
			moveOneFrameLater(curFrame, allFrames, motion);
		}
		else if (k=='m') // +5 ramek
		{
			moveFiveFramesLater(curFrame, allFrames, motion);
		}
		else if ((int)k==27) //ESC - wyjście
		{
			exitFromManualMode(esc);
		}
		else if (k=='x') //Wybór momentu filmu - docelowo suwaczek - teraz double [0;1]
		{
			setMovieTime(motion, allFrames, momentFilmu, curFrame);
		}

		i = curFrame;
		std::cout << curFrame << std::endl; 
		if (esc == true || stop == true)
			break;
	}
}

void setBegining(int &begin, int curFrame)
{
	begin = curFrame;
}

void setEnd(int &end, int curFrame)
{
	end = curFrame;
}

void moveToBegining(int &curFrame, int begin, std::deque<cv::Mat> allFrames)
{
	curFrame = begin;
	cv::imshow("Motion",allFrames[curFrame]);
}

void moveToEnd(int &curFrame, int end, std::deque<cv::Mat> allFrames)
{
	curFrame = end;
	cv::imshow("Motion",allFrames[curFrame]);
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

void saveVideoToFile(cv::VideoWriter video, std::vector<movieFragment> fragmentList, int &curFragment, int &curFrame, int &begin, int &end, double fps, int width, int height, std::deque<cv::Mat> allFrames)
{
	video.open((fragmentList[curFragment].name),CV_FOURCC('M','P','E','G'),fps, cv::Size(width,height),true);
	for (int j=begin; j<=end; j++)
	{
		video.write(allFrames[j]);
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
}

void manualMode(std::deque<cv::Mat> allFrames, std::deque<int> motion, cv::VideoCapture &movie, int width, int height)
{
	std::cout << "motion: " << motion.size() << std::endl;
	std::cout << "allFrames: " << allFrames.size() << std::endl;
	std::vector<movieFragment> fragmentList; 
	int movies_count = 0;
	int break_flag = 0;
	unsigned int index = 0;
    std::string video_name;
    cv::VideoWriter video;

	// Dodawanie do listy informacji na temat wszystkich fragmentów z ruchem (początek, koniec, nazwa filmu)

	while( true )
	{
		movieFragment movieData;
		++movies_count;
		while( motion[index] == 0 )
		{
			++index;
			if( index == motion.size() )
			{
				break_flag = 1;
				break;
			}
		}
        if( break_flag )
		{
			break;
		}
		
		movieData.begin = index;
		video_name = std::string("video_") + std::to_string((long double)movies_count) + std::string(".avi");
		movieData.name = video_name;
 
		while( motion[index] == 1 )
		{
			++index;
			if( index == motion.size() )
			{
				break_flag = 1;
				break;
			}
		}
		movieData.end = index - 1;
		if( break_flag)
		{
			break;
		}
		fragmentList.push_back(movieData);
		 
	}



	double fps = movie.get(CV_CAP_PROP_FPS);
	char k;
	int mSec = (int)(1000/fps);
	int curFragment = 0;
	int curFrame = fragmentList[0].begin;
	int begin = fragmentList[0].begin;
	int end = fragmentList[0].end;
	bool stop = false;
	bool esc = false;

	// Menu w postaci "getchara" -> docelowo będa to buttony, suwaczek, i text fieldy w GUI

	std::cout << "p - play, v - 5 ramek do tylu, b - 1 ramka do tylu, n - 1 ramka do przodu" << std::endl;
	std::cout << "m - 5 ramek do przodu, x - ustaw moment filmu, ESC - wyjscie" << std::endl;
	std::cout << "w - zapisz początek filmu, e - zapisz koniec filmu" << std::endl;
	std::cout << "y - poprzedni fragment, u - następny fragment, i - zapisz film" << std::endl;
	std::cout << "r - przesun na poczatek fragmentu, t - przesun na koniec fragmentu" << std::endl;
	std::cout << std::endl;
	double momentFilmu;


	while (true)
	{
		k = cv::waitKey(10000);
		
		if (k=='p')
		{
			playVideo(motion, allFrames, stop, esc, curFrame, momentFilmu, k, mSec);
			
		}
		
		////
		else if (k=='b') // -1 ramka
		{
			moveOneFrameEarlier(curFrame, allFrames);
		}
		
		
		else if (k=='v') // -5 ramek
		{
			moveFiveFramesEarlier(curFrame, allFrames);
		}
		
		
		else if (k=='n') // + 1 ramka
		{
			moveOneFrameLater(curFrame, allFrames, motion);
		}
		
		
		else if (k=='m') // + ramek
		{
			moveFiveFramesLater(curFrame, allFrames, motion);
		}
		
		
		else if (k=='w') // ustaw aktualną ramkę jako początek fragmentu
		{
			setBegining(begin, curFrame);
		}


		else if (k=='e') // ustaw aktualną ramkę jako koniec fragmentu
		{
			setEnd(end, curFrame);
		}


		else if (k=='r') // przesuń na początek fragmentu
		{
			moveToBegining(curFrame, begin, allFrames);
		}


		else if (k=='t') // przesuń na koniec fragmentu
		{
			moveToEnd(curFrame, end, allFrames);
		}


		else if (k=='y') // weź poprzedni fragment
		{
			takePreviousFragment(curFragment, begin, end, fragmentList, curFrame); 
		}


		else if (k=='u') // weź następny fragment
		{
			takeNextFragment(curFragment, begin, end, fragmentList, curFrame);
		}


		else if (k=='x') // Wybór momentu filmu - docelowo suwaczek - teraz double [0;1]
		{
			setMovieTime(motion, allFrames, momentFilmu, curFrame);
		}

		else if (k=='i') // zapis fragmentu filmu do pliku
		{
			saveVideoToFile(video, fragmentList, curFragment, curFrame, begin, end, fps, width, height, allFrames);
		}

		else if ((int)k==27) //ESC - wyjście
		{
			exitFromManualMode(esc);
		}

		std::cout << curFrame << std::endl; // wyświetlanie aktualnej ramki po każdej akcji - dobre do sprawdzenia poprawności :)
		if (esc == true)
			break;
	
	}

	std::cout << std::endl;
	for (int i=0; i<fragmentList.size(); i++)
		std::cout << fragmentList[i].begin << " " << fragmentList[i].end << " " << fragmentList[i].name << std::endl; // wypisywanie info nt. fragmentów



}
