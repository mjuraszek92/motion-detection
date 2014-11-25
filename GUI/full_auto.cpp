#include <opencv2\opencv.hpp>
#include <vector>
#include <deque>
#include <map>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <fstream>
#include <ctime>

// funkcja przetwarzająca wektor ruchu dostarczony przez algorytm detekcji zgodnie z parametrami użytkownika (celem usunięcia szumu, zakłóceń, łączenia framgnetów)
void motion_processing(std::deque<int> &motion, int offset, int befo_motion, int past_motion, int ones_size, int zeros_size){
    int median_sum;
    int big_counter = offset;
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

// funkcja odnajdująca pozycję znaku w napisie
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


std::wstring s2wsa(const std::string& s) {
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

std::string ws2sa(const std::wstring& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
}


// funkcja dokonująca zapisu fragmentów filmu zawierających ruch w oparciu o wektor ruchu
void save_motion_full_auto(std::deque<int> &motion, cv::VideoCapture &movie, std::string path, double fps, int start, int thread_no){
	int movies_count = 0;
	int current_frame = 0;
	int break_flag = 0;
	int index = 0;
	int frame_width = (int)movie.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)movie.get(CV_CAP_PROP_FRAME_HEIGHT);
	std::string video_name;
	std::string directory_path;
	cv::VideoWriter video;
	cv::Mat frame;
	std::fstream log_file;
	std::string temp_dir_path;
	int N = 1;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string date = std::string("_") + std::to_string((long double)ltm->tm_mday) + std::to_string((long double)ltm->tm_mon+1) + std::to_string((long double)ltm->tm_year+1900);
	// wyznaczanie ścieżki folderu docelowego i jego tworzenie
	directory_path = path.substr(0,find_char(path,'/')) + find_filename(path) + std::string("_auto") + date;
	temp_dir_path = directory_path;
    //std::wstring wdir = s2ws(directory_path);
    std::wstring wtemp = s2wsa(temp_dir_path);

    // tworzenie katalogu odpowiadającemu nazwie przetwarzanego pliku wideo
    while(CreateDirectory(wtemp.c_str(),NULL) < 1){
        temp_dir_path = directory_path + std::string("_") + std::to_string((long double)N);
        N++;
    }

	// tworzenie pliku logfile
	log_file.open(directory_path + std::string("//") + std::string("log_file.txt"), std::ios::out );
	
	movie.set(CV_CAP_PROP_POS_AVI_RATIO,0);

	// iteracja po wektorze ruchu i filmie celem zapisu wybranych ramek (zawierających ruch)
	while( true ){
		++movies_count;

		// pomijanie ramek nie zawierających ruchu (flaga ustawiona na zero)
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
		video.open(directory_path + std::string("//") + video_name,CV_FOURCC('F','M','P','4'),fps, cv::Size(frame_width,frame_height),true);

		// zapis do pliku logfile informacji o bieżącym fragmencie
		log_file << "No.: " << movies_count << "   Video Name: " << video_name << "   Motion Start Sec (Frame): " << std::floor((float)index/fps) << " (" << (index) << ")" << "   Motion End Sec (Frame): ";
		
		// zapisywanie ramek zawierających ruch (flaga ustawiona na jeden)
		while( motion[index] == 1 ){
			movie >> frame;
			++index;
			video.write(frame);
			if( index == motion.size() ){
				break_flag = 1;
				break;
			}
		}
		
		// zapis dodatkowych informacji do pliku z logami, zamknięcie utworzonego pliku wideo
		log_file << std::floor((float)index/fps) << " (" << (index) << ")" << "\n";
		video.release();
		
		if( break_flag){
			break;
		}
	}
	// zamknięcie pliku logfile
	log_file.close();
}

// funkcja detekcji ruchu w materiale wideo, implementuje metody "Adaptive Gaussian Mixture Background Model" oraz odejmowanie kolejnych ramek
void motion_detection_full_auto(std::string path, std::map<std::string,double> parameters){
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
	int thread_no = parameters["thread_no"];
	int threads = parameters["threads"];

	cv::Mat frame;
    cv::Mat fore;
	cv::VideoCapture movie;
    cv::BackgroundSubtractorMOG2 bg;
	
	// ustawienie dodatkowych parametrów metody mieszanin gaussa (domyślnej)
	bg.set("detectShadows", true); // rozróżnianie obiektów i cieni
	bg.set("nShadowDetection", 0); // ignorowanie cieni
	bg.set("history",history); // ilość ramek z jakich wyznaczany jest model tła
	bg.set("nmixtures",nmixtures); // ilość mieszanin gaussowskich

	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> tmp;

	movie.open(path);
	int height = movie.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = movie.get(CV_CAP_PROP_FRAME_WIDTH);
	int area = height*width;
	int flag = 0;
	int perimeter = 100;
	int start = 0;
	int end = 0;
	int frames_no = 0;
	int current_frame = 0;
	
	// główny wektor ruchu
	std::deque<int> motion;
	
	// tworzenie kontekstu dla filtracji medianowej
	for(int i=0; i<offset; i++){
		motion.push_back(0);
	}

	if( method == 1 ){ // metoda mieszanin gaussowskich
		// iteracja po kolejnych ramkach
		while( true ){
			current_frame++;
			// odczyt kolejnej ramki
			if(!movie.read(frame)){
				break;
			}
			// funkcja wyznaczjąca model tła
			bg.operator()(frame,fore);
			// eliminacja drobnych zakłóceń
			cv::erode(fore,fore,cv::Mat());
			cv::dilate(fore,fore,cv::Mat());
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
			// ustawienie odpowieniej flagi dla analizowanej ramki
			motion.push_back(flag);
			tmp.clear();
			
			// opuszczanie zadanej liczby kolejnych ramek (celem przyspieszenia obliczeń kosztem precyzji)
			for(int i=0; i<frame_skip; i++){
				if(!movie.read(frame)){
					break;
				}
				motion.push_back(flag);
				current_frame++;
			}
		}
	} else { // metoda odejmowania ramek
		int deq_index = 1;
		cv::Mat frame_2;
		cv::Mat frame_1;
		cv::Mat frame_0;
		cv::Mat diff_1;
		cv::Mat diff_2;

		// odczyt pierwszych 3 ramek (z ewentualnym pominięciem zadanej ilości pomiędzy (celem przyspieszenia obliczeń kosztem precyzji))
		movie >> frame_2;
		current_frame++;
		motion.push_back(1);
		for(int i=0; i<frame_skip; i++){
			if(!movie.read(frame)){
				break;
			}
			motion.push_back(1);
			current_frame++;
		}
		movie >> frame_1;
		current_frame++;
		motion.push_back(0);
		for(int i=0; i<frame_skip; i++){
			if(!movie.read(frame)){
				break;
			}
			motion.push_back(0);
			current_frame++;
		}
		movie >> frame_0;
		current_frame++;
		motion.push_back(1);
		for(int i=0; i<frame_skip; i++){
			if(!movie.read(frame)){
				break;
			}
			motion.push_back(1);
			current_frame++;
		}

		// kompresja do przestrzeni szarości
		cvtColor(frame_2, frame_2, CV_BGR2GRAY);
		cvtColor(frame_1, frame_1, CV_BGR2GRAY);
		cvtColor(frame_0, frame_0, CV_BGR2GRAY);

		// iteracja po kolejnych ramkach wideo
		while( true ){
			// odjęcie bezwględne sąsiadujących ramek, wyznaczenie części wspólnej (usuwa szumy i zakłócenia)
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
			current_frame++;
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
			for(int i=0; i<frame_skip; i++){
				if(!movie.read(frame)){
					break;
				}
				motion.push_back(flag);
				current_frame++;
			}
		}
	}

	// dodanie kontekstu filtarcji medianowej
	for(int i=0; i<offset; i++){
		motion.push_back(0);
	}
	std::cout<<"Detection completed\n";
	//przekazanie wektora ruchu do funkcji przetwarzającej i zapisującej
	motion_processing(motion,offset,befo_motion,past_motion,ones_size,zeros_size);
	std::cout<<"Processing completed\n";
	// wyznaczanie prawidłowej liczby fps
	double fps = movie.get(CV_CAP_PROP_FPS);
	if((movie.get(CV_CAP_PROP_FRAME_COUNT)/current_frame) > 1.2 ){
		fps = fps/2;
	}
	// przekazanie wektora ruchu i filmu do funkcji zapisującej
	save_motion_full_auto(motion,movie,path,fps,start,thread_no);
	std::cout<<"Saving completed\n";

	//cv::destroyWindow("Motion");
	//system("pause");
}


