#include "random.h"

random::random(graph g) : g(g)
{
	calc_probabilities();
}

void random::calc_probabilities()
{
	for (size_t from = 0; from < g.N; from++)
	{
		probability.push_back({});
		zero_dist.push_back({});
		double sum_priopity = 0;
		for (size_t to = 0; to < g.N; to++)
		{
			if (g.dist[from][to] == 0)
			{
				zero_dist[from].push_back(to);
				probability[from].push_back(sum_priopity);
				continue;
			}
			sum_priopity += g.prio[to] / g.dist[from][to];
			probability[from].push_back(sum_priopity);
		}
	}
}

size_t random::next_v(size_t from, std::vector<bool> used)
{
	for (size_t to : zero_dist[from])
	{
		if (!used[to])
			return to;
	}

	for (int i = 0; i < tries; i++)
	{
		size_t to = log_next(from);
		if (!used[to])
			return to;
	}

	return linear_next(from, used);
}

size_t random::linear_next(size_t from, std::vector<bool> used)
{
	double sum_priopity = 0;
	for (size_t to = 0; to < g.N; to++)
	{
		if (used[to])
			continue;
		if (g.dist[from][to] == 0)
			return to;
		double edge_priority = g.prio[to] / g.dist[from][to];
		sum_priopity += edge_priority;
	}

	double p = (rand() / static_cast<double>(RAND_MAX)) * sum_priopity;
	for (size_t to = 0; to < g.N; to++)
	{
		if (used[to])
			continue;

		double edge_priority = g.prio[to] / g.dist[from][to];
		if (p - edge_priority <= 0)
		{
			return to;
		}
		p -= edge_priority;
	}
	return -1;
}

size_t random::log_next(size_t from)
{
	double p = (rand() / static_cast<double>(RAND_MAX)) * probability[from].back();
	size_t l = -1, r = g.N;
	while (r - l > 1)
	{
		size_t m = (l + r) / 2;
		if (probability[from][m] < p)
			l = m;
		else
			r = m;
	}

	return r;
}