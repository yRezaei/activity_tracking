cmake_minimum_required(VERSION 2.8.12)
project(activity_tracker CXX)

add_definitions("-std=c++14 -DNDEBUG")


add_executable(activity_tracker main.cpp)
target_link_libraries(activity_tracker)