cmake_minimum_required(VERSION 3.2)
project(http_video_streamer)

set(CMAKE_CXX_FLAGS "-O2 -O3 -std=c++11 -mtune=native -fPIC -Wall")        ## Optimize

find_package(Boost REQUIRED COMPONENTS system thread)
#
# /usr/lib
find_package(OpenCV REQUIRED PATHS "/usr/")

set(OPENCV_INCLUDE_DIR /usr/include/opencv2)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${Boost_INCLUDE_DIR} ${OPENCV_INCLUDE_DIR})

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


include_directories(${INCLUDE_DIR})

set(HEADER_FILES ${INCLUDE_DIR}/http_video_server/video_capture.h)

set(SRC_FILES   ${SOURCE_DIR}/video_capture.cpp
                ${SOURCE_DIR}/main.cpp)


include_directories(${INCLUDE_DIR} )

add_executable(video_streamer ${SRC_FILES})
target_link_libraries(video_streamer ${Boost_LIBRARIES} ${OpenCV_LIBS})


