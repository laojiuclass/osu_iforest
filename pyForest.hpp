//
// Created by zemichet on 3/30/16.
//

#ifndef PYFOREST_HPP
#define PYFOREST_HPP
#include "main.hpp"
//#include "utility.hpp"

class pyForest {
private:
    int ntree;
    int nsample;
    int maxheight;
    bool rotate;
    bool adaptive;
    bool rangecheck;
    double rho;
    int stoplimit;
    doubleframe *traindf;
    doubleframe *testdf;
    Forest *iff;
    float const ALPHA=0.01;
public:
    //constructor and Destructor
    virtual ~pyForest(){
    delete traindf;
    delete testdf;
    delete iff;
    };
    pyForest();
    pyForest(int _ntree, int _nsample,int _maxheight, bool _rotate, bool _adaptive,
    		bool _rangecheck,double _rho,int stopLimit);


    int getNtree() const {
        return ntree;
    }

    void setNtree(int ntree) {
        pyForest::ntree = ntree;
    }

    int getNsample() const {
        return nsample;
    }

    void setNsample(int nsample) {
        pyForest::nsample = nsample;
    }

    int getMaxdepth() const {
        return maxheight;
    }

    void setMaxdepth(int maxdepth) {
        pyForest::maxheight = maxdepth;
    }

    bool isRotate() const {
        return rotate;
    }

    void setRotate(bool rotate) {
        pyForest::rotate = rotate;
    }

    bool isAdaptive() const {
        return adaptive;
    }

    void setAdaptive(bool adaptive) {
        pyForest::adaptive = adaptive;
    }

    bool isRangecheck() const {
        return rangecheck;
    }

    void setRangecheck(bool rangecheck) {
        pyForest::rangecheck = rangecheck;
    }

    //Methods

    void trainForest(std::vector<std::vector<double> > &traindf);
    void testForest(std::vector<std::vector<double> > &testdf);
    void saveModel(std::string modelName);
    Forest loadModel(std::string modelName);
    std::vector<double> getScore();
    std::vector<std::vector<double> > PathLength();
    std::vector<double> averageDepth();
    void convertVtoDf(std::vector<std::vector<double> > sourceVec,doubleframe* df);


};


#endif //IFOREST_ADDIN_PYFOREST_HPP
