#include "src/IO.hpp"
#include "src/vrp_solver/solver.hpp"

#include <format>

static std::string PATH = "C:/Users/mozha/source/repos/scooters/";

using namespace operations_research;
static std::string meta(auto metaheuristic) {
    switch (metaheuristic) {
    case LocalSearchMetaheuristic::GREEDY_DESCENT:
        return "GREEDY-DESCENT";
    case LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH:
        return "GUIDED-LOCAL-SEARCH";
    case LocalSearchMetaheuristic::SIMULATED_ANNEALING:
        return "SIMULATED-ANNEALING";
    case LocalSearchMetaheuristic::TABU_SEARCH:
        return "TABU-SEARCH";
    case LocalSearchMetaheuristic::GENERIC_TABU_SEARCH:
        return "GENERIC-TABU-SEARCH";
    default:
        return "?";
    }
}

static std::string start_strat(auto start) {
    switch (start) {
    case FirstSolutionStrategy::PATH_CHEAPEST_ARC:
        return "PATH-CHEAPEST-ARC";
    case FirstSolutionStrategy::SAVINGS:
        return "SAVINGS";
    case FirstSolutionStrategy::SWEEP:
        return "SWEEP";
    case FirstSolutionStrategy::CHRISTOFIDES:
        return "CHRISTOFIDES";
    case FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION:
        return "PARALLEL-CHEAPEST-INSERTION";
    default:
        return "?";
    }
}

static std::string graphics_filename(VRPparams params, int N, int couriers, int total_time) {
    auto folder = std::format("{}graphics/input{}/couriers-{}/", PATH, N, couriers);
    auto filename = std::format("result-{}_tl-{}s_meta-{}_start-{}_opt-{}.svg", 
        total_time, 
        params.time_limit, 
        meta(params.metaheuristic),
        start_strat(params.start),
        params.local_opt ? "Y" : "N");
    return std::format("{}{}", folder, filename);
}



void save_txt(int N = 5, int couriers = 1) {
    for (int i = 1; i <= N; i++) {
        std::string input_file = std::format("{}input_files/input{}.txt", PATH, i);
        std::string output_file = std::format("{}output_files/output{}.txt", PATH, i);

        auto [data, coordinates] = from_file(input_file, couriers);
        auto result = vrp(data, VRPparams(100, FirstSolutionStrategy::CHRISTOFIDES, LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH));
        print_solution(output_file, result);
    }
}

void show_plot(int i, int couriers) {
    std::string input_file = std::format("{}input_files/input{}.txt", PATH, i);
    std::string output_file = std::format("{}output_files/output{}.txt", PATH, i);

    auto [data, coordinates] = from_file(input_file, couriers);
    auto params = VRPparams();
    auto result = vrp(data, params);
    std::string graphics_file = graphics_filename(params, i, couriers, 0);
    show_plot(result, coordinates, graphics_file, true);
}

void full_tryhard() {
    std::vector<int> filenames{ 4 };
    std::vector<int> couriers_cnt{ 10 };
    std::vector<int> time_limits{ 1 };
    std::vector< FirstSolutionStrategy::Value> starts{ 
        FirstSolutionStrategy::PATH_CHEAPEST_ARC,
        //FirstSolutionStrategy::SAVINGS,
        //FirstSolutionStrategy::CHRISTOFIDES,
        //FirstSolutionStrategy::PARALLEL_CHEAPEST_INSERTION,
    };
    std::vector<LocalSearchMetaheuristic::Value> metas{
        //LocalSearchMetaheuristic::GREEDY_DESCENT,
        LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH,
        //LocalSearchMetaheuristic::SIMULATED_ANNEALING,
        //LocalSearchMetaheuristic::TABU_SEARCH,
        //LocalSearchMetaheuristic::GENERIC_TABU_SEARCH,
    };

    for (auto i : filenames) {
        std::string input_file = std::format("{}input_files/input{}.txt", PATH, i);
        for (auto couriers : couriers_cnt) {
            auto [data, coordinates] = from_file(input_file, couriers);
            for (auto tl : time_limits) {
                int j = 0;
                for (auto start : starts) {
                    for (auto meta : metas) {
                        auto params = VRPparams(tl, start, meta, true);
                        auto result = vrp(data, params);
                        int total_time = 0;
                        for (auto& res : result) {
                            total_time += res.dist;
                        }

                        std::string graphics_file = graphics_filename(params, i, couriers, total_time);
                        show_plot(result, coordinates, graphics_file, true);
                    }
                }
            }
        }
    }
}

int main() {
    full_tryhard();
    //save_txt();
    return 0;
}