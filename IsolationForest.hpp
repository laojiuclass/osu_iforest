/*
 * IsolationForest.hpp
 *
 *  Created on: May 5, 2015
 *      Author: tadeze
 */

#ifndef ISOLATIONFOREST_HPP_
#define ISOLATIONFOREST_HPP_
#include "Forest.hpp"

class IsolationForest:public Forest{
    public:
	IsolationForest();

	IsolationForest(int _ntree,doubleframe* _df,
			int _nsample,int _maxheight, bool _stopheight,bool _rsample);
	//int adaptiveForest(double alpha,int stopLimit);
	void buildForest();
	virtual ~IsolationForest()
	{
	}
	//convergent iForest
  int adaptiveForest(double alpha,int stopLimit); 
  void fixedTreeForest();
   int confTree(double alpha,double rho,int init_tree);
   int batchForest();
};



#endif /* ISOLATIONFOREST_HPP_ */
