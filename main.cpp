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
#include "parallel_task_per_fish.hpp"
#include "parallel_partition.hpp"
#endif

int main() {
	std::vector<Fish> school(NUM_OF_FISH);

#ifdef SEQUENTIAL
	clock_t begin = std::clock();
	sequential(school);
#else
	double begin = omp_get_wtime();
#endif

#ifdef PARALLEL_FOR
	parallel_for(school);
#endif

#ifdef PARALLEL_TASKS
	parallel_tasks(school);
#endif

#ifdef PARALLEL_TASK_PER_FISH
	parallel_task_per_fish(school);
#endif

#ifdef PARALLEL_PARTITION
	parallel_partition(school);
#endif

#ifdef PARALLEL
	double end = omp_get_wtime();
	double time_spent = end - begin;
#else
	clock_t end = std::clock();
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
#endif
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << " seconds\n";
	return 0;
}
