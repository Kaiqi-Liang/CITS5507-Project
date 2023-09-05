#ifndef FISH_HPP
#define FISH_HPP

#include <algorithm>
#include <cstdlib>
#include <random>

/*
----------------------------------------
Default Hyper-parameters
----------------------------------------
*/

constexpr std::size_t NUM_OF_STEPS = 4000;
constexpr std::size_t NUM_OF_FISH = 4000;
constexpr double SQUARE = 100;
constexpr double INITIAL_WEIGHT = 2000;
constexpr double INITIAL_FOOD = 1;

/*
----------------------------------------
*/

double random_real_number(double min, double max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution distribution(min, max);
	return distribution(gen);
}

class Fish {
public:
	double distance_;
	double weight_;

	Fish()
	: weight_{INITIAL_WEIGHT}
	, x_{random_real_number(-100, 100)}
	, y_{random_real_number(-100, 100)} {}

	void action(std::size_t step, double max_difference) {
		eat(step, max_difference);
		swim();
	}

	double difference() const {
		return std::abs(distance() - distance_);
	}

	bool operator<(Fish const &fish) const {
		return (x_ == fish.x_) ? y_ < fish.y_ : x_ < fish.x_;
	}

private:
	double x_;
	double y_;

	void swim() {
		if (weight_ == 2 * INITIAL_WEIGHT) return;
		x_ += random_real_number(-0.1, 0.1);
		y_ += random_real_number(-0.1, 0.1);
		std::clamp(x_, -SQUARE, SQUARE);
		std::clamp(y_, -SQUARE, SQUARE);
	}

	void eat(std::size_t step, double max_difference) {
		if (step == 0) {
			weight_ += INITIAL_FOOD;
		} else {
			weight_ +=
			    (max_difference == 0) ? 0 : (difference() / max_difference);
		}
		distance_ = distance();
		weight_ = std::min(weight_, 2 * INITIAL_WEIGHT);
	}

	double distance() const {
		return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2));
	}
};
#endif
