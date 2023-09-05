/**
 * @author Kaiqi Liang (23344153)
 * @author Briana Davies-Morrell (22734723)
 */
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "fish.hpp"
#include "sequential.hpp"
#ifdef PARALLEL
#include "parallel_for.hpp"
#include "parallel_tasks.hpp"
#include "parallel_partition.hpp"
#endif

int main() {
	clock_t begin = std::clock();

	std::vector<Fish> school(NUM_OF_FISH);
	sequential(school);

#ifdef PARALLEL_FOR
	parallel_for(school);
#endif

#ifdef PARALLEL_TASKS
	parallel_tasks(school);
#endif

#ifdef PARALLEL_PARTITION
	parallel_partition(school);
#endif

	clock_t end = std::clock();
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << std::endl;
	return 0;
}
