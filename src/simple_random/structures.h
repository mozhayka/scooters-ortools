#pragma once
#include <vector>

struct graph
{
	size_t N;

	std::vector<int> prio;
	std::vector<std::vector<int>> dist;
};

struct limits
{
	int capacity;
	int time_left;
	double penalty;
};

struct coordinates
{
	double x, y;
};

struct path
{
	std::vector<size_t> vertices;
	double gain;

	path(std::vector<size_t> v, graph g, limits lim) : vertices(v)
	{
		gain = 0;
		int sum_dist = 0;
		size_t from = 0;
		for (size_t to : v)
		{
			gain += g.prio[to];
			sum_dist += g.dist[from][to];
			from = to;
		}
		sum_dist += g.dist[from][0];
		gain -= lim.penalty * sum_dist;
	}
};