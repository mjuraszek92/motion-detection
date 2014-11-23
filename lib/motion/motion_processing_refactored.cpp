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
