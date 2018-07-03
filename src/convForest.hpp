/*
 * convForest.h
 *
 *  Created on: Aug 12, 2015
 *      Author: tadeze
 */

#ifndef CONVFOREST_H_
#define CONVFOREST_H_
#include "forest.hpp"
class convForest: public Forest {

    struct topscore
    {
        bool operator() (const std::pair<int,double> p1,const std::pair<int,double> p2)
        {
            return p1.second < p2.second;
        }
    };
    struct larger
    {
        bool operator()(const std::pair<int,double> p1,const std::pair<int,double> p2)

        {
            return p1.second <p2.second;
        }
    };
    void inserTopK(std::vector<std::pair<int,int> > &sl,int b);
    int tau;
    int alpha;
    double topcommonK(std::vector<int> &v1,std::vector<int> &v2);

public:
	convForest(int _ntree,doubleframe* _df,const int _nsample,int _maxheight,bool _stopheight,
            bool _rsample,double _tau,double _alpha):Forest(_ntree,_df,_nsample,_maxheight,_stopheight, _rsample)
	{tau=_tau;alpha=_alpha;}
	virtual ~convForest()=default;
	void convergeIF(double tau,double alpha);
//Sequential confidence interval stopping
	void confstop(double alpha);


};
#endif /* CONVFOREST_H_ */
