#ifndef FISH_HPP
#define FISH_HPP

#include <algorithm>
#include <cstdlib>
#include <ostream>
#include <random>

/*
----------------------------------------
Default Hyper-parameters
----------------------------------------
*/

constexpr std::size_t NUM_STEPS = 1000;
constexpr std::size_t NUM_FISH = 60000000;
constexpr double SQUARE = 100;
constexpr double INITIAL_WEIGHT = 2000;
constexpr double INITIAL_FOOD = 1;

/*
----------------------------------------
*/

namespace {
	std::random_device rd;
	std::mt19937 gen(rd());
	double random_real_number(double min, double max) {
		std::uniform_real_distribution distribution(min, max);
		return distribution(gen);
	}
} // namespace

class Fish {
public:
	double x_;
	double y_;
	double weight_;
	double distance_;

	Fish()
	: x_{random_real_number(-100, 100)}
	, y_{random_real_number(-100, 100)}
	, weight_{INITIAL_WEIGHT} {}

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

	bool operator==(Fish const &fish) const = default;

	friend std::ostream &operator<<(std::ostream &os, Fish const &fish) {
		return os << std::fixed << '(' << fish.x_ << ", " << fish.y_ << ") "
		          << fish.weight_;
	}

private:
	void swim() {
		if (weight_ == 2 * INITIAL_WEIGHT) return;
		x_ += random_real_number(-0.1, 0.1);
		y_ += random_real_number(-0.1, 0.1);
		x_ = std::clamp(x_, -SQUARE, SQUARE);
		y_ = std::clamp(y_, -SQUARE, SQUARE);
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

#endif // FISH_HPP
