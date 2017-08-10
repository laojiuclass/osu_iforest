/*
 * Tree.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Tadeze
 */

#ifndef TREE_H_
#define TREE_H_
#include "utility.hpp"
#include "cincl.hpp"
#include "Contribution.hpp"


class Tree {
private:
	Tree *leftChild;
	Tree *rightChild;
	Tree *parent;
	int nodeSize;
	int splittingAtt;
    double splittingPoint;
    int depth;
    double minAttVal,maxAttVal;

public:
	int getNodeSize() const;
	int getSplittingAtt() const;
	double getSplittingPoint() const;
	int getDepth() const;
	double getMinAttVal() const;
	double getMaxAttVal() const;
	static bool rangeCheck;
	std::vector<int> trainIndex;

    void serialize(std::ostream &s) const;
	void deserialize(std::istream &s) const;
	Tree* assignTree(Tree* tr, std::istream &s) const;
	Tree* getLeftChild(){return leftChild;}
	Tree* getRightChild(){return rightChild;}
	Tree() {
		leftChild = NULL;
		rightChild = NULL;
		parent = NULL;
		splittingAtt = -1;
		splittingPoint = 999;
		depth = 0;
		nodeSize = 0;
		minAttVal=maxAttVal=0;
	}
	;

	virtual ~Tree()
	{
        delete leftChild; //check if deleting the child is need.
        delete rightChild;

	};

	void iTree(std::vector<int> const &dIndex,const doubleframe* dt, int height, int maxHeight, bool stopheight);
	double pathLength(double *inst);
	bool indexAvailable(int index);

	//Contribution
	contrib featureContribution(double* inst);//std::vector<double> &inst);
	std::map<int,double> explanation(double* inst){ // /std::vector<double> &inst){
		return featureContribution(inst).featureContribution();
	};

};

#endif /* TREE_H_ */
