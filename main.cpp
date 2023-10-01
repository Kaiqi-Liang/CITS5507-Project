/**
 * @author Kaiqi Liang (23344153)
 * @author Briana Davies-Morrell (22734723)
 */
#include <ctime>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <vector>

#include "fish.hpp"

#ifdef SEQUENTIAL
#include "sequential.hpp"
#elif PARALLEL_FOR
#include "parallel_for.hpp"
#elif PARALLEL_TASKS
#include "parallel_tasks.hpp"
#elif PARALLEL_TASK_PER_FISH
#include "parallel_task_per_fish.hpp"
#elif PARALLEL_PARTITION
#include "parallel_partition.hpp"
#endif

int main() {
	std::vector<Fish> school(NUM_FISH);
	double begin = omp_get_wtime();

#ifdef SEQUENTIAL
	sequential(school);
	std::cout << "sequential\n";
#elif PARALLEL_FOR
	parallel_for(school);
	std::cout << "parallel_for\n";
#elif PARALLEL_TASKS
	parallel_tasks(school);
	std::cout << "parallel_tasks\n";
#elif PARALLEL_TASK_PER_FISH
	parallel_task_per_fish(school);
	std::cout << "parallel_task_per_fish\n";
#elif PARALLEL_PARTITION
	parallel_partition(school);
	std::cout << "parallel_partition\n";
#endif

	double end = omp_get_wtime();
	double time_spent = end - begin;
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << " seconds\n";
	return 0;
}
