/**
 * @author Kaiqi Liang (23344153)
 * @author Briana Davies-Morrell (22734723)
 */
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
	std::cout << "sequential\n";
	sequential(school);
#elif PARALLEL_FOR
	parallel_for(school);
	std::cout << "parallel_for\n";
#elif PARALLEL_TASKS
	std::cout << "parallel_tasks\n";
	parallel_tasks(school);
#elif PARALLEL_TASK_PER_FISH
	std::cout << "parallel_task_per_fish\n";
	parallel_task_per_fish(school);
#elif PARALLEL_PARTITION
	std::cout << "parallel_partition\n";
	parallel_partition(school);
#endif

	double end = omp_get_wtime();
	double time_spent = end - begin;
	std::cout << "time spent:" << std::fixed << std::setw(10)
	          << std::setprecision(6) << time_spent << " seconds\n";
	return 0;
}
