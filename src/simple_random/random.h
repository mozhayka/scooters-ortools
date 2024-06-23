#pragma once
#include <vector>
#include "structures.h"

class random
{
private:
	graph g;
	std::vector<std::vector<double>> probability;
	std::vector<std::vector<size_t>> zero_dist;
	const int tries = 5;

public:
	random() = delete;
	random(graph g);
	size_t next_v(size_t from, std::vector<bool> used);


private:
	void calc_probabilities();
	size_t linear_next(size_t from, std::vector<bool> used);
	size_t log_next(size_t from);
};

