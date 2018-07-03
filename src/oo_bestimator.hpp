//
// Created by tadeze on 8/6/17.
//
#include "utility.hpp"
struct oo_bestimator{
    util::Matrix<bool> *mat;
    int ntree,sampleSize;
    oo_bestimator(size_t _ntree, size_t _nsample):
            ntree(_ntree),sampleSize(_nsample){
        mat = new util::Matrix<bool>(_ntree,_nsample);
        for(unsigned i=0;i<_ntree;i++)
            for(unsigned j=0;j<_nsample;j++)
                (*mat)(i,j) = false; //initialy all false
    }
    void markTree(int treeIndex, int xIndex){
        if(treeIndex>ntree || xIndex > sampleSize)
            return ;
        (*mat)(treeIndex,xIndex) = true;
    }
    /**
     * Returns all out of bag trees that don't invole x
     * @param xIndex : int index of sample
     * @return Tree index in the forest that XIndex don't involve
     */
    std::vector<int> OOBTrees(int xIndex){
        std::vector<int> treeIndexes;
        for(size_t i=0;i<ntree;i++)
            if((*mat)(i,(size_t )xIndex))
                treeIndexes.push_back(i);
        return treeIndexes;
    }

    ~oo_bestimator(){delete mat;}
};
typedef struct oo_bestimator OOBEstimator;