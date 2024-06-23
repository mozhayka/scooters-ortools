#include <iostream>
#include <fstream>
#include <matplot/matplot.h>
#include <cmath>

#include "IO.hpp"

using namespace operations_research;
using std::cout, std::endl;

InputData from_file(std::string filename, int couriers_cnt)
{
	std::ifstream in(filename);
	if (!in) {
		std::cout << "Cannot open file " + filename << std::endl;
		throw new std::runtime_error("Cannot open file " + filename);
	}

	int n;
	in >> n;
	n++;
	std::vector<coordinates> position(n);

	for (int i = 0; i < n; i++)
	{
		double x, y;
		int p;
		in >> x >> y >> p;

		position[i] = { x, y };
		
	}

	std::vector<int64_t> demands(n, 1);
	demands[0] = 0;
	std::vector<std::vector<int64_t>> dist(n, std::vector<int64_t>(n, 0));

	for (int from = 0; from < n; from++)
	{
		for (int to = 0; to < n; to++)
		{
			int64_t time_dist;
			in >> time_dist;
			dist[from][to] = time_dist;
		}
	}

	int capacity, time_left;
	double penalty;
	in >> capacity;
	in >> time_left;
	in >> penalty;

	return { DataModel(dist, demands, std::vector<int64_t>(couriers_cnt, capacity), couriers_cnt), position };
}

void print_solution(std::string filename, const std::vector<PathSpecs>& solver) {
	std::ofstream out(filename);
	if (!out) {
		std::cout << "Cannot open file " + filename << std::endl;
		throw new std::runtime_error("Cannot open file " + filename);
	}

	int num = 0;
	for (auto path : solver) {
		out << path.path.size() - 1 << endl;
		cout << "size: " << path.path.size() - 1 << " time spend: " << path.dist << endl;
		for (int i = 1; i < path.path.size(); ++i) {
			out << path.path[i] << " ";
			cout << path.path[i] << " ";
		}
		out << endl;
		cout << endl;
		++num;
	}
}

void show_plot(const std::vector<PathSpecs>& solver, std::vector<coordinates> position, std::string filename, bool _show) {
	using namespace matplot;
	std::vector<double> x, y;
	cla();
	hold(on);


	for (auto pos : position) {
		x.push_back(pos.x);
		y.push_back(pos.y);
	}

	scatter(x, y, 1)->display_name("SCOOTERS");
	scatter(std::vector<double>{ position[0].x }, std::vector<double>{ position[0].y }, 5)->display_name("BASE");

	xlabel("Longitude");
	ylabel("Latitude");

	int total_time = 0;
	for (int i = 0; i < solver.size(); ++i) {
		std::vector<double> x1, y1;
		for (auto idx : solver[i].path) {
			x1.push_back(position[idx].x);
			y1.push_back(position[idx].y);
		}
		x1.push_back(position[0].x);
		y1.push_back(position[0].y);

		total_time += solver[i].dist;
		auto p = plot(x1, y1);
		auto l = ::matplot::legend({p}, { std::format("{} MIN {} SEC", solver[i].dist / 60, solver[i].dist % 60)});
		l->font_size(5);
	}

	int hours = total_time / 3600;
	int minutes = (total_time % 3600) / 60;
	int secs = total_time % 60;

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << hours << ":"
		<< std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << secs;

	title(std::format("TOTAL: {}", oss.str()));
	gca()->title_enhanced(false);
	save(filename);
	//if (_show)
	//	show();
}

void compare(std::vector<int> tls, std::vector<std::vector<int>> results, std::string filename, std::vector<std::string> names) {
	using namespace matplot;
	cla();
	hold(on);
	title("COMPARING METAHEURISTICS");
	xlabel("Execution time (seconds)");
	ylabel("Total time (minutes)");

	xticks({ 1, 3, 10, 30, 100, 300, 1000 });
	xrange({ 1, 1000 });

	yticks({ 300, 360, 420, 480, 540, 600 });
	yrange({ 300, 600 });

	int i = 0;
	for (auto result : results) {
		auto p = semilogx(tls, result);
		auto l = ::matplot::legend({ p }, { names[i++]});
		l->font_size(5);
	}

	save(filename);
	// show();
}