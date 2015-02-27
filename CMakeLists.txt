project(map2json)
cmake_minimum_required(VERSION 2.8)
add_definitions (-std=c++11 -Wall)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

