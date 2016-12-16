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

class Tree
{
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
	Tree* getLeftChild(){return leftChild;}
	Tree* getRightChild(){return rightChild;}
	Tree()
	{
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


};

#endif /* TREE_H_ */
