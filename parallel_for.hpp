#include <algorithm>
#include <exception>
#include <iostream>
#include <omp.h>
#include <vector>

#include "fish.hpp"

void parallel_for(std::vector<Fish>& school) {
	for (std::size_t i = 0; i < NUM_OF_STEPS; i++) {
		double max_difference = 0;
		double numerator = 0;
		double denominator = 0;
		#pragma omp parallel
		{
			#pragma omp for reduction(max : max_difference)
			for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
				max_difference =
				    std::max(max_difference, school[j].difference());
			}

			#pragma omp for
			for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
				school[j].action(i, max_difference);
			}

			#pragma omp for reduction(+ : numerator, denominator)
			for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
				numerator += school[j].distance_ * school[j].weight_;
				denominator += school[j].distance_;
			}
		}

		double barycentre = numerator / denominator;
		if (barycentre <= 0) {
			throw std::exception();
		}
#ifdef DEBUG
		std::cout << barycentre << std::endl;
#endif
	}
}
