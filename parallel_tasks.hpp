#include <algorithm>
#include <exception>
#include <iostream>
#include <omp.h>
#include <vector>

#include "fish.hpp"

void parallel_tasks(std::vector<Fish>& school) {
	for (std::size_t i = 0; i < NUM_STEPS; i++) {
		double max_difference = 0;
		double numerator = 0;
		double denominator = 0;
		#pragma omp parallel
		{
			#pragma omp for reduction(max: max_difference) schedule(static)
			for (std::size_t j = 0; j < NUM_FISH; j++) {
				max_difference = std::max(max_difference, school[j].difference());
			}

			#pragma omp for schedule(static)
			for (std::size_t j = 0; j < NUM_FISH; j++) {
				school[j].action(i, max_difference);
			}

			#pragma omp single
			{
				#pragma omp task
				for (std::size_t j = 0; j < NUM_FISH; j++) {
					numerator += school[j].distance_ * school[j].weight_;
				}
				#pragma omp task
				for (std::size_t j = 0; j < NUM_FISH; j++) {
					denominator += school[j].distance_;
				}
			}
		}

		double barycentre = numerator / denominator;
		if (barycentre <= 0) {
			throw std::exception();
		}
#ifdef DEBUG
		std::cout << std::fixed << barycentre << std::endl;
#endif
	}
}
