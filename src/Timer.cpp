#include "Timer.hpp"

void Timer::start() {
	startTime = clock();
}

void Timer::stop() {
	stopTime = clock();
}

double Timer::getElapsed() {
	return double(startTime - stopTime) / CLOCKS_PER_SEC;
}

void Timer::printElapsed() {
	cout << "Time elapsed: " << getElapsed() << endl;
}
