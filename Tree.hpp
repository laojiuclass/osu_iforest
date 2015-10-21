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

	Tree *leftChild;
	Tree *rightChild;
	Tree *parent;
	int nodeSize;
	int splittingAtt;
	double splittingPoint;
	int depth;
	bool isLeaf;
	

public:
	Tree()
	{
		leftChild = NULL;
		rightChild = NULL;
		parent = NULL;
		splittingAtt = -1;
		splittingPoint = 999;
		depth = 0;
		isLeaf = false;
		nodeSize = 0;

	}
	;

	virtual ~Tree()
	{
        delete leftChild; //check if deleting the child is need.
        delete rightChild;

	};

	void iTree(std::vector<int> const &dIndex,const doubleframe* dt, int height, int maxHeight, bool stopheight);
	double pathLength(double *inst);

};

#endif /* TREE_H_ */
