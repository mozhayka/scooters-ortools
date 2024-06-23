#pragma once
#include "structures.h"
#include "random.h"
#include <map>


class path_generator
{
private:
	graph g;
	random r;

public:
	path_generator(graph g);

	path gen(limits lim);
	path gen_x_times(limits lim, int x = 10);
};

