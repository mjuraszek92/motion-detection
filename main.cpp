/**
 * imports
 */
#include "./lib/motion/motion.cpp"
#include <stdio.h>

/**
 * program
 */
int main(int argc, char* argv[]) {
	std::cout << "Starting program..." << std::endl; 

	std::string* paths;
	std::map<std::string,double>* parameters;
	int cnt;
	int i;

	cnt			= 3;
	paths 		= new std::string[cnt];
	parameters 	= new std::map<std::string,double>[cnt];

	for (i = 0; i < cnt; ++i) {
		paths[i]		= "path";
		parameters[i]	= *(new std::map<std::string,double>());
	}

	motion_detection_distributed(cnt, paths, parameters);

	delete [] paths;
	delete [] parameters;

	std::cout << "Bye!" << std::endl;

	return 0;
}