/*
 * Forest.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: tadeze
 */

#include "Forest.hpp"
//TODO: Complete the out of bag scoring 
//Update tree for saving index used for training. 
double Forest::getdepth(double* inst,Tree* tree)
{
	return tree->pathLength(inst);
}

/*
 * Accepts single point (row) and return Anomaly Score
 */
double Forest::instanceScore(double *inst) {
	double avgPathLength = util::mean(pathLength(inst));
	double scores = pow(2, -avgPathLength / util::avgPL(this->nsample));
	return scores;
}

/*
 * Score for  a set of dataframe in dataset
 */
std::vector<double> Forest::AnomalyScore(doubleframe* df) {
	std::vector<double> scores;
	//iterate through all points
	for (int inst = 0; inst <df->nrow; inst++) {
		scores.push_back(instanceScore(df->data[inst]));
	}
	return scores;
}


/*
 * Score for out of bag scoring 
 */
std::vector<double> Forest::outOfBagScore(doubleframe* df) {
	std::vector<double> scores;
	double score;
//    int numTreeUsed;
    double avgDepth;
//	//iterate through all points
//	for (int inst = 0; inst <df->nrow; inst++)
//	{
//	  depth =0.0;
//	  for(int it=0;it<this->ntree;it++)
//	    {
//         numTreeUsed =0;
//		if(!this->trees[it]->indexAvailable(inst)){
//			depth +=getdepth(df->data[inst],this->trees[it]);
//			numTreeUsed++;
//		}
//
//	   }
    std::vector<std::vector<double> > depths = oOBPathLength(df);
    for(auto& instDepth : depths) {
     avgDepth =  util::mean(instDepth);
     score = pow(2, -avgDepth / util::avgPL(this->nsample));
     scores.push_back(score);

    }

	//score = pow(2,-(depth/numTreeUsed)*util::avgPL(this->nsample));
	//scores.push_back(score);
 

	return scores;
}
std::vector<std::vector<double> > Forest::oOBPathLength(doubleframe *df){
	std::vector<double> scores;
	double depth;
	//iterate through all points
	std::vector<std::vector<double> > depths;
	for (int inst = 0; inst <df->nrow; inst++) {
		std::vector<double> itDepth;
		for(int it=0;it<this->ntree;it++) {
			depth =0.0;
			if(!this->trees[it]->indexAvailable(inst)) {
				depth = getdepth(df->data[inst],this->trees[it]);
			}
			itDepth.push_back(depth);
		}
		depths.push_back(itDepth);
	}
	return depths;
}



/*
 * Return instance depth in all trees
*/

std::vector<double> Forest::pathLength(double *inst)
{
	std::vector<double> depth;
	for (std::vector<Tree*>::iterator it = this->trees.begin(); it != trees.end();
			++it)
	{
 	
		 depth.push_back((*it)->pathLength(inst));

	}
	return depth;
}

/* PathLength for all points
*/
std::vector<std::vector<double> > Forest::pathLength(doubleframe*  data)
{
	std::vector < std::vector<double> > depths;
	for (int r = 0; r < data->nrow; r++)
		depths.push_back(pathLength(data->data[r]));
	return depths;
}

/*
 * Anderson_Darling test from the pathlength
 */
/*
vector<double> IsolationForest::ADtest(const vector<vector<double> > &pathlength,bool weighttotail)
{

//Will fix later
	return ADdistance(pathlength,weighttotail);
}*/

//Sample data from the datset
void Forest::getSample(std::vector<int> &sampleIndex,const int nsample,bool rsample,int nrow)
{
	sampleIndex.clear();
	if (rsample && nsample < nrow)
		util::sampleI(0, nrow - 1, nsample, sampleIndex); //sample nsample
	else
		util::sampleI(0, nrow - 1, nrow, sampleIndex); //shuffle all index of the data



}

int Forest::adaptiveForest(double alpha,int stopLimit)
{
//For now remove warning compiler
 return (int)alpha*stopLimit;
}
int Forest::confTree(double alpha,double rho, int init_tree){
	//For now remove warning
	return (int)alpha*rho*init_tree;
}
/*
 * Feature contribution
 */
std::vector<std::map<int, double> > Forest::featureContrib(double *inst) {
	// Iterate through all trees and collect their contributions

	std::vector<std::map<int, double> > contributions;
	for (auto tree : this->trees) {
		auto treeexplanation = tree->featureContribution(inst).featureContribution();
		contributions.push_back(treeexplanation);
	}

	return contributions;
}

/* Compute the feature importance of a point
 * input: *inst data instance
 * output: feature importance
 * status: Incomplete!!
 */
std::map<int, double>
Forest::importance(double* inst){ //std::vector<double> &inst) {
	//Need to be re-implemented
	std::map<int, double> totalexplan;
	std::vector<std::map<int,double> > featureExplanation = Forest::featureContrib(inst);
	for (const auto feature : featureExplanation) {
		for (auto explan : feature) {

			if (totalexplan.count(explan.first) > 0)
				totalexplan[explan.first] += explan.second;
			else
				totalexplan.insert(explan);
		}
	}
    return totalexplan;
}

void Forest::featureExplanation(doubleframe* df,std::ofstream &out){
	out<<"ix";
    // /header
    for(int i=0;i<df->ncol;i++)
        out<<","<<"V"<<i; //+std::to_string(i);

    out<<"\n";
    for (int r = 0; r < df->nrow; r++) {
        out<<r;
        for(auto mpr : this->importance(df->data[r]))
            out<<","<<mpr.second;
        out<<"\n";
    }

}
//TODO: Modify for all explanation methods