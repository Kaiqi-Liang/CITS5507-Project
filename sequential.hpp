#include <algorithm>
#include <exception>
#include <iostream>
#include <vector>

#include "fish.hpp"

void sequential(std::vector<Fish> &school) {
	for (std::size_t i = 0; i < NUM_STEPS; i++) {
		double max_difference = 0;
		double numerator = 0;
		double denominator = 0;

		for (std::size_t j = 0; j < NUM_FISH; j++) {
			max_difference = std::max(max_difference, school[j].difference());
		}

		for (std::size_t j = 0; j < NUM_FISH; j++) {
			school[j].action(i, max_difference);
		}

		for (std::size_t j = 0; j < NUM_FISH; j++) {
			numerator += school[j].distance_ * school[j].weight_;
			denominator += school[j].distance_;
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
