cmake_minimum_required(VERSION 3.8)
project(ideasBank_v1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp AVL_Tree.h FileHandler.h Idea.h IdeasBank.h)
add_executable(ideasBank_v1 ${SOURCE_FILES})