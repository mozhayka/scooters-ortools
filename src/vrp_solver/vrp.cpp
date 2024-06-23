#include "solver.hpp"
using namespace operations_research;

std::vector<PathSpecs> save_result(const DataModel& data, const RoutingIndexManager& manager,
    const RoutingModel& routing, const Assignment& solution) {
    std::vector<PathSpecs> result;

    for (int vehicle_id = 0; vehicle_id < data.num_vehicles; ++vehicle_id) {
        int64_t index = routing.Start(vehicle_id);
        int64_t route_distance = 0;
        std::vector<int64_t> path;

        while (!routing.IsEnd(index)) {
            const int node_index = manager.IndexToNode(index).value();
            path.push_back(node_index);
            const int64_t previous_index = index;
            index = solution.Value(routing.NextVar(index));
            route_distance += routing.GetArcCostForVehicle(previous_index, index,
                int64_t{ vehicle_id });
        }
        result.push_back({ path, route_distance });
    }

    return result;
}

std::vector<DataModelTsp> solo_pathes(DataModel data, std::vector<PathSpecs> result) {
    std::vector<DataModelTsp> ans;
    for (auto& path : result) {
        std::vector<std::vector<int64_t>> distance_matrix;
        for (int i = 0; i < path.path.size(); ++i) {
            distance_matrix.push_back(std::vector<int64_t>());
            for (int j = 0; j < path.path.size(); ++j) {
                int64_t from = path.path[i];
                int64_t to = path.path[j];
                distance_matrix[i].push_back(data.distance_matrix[from][to]);
            }
        }
        ans.push_back({ distance_matrix });
    }
    return ans;
}

std::vector<PathSpecs> local_optimization(DataModel data, std::vector<PathSpecs> result) {
    std::vector<PathSpecs> optimized_result;
    int num = 0;
    for (auto solo_data : solo_pathes(data, result)) {
        std::vector<int64_t> path;
        auto [tsp_path, dist] = tsp(solo_data);
        for (auto point : tsp_path) {
            path.push_back(result[num].path[point]);
        }

        optimized_result.push_back({ path, dist });
        ++num;
    }
    return optimized_result;
}

std::vector<PathSpecs> vrp(DataModel data, VRPparams params) {
    RoutingIndexManager manager(data.distance_matrix.size(), data.num_vehicles,
        data.depot);
    RoutingModel routing(manager);

    // Create and register a transit callback.
    const int transit_callback_index = routing.RegisterTransitCallback(
        [&data, &manager](const int64_t from_index,
            const int64_t to_index) -> int64_t {
                // Convert from routing variable Index to distance matrix NodeIndex.
                const int from_node = manager.IndexToNode(from_index).value();
                const int to_node = manager.IndexToNode(to_index).value();
                return data.distance_matrix[from_node][to_node];
        });

    // Define cost of each arc.
    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

    // Add Capacity constraint.
    const int demand_callback_index = routing.RegisterUnaryTransitCallback(
        [&data, &manager](const int64_t from_index) -> int64_t {
            // Convert from routing variable Index to demand NodeIndex.
            const int from_node = manager.IndexToNode(from_index).value();
            return data.demands[from_node];
        });
    routing.AddDimensionWithVehicleCapacity(
        demand_callback_index,    // transit callback index
        int64_t{ 0 },               // null capacity slack
        data.vehicle_capacities,  // vehicle maximum capacities
        true,                     // start cumul to zero
        "Capacity");

    // Allow to drop nodes.
    int64_t penalty{ 1000 };
    for (int i = 1; i < data.distance_matrix.size(); ++i) {
        routing.AddDisjunction(
            { manager.NodeToIndex(RoutingIndexManager::NodeIndex(i)) }, penalty);
    }

    // Setting first solution heuristic.
    RoutingSearchParameters search_parameters = DefaultRoutingSearchParameters();
    search_parameters.set_first_solution_strategy(params.start);
    search_parameters.set_local_search_metaheuristic(params.metaheuristic);
    search_parameters.mutable_time_limit()->set_seconds(params.time_limit);

    const Assignment* solution = routing.SolveWithParameters(search_parameters);
    auto result = save_result(data, manager, routing, *solution);

    return params.local_opt ? local_optimization(data, result) : result;
}
