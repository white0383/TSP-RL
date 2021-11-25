cmake_minimum_required(VERSION 3.13)
project(tsp_RL)

add_definitions("-std=c++17")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

find_library(
  LAPACKE 
  NAMES liblapacke.a 
  HINTS /usr/local/lib 
  REQUIRED
)
find_library(
  LAPACK 
  NAMES liblapack.a 
  HINTS /usr/local/lib 
  REQUIRED
)
find_library(
  BLAS 
  NAMES libblas.a 
  HINTS /usr/local/lib 
  REQUIRED
)

find_library(
  GFORTRAN 
  NAMES libgfortran.a 
  HINTS /usr/local/Cellar/gcc/11.2.0_2/lib/gcc/11 
  REQUIRED
)

find_library(
  QUADMATH
  NAMES libquadmath.a 
  HINTS /usr/local/Cellar/gcc/11.2.0_2/lib/gcc/11 
  REQUIRED
)

add_executable(${PROJECT_NAME} 
  ./src/main.cpp 
  ./src/model/Graph.cpp 
  ./src/model/Node.cpp 
  ./src/model/Distance.cpp
  ./src/model/Tour.cpp
  ./src/model/Arguments.cpp
  ./src/RLmodel/LinearFittedQIteration.cpp
  ./src/RLmodel/MarkovDecisionProcess.cpp
  ./src/helper/FileIOHelper.cpp
  ./src/helper/mt19937ar.cpp
  ./src/helper/RandVec.cpp
  ./src/helper/fitLinearQ.cpp
  ./src/solver/initial_solution/GenerateInitialSolution.cpp
  ./src/solver/initial_solution/method/RandomTour.cpp
  ./src/solver/initial_solution/method/NearestNeighbor.cpp
  ./src/solver/initial_solution/method/FarthestInsertion.cpp
  ./src/solver/local_search/SearchLocalOpt.cpp
  ./src/solver/local_search/method/TwoOpt.cpp
  ./src/solver/local_search/method/FastTwoOpt.cpp
  ./src/solver/local_search/method/ScaledFastTwoOpt.cpp
)

target_link_libraries(${PROJECT_NAME} ${LAPACKE} ${LAPACK} ${BLAS} ${GFORTRAN} ${QUADMATH})