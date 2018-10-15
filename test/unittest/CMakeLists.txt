include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(runiforestTests
        Addition_test.cpp test_basiccheck.cpp rmatrix_test.cpp)
target_link_libraries(runiforestTests gtest gtest_main)
target_link_libraries(runiforestTests Clib iforestlib ${Eigen_INCLUDE_DIR}) # ${ARMADILLO_INCLUDE_DIRS})

#test_Forest.cpp test_FacadeForest.cpp

