#pragma once
#include "vrp_solver/solver.hpp"

struct coordinates
{
	double x, y;
};

struct InputData {
	DataModel model;
	std::vector<coordinates> position;
};

InputData from_file(std::string filename, int couriers_cnt = 1);
void print_solution(std::string filename, const std::vector<PathSpecs>& solver);
void show_plot(const std::vector<PathSpecs>& solver, std::vector<coordinates> position, std::string filename, bool _show = false);
void compare(std::vector<int> tls, std::vector<std::vector<int>> results, std::string filename, std::vector<std::string> names);
