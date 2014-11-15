/**
 * Imports
 */
#include <iostream>
#include <map>
#include <thread>
#include "./motion.h"

/**
 * Program
 */
void motion_detection(std::string path, std::map<std::string,double> parameters) {
	std::cout << "Here is execution of some powerful image processing algorithm!" << std::endl;
}

void motion_detection_distributed(int cnt, std::string* path, std::map<std::string,double>* parameters) {
	std::thread threads[cnt];
	int i;

	// dispatch threads
	for (i = 0; i < cnt; ++i) {
		threads[i] = std::thread(motion_detection, path[i], parameters[i]);
	}

	// w8 for execution of all threads finishes
	for (i = 0; i < cnt; ++i) {
		threads[i].join();
	}
}