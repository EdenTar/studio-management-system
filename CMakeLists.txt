cmake_minimum_required(VERSION 3.20)
project(SPLassignment1)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)

add_executable(SPLassignment1 src/main.cpp
        src/Action.cpp
        src/Customer.cpp
        src/Trainer.cpp
        src/Workout.cpp
        src/Studio.cpp)
