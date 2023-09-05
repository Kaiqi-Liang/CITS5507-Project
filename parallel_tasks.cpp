/**
 * @author Kaiqi Liang (23344153)
 * @author Briana Davies-Morrell (22734723)
 */
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

/*
----------------------------------------
Hyper-parameters
----------------------------------------
*/

constexpr std::size_t NUM_OF_STEPS = 10000;
constexpr std::size_t NUM_OF_FISH = 10000;
constexpr double SQUARE = 100;
constexpr double INITIAL_WEIGHT = 5000;
constexpr double INITIAL_FOOD = 1;
/*
----------------------------------------
*/

class Fish {
public:
	double distance_;
	double weight_;

	Fish()
	: weight_{INITIAL_WEIGHT}
	, gen_(rd_()) {
		std::uniform_real_distribution distribution(-100.0, 100.0);
		x_ = distribution(gen_);
		y_ = distribution(gen_);
	}

	void action(std::size_t step, double max_difference) {
		eat(step, max_difference);
		swim();
	}

	double difference() {
		return std::abs(distance() - distance_);
	}

private:
	std::random_device rd_;
	std::mt19937 gen_;
	double x_;
	double y_;

	void swim() {
		if (weight_ == 2 * INITIAL_WEIGHT) return;
		std::uniform_real_distribution distribution(-0.1, 0.1);
		x_ += distribution(gen_);
		y_ += distribution(gen_);
		std::clamp(x_, -SQUARE, SQUARE);
		std::clamp(y_, -SQUARE, SQUARE);
	}

	void eat(std::size_t step, double max_difference) {
		if (step == 0) {
			weight_ += INITIAL_FOOD;
		} else {
			weight_ += (max_difference == 0) ? 0 : (difference() / max_difference);
		}
		distance_ = distance();
		weight_ = std::min(weight_, 2 * INITIAL_WEIGHT);
	}

	double distance() {
		return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2));
	}
};

int main() {
	clock_t begin = std::clock();

	std::vector<Fish> school(NUM_OF_FISH);
	for (std::size_t i = 0; i < NUM_OF_STEPS; i++) {
		double max_difference = 0;
		double numerator = 0;
		double denominator = 0;
		#pragma omp parallel
		{
			#pragma omp for reduction(max: max_difference)
			for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
				max_difference = std::max(max_difference, school[j].difference());
			}

			#pragma omp for
			for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
				school[j].action(i, max_difference);
			}

			#pragma omp single
			{
				#pragma omp task
				for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
					numerator += school[j].distance_ * school[j].weight_;
				}
				#pragma omp task
				for (std::size_t j = 0; j < NUM_OF_FISH; j++) {
					denominator += school[j].distance_;
				}
			}
		}
		double barycentre = numerator / denominator;
		// std::cout << barycentre << std::endl;
	}

	clock_t end = std::clock();
	std::cout << begin << std::endl;
	std::cout << end << std::endl;
	double time_spent = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
	std::cout << "time spent:" << std::fixed << std::setw(10)
			  << std::setprecision(6) << time_spent << std::endl;
	return 0;
}
