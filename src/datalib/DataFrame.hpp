//
// Created by tadeze on 8/13/17.
//

#ifndef OSU_IFOREST_DATAFRAME_HPP
#define OSU_IFOREST_DATAFRAME_HPP
#include<Eigen/Dense>
using namespace Eigen;
#include <string>
#include <vector>
template <class T>
class DataFrame<T> {
    std::string colnames;
    std::string rownames;
    int ncol_;
    int nrow_;

    std::vector<std::vector<T> > data;
public:
    virtual DataFrame()= default;
    int nrow(){
        return nrow_;
    }
    int ncol(){
        return ncol_;
    }

};


#endif //OSU_IFOREST_DATAFRAME_HPP
