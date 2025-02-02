cmake_minimum_required(VERSION 3.2)

project(thrdpool VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(thrdpool STATIC 
                            src/thrdpool.cpp
)

target_include_directories(thrdpool PUBLIC
                                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                            $<INSTALL_INTERFACE:include>
)