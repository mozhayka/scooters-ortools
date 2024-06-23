cmake_minimum_required(VERSION 3.18)
project(scooters VERSION 1.0)

# x64 Release
list(APPEND CMAKE_PREFIX_PATH "../or-tools_x64_VisualStudio2022_cpp_v9.9.3963")
include("C:/Users/mozha/vcpkg/scripts/buildsystems/vcpkg.cmake")
find_package(ortools CONFIG REQUIRED)
find_package(Matplot++ CONFIG REQUIRED)

add_executable(main main.cpp "src/IO.cpp" "src/vrp_solver/vrp.cpp" "src/vrp_solver/tsp.cpp")

target_link_libraries(main ortools::ortools)
target_link_libraries(main Matplot++::cimg  Matplot++::matplot)