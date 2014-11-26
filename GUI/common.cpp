//Copyright (c) 2014, ZPI_PT_10_45
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//1. Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//3. All advertising materials mentioning features or use of this software
//   must display the following acknowledgement:
//   This product includes software developed by the ZPI_PT_10_45.
//4. Neither the name of the ZPI_PT_10_45 nor the
//   names of its contributors may be used to endorse or promote products
//   derived from this software without specific prior written permission.

//THIS SOFTWARE IS PROVIDED BY ZPI_PT_10_45 ''AS IS'' AND ANY
//EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL ZPI_PT_10_45 BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <deque>

void medianFilter(std::deque<int> &motion, int offset);
void removeNoise(std::deque<int> &motion, int offset, int ones_size);
void mergeMotions(std::deque<int> &motion, int offset, int zeros_size);
void deleteMedianFilterContext(std::deque<int> &motion, int offset);
void saveAdditionalFrames(std::deque<int> &motion, int beforeMotion, int pastMotion);

// funkcja przetwarzająca wektor ruchu dostarczony przez algorytm detekcji zgodnie z parametrami użytkownika (celem usunięcia szumu, zakłóceń, łączenia framgnetów)
void motion_processing(std::deque<int> &motion, int offset, int beforeMotion, int pastMotion, int onesSize, int zerosSize) {
	// Filtracja medianowa wektora ruchu
	medianFilter(motion, offset);
	// Pętla usuwająca ruch trwający krócej niż zadana długość (prawdopodobny szum, zakłócenia)
	removeNoise(motion, offset, onesSize);
	// Pętla scalająca fragmenty ruchu przedzielone niewielkimi (mniejszymi niż zadane) przerwami (zakłócenia)
	mergeMotions(motion, offset, zerosSize);
	// Usunięcie dodatkowego konteksu wykorzystywanego przez filtracje medianową
	deleteMedianFilterContext(motion, offset);
	// Pętla umożliwiająca zapisanie dodatkowych N ramek przed wystąpieniem ruchu oraz M ramek po jego wystąpnieniu
	saveAdditionalFrames(motion, beforeMotion, pastMotion);
}

void medianFilter(std::deque<int> &motion, int offset) {
	int medianSum;
	for(int i = offset; i < motion.size() - offset - 1; i++) {
		medianSum = 0;
		for (int j = i - offset; j <= i + offset; j++ ) {
			medianSum += motion[j];
		}
		if (medianSum > offset + 0.5) {
			motion[i] = 1;
		} else {
			motion[i] = 0;
		}
	}
}

void removeNoise(std::deque<int> &motion, int offset, int ones_size) {
	int smallCounter;
	int bigCounter = offset;
	int break_flag = 0;

	while (true) {
		smallCounter = 0;
		while (motion[bigCounter] == 0) {
			++bigCounter;
			if (bigCounter > motion.size() - offset - 1) {
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 1) {
			break;
		}
		while (motion[bigCounter] == 1) {
			++smallCounter;
			++bigCounter;
		}
		if (smallCounter < ones_size) {
			while (smallCounter > 0) {
				motion[bigCounter-smallCounter] = 0;
				--smallCounter;
			}
		}
	}
}

void mergeMotions(std::deque<int> &motion, int offset, int zeros_size) {
	int smallCounter;
	int	big_counter = offset;
	int break_flag = 0;
	while (true) {
		smallCounter = 0;
		while (motion[big_counter] == 1) {
			++big_counter;
		}
		while (motion[big_counter] == 0) {
			++smallCounter;
			++big_counter;
			if (big_counter > motion.size() - offset - 1) {
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 1) {
			break;
		}
		if (smallCounter < zeros_size) {
			while (smallCounter > 0) {
				motion[big_counter-smallCounter] = 1;
				--smallCounter;
			}
		}
	}
}

void deleteMedianFilterContext(std::deque<int> &motion, int offset) {
	for(int i = 0; i < offset; i++) {
		motion.pop_front();
		motion.pop_back();
	}
}

void saveAdditionalFrames(std::deque<int> &motion, int beforeMotion, int pastMotion) {
	int bigCounter = 0;
	int smallCounter = 0;
	int break_flag = 0;
	while (true) {
		while (motion[bigCounter] == 0) {
			++bigCounter;
			if (bigCounter >= motion.size()) {
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 1) {
			break;
		}
		smallCounter = 1;
		while ((smallCounter <= beforeMotion) && ((signed int)bigCounter - (signed int)smallCounter > 0)) {
			motion[bigCounter-smallCounter] = 1;
			++smallCounter;
		}
		while (motion[bigCounter] == 1) {
			++bigCounter;
			if (bigCounter >= motion.size()) {
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 1){
			break;
		}
		smallCounter = 0;
		while ((smallCounter < pastMotion) && (bigCounter+smallCounter < motion.size())) {
			motion[bigCounter+smallCounter] = 1;
			++smallCounter;
		}
		while (motion[bigCounter] == 1) {
			++bigCounter;
			if (bigCounter >= motion.size()) {
				break_flag = 1;
				break;
			}
		}
		if (break_flag == 1) {
			break;
		}
	}
}

// funkcja odnajdująca pozycję znaku w napisie
int find_char(std::string path, char c) {
	int position = 0;
	for (int i = path.length() - 1; i >= 0; i--) {
		if (path[i] == c) {
			position = i;
			break;
		}
	}
	return position + 1;
}

// funkcja wyznaczjąca nazwę pliku bez rozszerzenia z pełnej ścieżki
std::string find_filename(std::string path) {
	std::string file_name = path.substr(find_char(path, '/'), path.length());
	file_name = file_name.substr(0, find_char(file_name,'.') - 1);
	return file_name;
}
