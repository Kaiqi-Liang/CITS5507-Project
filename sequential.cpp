#include <ctime>
#include <iomanip>
#include <iostream>

#define NUM_OF_STEPS 100
#define NUM_OF_STEPS 100
#define SQUARE 100

void setup() {
	// Generate initial location for all the fish randomly
}

int main() {
	clock_t begin = std::clock();
	clock_t end = std::clock();

	setup();
	
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << std::endl;
	return 0;
}
