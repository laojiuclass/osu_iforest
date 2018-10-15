//
// Created by tadeze on 10/10/18.
//

#include "gtest/gtest.h"
//#include "../Addition.hpp"
#include "r_matrix.hpp"

class RMatrix : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {

        // Code here will be called immediately after each test

        // (right before the destructor).
    }

};
TEST (RMatrix, cbind){
    Eigen::MatrixXd x;//empty matrix
    PRINT(x);

    Eigen::MatrixXd y(3,1);
    y<<1.0,10.0,100.0;
    PRINT(y);

    Eigen::MatrixXd a = cbind(x,y);
    PRINT(a);

    Eigen::MatrixXd ans(3,1);
    ans<<1.0,10.0,100.0;
    PRINT(ans);

    ASSERT_LE(std::abs((ans-a).sum()), 0.01);
}


//
//Eigen::MatrixXf x(6,1);
//x<<1.0,2.0,3.0,4.0,5.0,6.0;