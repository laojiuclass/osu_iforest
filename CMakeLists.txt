cmake_minimum_required(VERSION 3.2)
project(osu_iforest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## User defined options
option(BUILD_DOC "Build documenation with Doxygen" OFF)
option(BUIL_EXe "Build documenation with Doxygen" OFF)
option(BUIL_shared_LIBS "Build documenation with Doxygen" OFF)



if (UNIX)
    message("Serialization of models can work good.")
add_definitions(-DSERIALIZATION=true)
endif (UNIX)
if (WIN32)
    message("Serialization don't work properly.")
endif(WIN32)
include_directories(src)
add_subdirectory(src)
if(BUILD_DOC)
    find_package(Doxygen)
        if(DOXYGEN_FOUND)
            configure_file(${PROJECT_SOURCE_DIR}/doc/Doxygen/Doxyfile.in ${PROJECT_SOURCE_DIR}/doc/Doxyfile @ONLY)
            add_custom_target(${PROJECT_NAME}_doc ALL ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/Doxyfile)
        endif()
endif()
#configure_file("${PROJECT_SOURCE_DIR}/cmake/cmake_install.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
#install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
# Third party libs
FIND_PACKAGE(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})
 # look for Eigen
set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/eigen)  ## Added by TZ
# look for Eigen
if (DEFINED ENV{EIGEN3_INCLUDE_DIR} AND NOT DEFINED EIGEN3_INCLUDE_DIR) # use env variable if not set
    set(EIGEN3_INCLUDE_DIR $ENV{EIGEN3_INCLUDE_DIR})
endif()
get_filename_component(EIGEN3_INCLUDE_DIR "${EIGEN3_INCLUDE_DIR}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
message("-- Eigen dir is " ${EIGEN3_INCLUDE_DIR})
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
#find_package(Eigen3 REQUIRED)
if (NOT Eigen_FOUND)
    include(${PROJECT_SOURCE_DIR}/lib/eigen/CTestConfig.cmake)
    set(Eigen_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
endif()
include_directories("${EIGEN3_INCLUDE_DIR}")
include_directories(${SOURCE_FILES}/src/cereal)
# Enable testing
enable_testing()
add_subdirectory(test)

add_executable(iforest main.cpp ${SOURCE_FILES})
target_link_libraries(iforest iforestlib Clib ${Eigen_INCLUDE_DIR}) # ${ARMADILLO_INCLUDE_DIRS})
