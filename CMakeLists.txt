cmake_minimum_required(VERSION 2.8)
project(osu_iforest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (UNIX)
    message("Serialization of models can work good.")
add_definitions(-DSERIALIZATION=true)
endif (UNIX)
if (WIN32)
    message("Serialization don't work properly.")
endif(WIN32)
FIND_PACKAGE(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})
include_directories(iforestlib)
add_subdirectory(iforestlib)
 # look for Eigen
set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/eigen)  ## Added by TZ
# look for Eigen
if (DEFINED ENV{EIGEN3_INCLUDE_DIR} AND NOT DEFINED EIGEN3_INCLUDE_DIR) # use env variable if not set
    set(EIGEN3_INCLUDE_DIR $ENV{EIGEN3_INCLUDE_DIR})
endif()
get_filename_component(EIGEN3_INCLUDE_DIR "${EIGEN3_INCLUDE_DIR}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
message("-- Eigen dir is " ${EIGEN3_INCLUDE_DIR})
find_package(Eigen3 QUIET)
if (NOT Eigen_FOUND)
    include(${PROJECT_SOURCE_DIR}/lib/eigen/CTestConfig.cmake)
    set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()
include_directories(${EIGEN3_INCLUDE_DIR})

FIND_PACKAGE(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

#find_package(Armadillo REQUIRED)
#include_directories(${ARMADILLO_INCLUDE_DIRS})
include_directories(${SOURCE_FILES}/iforestlib/cereal)
set(SOURCE_FILES main.cpp)
add_executable(iforest ${SOURCE_FILES})

target_link_libraries(iforest iforestlib Clib) # ${ARMADILLO_INCLUDE_DIRS})
