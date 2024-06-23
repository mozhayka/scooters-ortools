#pragma once
#include <vector>

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

struct DataModel {
    const std::vector<std::vector<int64_t>> distance_matrix;
    const std::vector<int64_t> demands;

    const std::vector<int64_t> vehicle_capacities{ 50, 50, 50, 50 };
    const int num_vehicles = 4;
    const operations_research::RoutingIndexManager::NodeIndex depot{ 0 };
};

struct VRPparams {
    int time_limit = 1;
    operations_research::FirstSolutionStrategy::Value start = operations_research::FirstSolutionStrategy::AUTOMATIC;
    operations_research::LocalSearchMetaheuristic::Value metaheuristic = operations_research::LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH;

    bool local_opt = false;
};

struct DataModelTsp {
    const std::vector<std::vector<int64_t>> distance_matrix;

    const int num_vehicles = 1;
    const operations_research::RoutingIndexManager::NodeIndex depot{ 0 };
};

struct TSPparams {
    static const int time_limit = 10;
    static const auto metaheuristic = operations_research::LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH;
};

struct PathSpecs {
    std::vector<int64_t> path;
    int64_t dist;
};

std::vector<PathSpecs> vrp(DataModel data, VRPparams params);
PathSpecs tsp(DataModelTsp data);