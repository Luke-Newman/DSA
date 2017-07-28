cmake_minimum_required(VERSION 3.6)
project(mini_SHRDLU_v1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Mini_SHRDLU.h AtomGoal.h StateGenerator.h State.h Cell.h DisjunctiveSolver.h Action.h ConjunctiveSolver.h GoalStateGenerator.h)
add_executable(mini_SHRDLU_v1 ${SOURCE_FILES})