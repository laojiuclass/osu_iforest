//
// Created by zemichet on 3/30/16.
//

#include "pyForest.hpp"


void pyForest::saveModel(std::string modelName) {

}

pyForest::pyForest(int _ntree, int _nsample,int _maxheight, bool _rotate, bool _adaptive,
		bool _rangecheck,double _rho,int _stoplimit=0) {
	ntree = _ntree;
	nsample = _nsample;
	rotate = _rotate;
	maxheight= _maxheight;
	adaptive = _adaptive;
	rangecheck = _rangecheck;
	traindf = new doubleframe();
	testdf = new doubleframe();
	rho = _rho;
	stoplimit = _stoplimit;
	iff = nullptr;
}

void pyForest::trainForest(std::vector<std::vector<double> >& _traindf) {

	bool rsample = nsample != 0;
	bool stopheight = maxheight != 0;

	convertVtoDf(_traindf,traindf);

	if(rotate)
	{
		IsolationForest iforest(ntree,traindf,nsample,maxheight,stopheight,rsample); //build iForest
		iff = &iforest;

	}
	else{
		RForest rff(ntree,traindf,nsample,maxheight,stopheight,rsample);
		//string rot_output(output_name);
		//buildForest(rff,test_dt,alpha,stopLimit,rho,"rotate_"+rot_output,metadata,pathlength);
		iff = &rff;
		}
	//Build the forest
	//Forest &iff, doubleframe* test_dt, const double alpha,int stopLimit,float rho
	buildForestPy(*iff,testdf,ALPHA,stoplimit,rho);
	//buildForestPy(iff,testdf,ALPHA,stoplimit,rho);
	//buildForest(iff,testdf,ALPHA,stoplimit,rho);

}



void pyForest::testForest(std::vector<std::vector<double> >& _testdf) {

    	//morph _testdf vector to doublerame format.
	convertVtoDf(_testdf,testdf);

	}

void convertVtoDf(std::vector<std::vector<double> > sourceVec,doubleframe* df)
{
	int _ncol =(int) sourceVec[0].size();
	int _nrow =(int) sourceVec.size();

	for(int i=0;i<_nrow;i++)
	{
		for(int j=0;j<_ncol;j++)
		{
			df->data[i][j] = sourceVec[i][j];

		}
	}
	df->ncol = _ncol;
	df->nrow = _nrow;

}

Forest pyForest::loadModel(std::string modelName) {
    return nullptr;
}

std::vector<double> pyForest::getScore() {
    return iff->AnomalyScore(testdf);
}


std::vector<std::vector<double> > pyForest::PathLength() {
	return iff->pathLength(testdf);
}

std::vector<double> pyForest::averageDepth() {
	std::vector<double> meandepth(testdf->nrow);
	for(std::vector<double> row : this->PathLength())
	{
		meandepth.push_back(util::mean(row));
	}
return meandepth;
}






