/*
 * Tree.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Tadeze
 */

#ifndef TREE_H_
#define TREE_H_

#ifdef SERIALIZATION
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/polymorphic.hpp"
#endif

#include "utility.hpp"
#include "cincl.hpp"
#include "contribution.hpp"
#include <memory>

class Tree: public std::enable_shared_from_this<Tree> {
private:

    std::shared_ptr<Tree> leftChild,rightChild,parent;
    int nodeSize,splittingAtt,depth;
    double splittingPoint,minAttVal,maxAttVal;

public:
	int getNodeSize() const;
	int getSplittingAtt() const;
	double getSplittingPoint() const;
	int getDepth() const;
	double getMinAttVal() const;
	double getMaxAttVal() const;
	static bool rangeCheck;
	std::vector<int> trainIndex;
    Tree():leftChild(nullptr),rightChild(nullptr),parent(nullptr)
    ,splittingAtt(-1),splittingPoint(999.0),depth(0),nodeSize(0),
    minAttVal(0.0),maxAttVal(0.0) {};

	virtual ~Tree() = default;

	void iTree(std::vector<int> const &dIndex,const doubleframe* dt, int height, int maxHeight, bool stopheight);
	double pathLength(double *inst);
	bool indexAvailable(int index);

	//Contribution
	contrib featureContribution(double* inst) const;
	inline std::map<int,double> explanation(double* inst){
		return featureContribution(inst).featureContribution();
	};
#ifdef SERIALIZATION
    // Serialization
    template<class Archive>
    void  serialize(Archive & archive){
        archive(cereal::make_nvp("nodesize",nodeSize),cereal::make_nvp("depth",depth),
                cereal::make_nvp("splittingAtt",splittingAtt),cereal::make_nvp("splittingPoint",splittingPoint),
                cereal::make_nvp("minAttVal",minAttVal),cereal::make_nvp("maxAttVal",maxAttVal),
                cereal::make_nvp("leftChild",leftChild),cereal::make_nvp("rightChild",rightChild)

        );
    }
#endif

};

#endif /* TREE_H_ */
