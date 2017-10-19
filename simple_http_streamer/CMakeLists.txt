cmake_minimum_required(VERSION 2.8.3)
project(streamer)


set(CMAKE_CXX_FLAGS "-O2 -O3 -std=c++11 -isystem -mtune=native -fPIC -Wall -Wno-deprecated-declarations -Wno-reorder")
set(CMAKE_BUILD_TYPE Release                                                                    )
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"                                          )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}"                                        )

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenCV REQUIRED)
find_package(OpenSceneGraph)

message(status "openscenegraph found: ${OPENSCENEGRAPH_FOUND}")
message(status "openscenegraph include: ${OPENSCENEGRAPH_INCLUDE_DIR}")
message(status "openscenegraph library: ${OPENSCENEGRAPH_LIBRARIES}")

message(status "opencv include: " ${OpenCV_INCLUDE_DIRS})
message(status "opencv library: " ${OpenCV_LIBRARIES})


set(OSG_LIGB
    osg
    osgDB
    osgUtil
    osgGA
    osgViewer
    osgText
    osgAnimation
    osgManipulator
    osgTerrain
    osgFX
    osgShadow
    OpenThreads)



include_directories(include ${INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIR})


add_executable(streamer main.cpp video_server.cpp tcp_connection.cpp video_capture.cpp)
target_link_libraries(streamer GL GLU glut ${Boost_LIBRARIES} ${OSG_LIGB} ${OPENSCENEGRAPH_LIBRARIES} ${OpenCV_LIBRARIES} )

