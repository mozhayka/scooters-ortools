#include "solver.hpp"
using namespace operations_research;

PathSpecs save_result(const DataModelTsp& data, const RoutingIndexManager& manager,
    const RoutingModel& routing, const Assignment& solution) {
    auto result = std::vector<int64_t>();
    int64_t distance{ 0 };

    int64_t index = routing.Start(0);
    while (!routing.IsEnd(index)) {
        result.push_back(manager.IndexToNode(index).value());
        const int64_t previous_index = index;
        index = solution.Value(routing.NextVar(index));
        distance += routing.GetArcCostForVehicle(previous_index, index, int64_t{ 0 });
    }   
    return { result, distance };
}

PathSpecs tsp(DataModelTsp data) {
    RoutingIndexManager manager(data.distance_matrix.size(), data.num_vehicles,
        data.depot);

    RoutingModel routing(manager);

    const int transit_callback_index = routing.RegisterTransitCallback(
        [&data, &manager](const int64_t from_index,
            const int64_t to_index) -> int64_t {
                // Convert from routing variable Index to distance matrix NodeIndex.
                const int from_node = manager.IndexToNode(from_index).value();
                const int to_node = manager.IndexToNode(to_index).value();
                return data.distance_matrix[from_node][to_node];
        });

    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);

    RoutingSearchParameters searchParameters = DefaultRoutingSearchParameters();
    searchParameters.set_local_search_metaheuristic(TSPparams::metaheuristic);
    searchParameters.mutable_time_limit()->set_seconds(TSPparams::time_limit);
    searchParameters.set_log_search(true);

    std::vector<int64_t> initial_path(data.distance_matrix.size() - 1);
    std::iota(initial_path.begin(), initial_path.end(), 1);
    const Assignment* initial_solution = routing.ReadAssignmentFromRoutes({ initial_path }, true);
    const Assignment* solution = routing.SolveFromAssignmentWithParameters(initial_solution, searchParameters);

    return save_result(data, manager, routing, *solution);
}