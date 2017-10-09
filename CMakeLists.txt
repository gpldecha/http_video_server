cmake_minimum_required(VERSION 3.2)
project(http_video_streamer)

set(CMAKE_CXX_FLAGS "-O2 -O3 -std=c++11 -mtune=native -fPIC -Wall")        ## Optimize

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


include_directories(${INCLUDE_DIR})

set(SRC_FILES ${SOURCE_DIR}/main.cpp)


include_directories(${HEAD_FILES} )

add_executable(video_streamer ${SRC_FILES})
target_link_libraries(video_streamer ${Boost_LIBRARIES} -lpthread)


