//
// Created by tadeze on 10/11/18.
//

#ifndef OSU_IFOREST_DOUBLEFRAME_HPP
#define OSU_IFOREST_DOUBLEFRAME_HPP
#include "r_matrix.hpp"
#include <vector>
#include <fstream>

class DoubleFrame {
private:
    long nrow, ncol;
    std::set<std::string> columns;
public:
    np::DoubleMatrix data;
    //DoubleMatrix data;
    virtual ~DoubleFrame() {};

    DoubleFrame(np::DoubleMatrix &matrix, std::set<std::string> &cols) {
        data = matrix;
        cols = cols;
    }

    DoubleFrame(np::DoubleMatrix &matrix) {
        data = matrix;
    }

    DoubleFrame() {};

    //DoubleMatrix data(){ return data}
    inline void set_data(np::DoubleMatrix &data) {
        nrow = data.rows();
        ncol = data.cols();
    }

    long const nrows() { return nrow; }

    long const ncols() { return ncol; }
    inline np::DoubleMatrix rows(int i){
        return data.row(i);
    }
    inline np::DoubleMatrix cols(int i){
        return data.col(i);
    }
};




#endif //OSU_IFOREST_DOUBLEFRAME_HPP
