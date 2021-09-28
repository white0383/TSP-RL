cmake_minimum_required(VERSION 3.13)
project(tsp_RL)

add_definitions("-std=c++17")

add_executable(tsp 
  ./src/main.cpp 
  ./src/model/Graph.cpp 
  ./src/model/Node.cpp 
  ./src/model/Distance.cpp
  ./src/model/Tour.cpp
  ./src/model/Arguments.cpp
  ./src/helper/FileIOHelper.cpp
  ./src/helper/mt19937ar.cpp
  ./src/solver/initial_solution/GenerateInitialSolution.cpp
  ./src/solver/initial_solution/method/RandomTour.cpp
  ./src/solver/initial_solution/method/NearestNeighbor.cpp
  ./src/solver/local_search/SearchLocalOpt.cpp
  ./src/solver/local_search/method/TwoOpt.cpp
)