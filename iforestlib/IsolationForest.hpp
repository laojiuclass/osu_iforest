/*
 * IsolationForest.hpp
 *
 *  Created on: May 5, 2015
 *      Author: tadeze
 */

#ifndef ISOLATIONFOREST_HPP_
#define ISOLATIONFOREST_HPP_
#include "Forest.hpp"
#include "cereal/types/polymorphic.hpp"

class IsolationForest:public Forest{
    public:
	IsolationForest()= default;

	IsolationForest(int _ntree,doubleframe* _df,
			int _nsample,int _maxheight, bool _stopheight,bool _rsample);
	//int adaptiveForest(double alpha,int stopLimit);
	void buildForest();
	virtual ~IsolationForest()
	{
	}
	//convergent iForest
  int adaptiveForest(double alpha,int stopLimit); 
   void fixedTreeForest(int epoch);
  // int confTree(double alpha,double rho,int init_tree);
   int batchForest(int epoch);

	template<class Archive>
	void serialize(Archive & archive){
		archive(cereal::make_nvp("ntree",ntree),cereal::make_nvp("nsample",nsample),
				cereal::make_nvp("rsample",rsample),cereal::make_nvp("stopheight",stopheight),
				cereal::make_nvp("trees",trees));

	}
};


CEREAL_REGISTER_TYPE(IsolationForest);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Forest,IsolationForest);


#endif /* ISOLATIONFOREST_HPP_ */
