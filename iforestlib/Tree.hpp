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

struct Contrib{
	int feature;
	std::map<int,std::vector<double> > contributions;
    Contrib(int ncols):feature(ncols) { //initialize all features.
        for(int i=0;i<feature;i++) {
            std::vector<double> contr;
            contr.push_back(0.0);
            contributions.insert({i, contr});

        }
    }

	void addcont(int index, double depth){
		if(contributions.count(index)<1) {
			std::vector<double> contr;
			contr.push_back(depth);
			contributions.insert({index,contr});
		}
		else{
			contributions[index].push_back(depth);
		}
	}
	std::map<int,double> featureContribution(){
		std::map<int,double> explanation;


		for(const auto & contr : contributions){
			double expl=0.0;
			expl = 1.0/contr.second[0];
			//for(auto depth : contr.second)
			//   expl += 1.0/depth;
			explanation.insert({contr.first,expl});

		}
		return explanation;
	}

};
typedef  struct Contrib contrib;
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

	//Contribution
	contrib featureContribution(double* inst);//std::vector<double> &inst);
	std::map<int,double> explanation(double* inst){ // /std::vector<double> &inst){
		return featureContribution(inst).featureContribution();
	};

};

#endif /* TREE_H_ */
