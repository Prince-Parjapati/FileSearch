cmake_minimum_required(VERSION 3.10)
project(MultithreadedFileSearch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(file_search_tool
    main.cpp
    file_search.cpp
)
