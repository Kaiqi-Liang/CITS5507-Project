#include <ctime>
#include <iomanip>
#include <iostream>

#define NUM_OF_STEPS 100

int main() {
	clock_t begin = std::clock();
	clock_t end = std::clock();
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << std::endl;
	return 0;
}
