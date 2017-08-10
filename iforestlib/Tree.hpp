/*
 * Tree.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Tadeze
 */

#ifndef TREE_H_
#define TREE_H_
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/polymorphic.hpp"
#include "utility.hpp"
#include "cincl.hpp"
#include "Contribution.hpp"
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

   // void serialize(std::ostream &s) const;
//	void deserialize(std::istream &s) const;
	/*Tree* assignTree(Tree* tr, std::istream &s) const;
	Tree* getLeftChild(){return leftChild;}
	Tree* getRightChild(){return rightChild;}
	 */
    const std::shared_ptr<Tree> &getLeftChild() const;
    const std::shared_ptr<Tree> &getRightChild() const;

    Tree():leftChild(nullptr),rightChild(nullptr),parent(nullptr)
    ,splittingAtt(-1),splittingPoint(999),depth(0),nodeSize(0),
    minAttVal(0),maxAttVal(0) {};

	virtual ~Tree()
	{
  /*      delete leftChild; //check if deleting the child is need.
        delete rightChild;
*/
	};

	void iTree(std::vector<int> const &dIndex,const doubleframe* dt, int height, int maxHeight, bool stopheight);
	double pathLength(double *inst);
	bool indexAvailable(int index);

	//Contribution
	contrib featureContribution(double* inst) const;//std::vector<double> &inst);
	std::map<int,double> explanation(double* inst){ // /std::vector<double> &inst){
		return featureContribution(inst).featureContribution();
	};

    // Serialization
    template<class Archive>
    void  serialize(Archive & archive){
        archive(cereal::make_nvp("nodesize",nodeSize),cereal::make_nvp("depth",depth),
                cereal::make_nvp("splittingAtt",splittingAtt),cereal::make_nvp("splittingPoint",splittingPoint),
                cereal::make_nvp("minAttVal",minAttVal),cereal::make_nvp("maxAttVal",maxAttVal),
                cereal::make_nvp("leftChild",leftChild),cereal::make_nvp("rightChild",rightChild)

        );
    }

};

#endif /* TREE_H_ */
