/*
 * convForest.h
 *
 *  Created on: Aug 12, 2015
 *      Author: tadeze
 */

#ifndef CONVFOREST_H_
#define CONVFOREST_H_

#include "Forest.hpp"
class convForest: public Forest {
public:
	int tau;
	int alpha;
	convForest(int ntree,doubleframe* df,int maxheight,bool stopheight, const int nsample, 
            bool rSample,double _tau,double _alpha):Forest(ntree,df,nsample,maxheight,stopheight, rSample)
	{tau=_tau;alpha=_alpha;}
	virtual ~convForest()=default;

	void convergeIF(double tau,double alpha);
//Sequential confidence interval stopping 
	void confstop(double alpha);


};
#endif /* CONVFOREST_H_ */
