/*
 * main.hpp
 *
 *  Created on: May 5, 2015
 *      Author: tadeze
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#include "RForest.hpp"
#include "IsolationForest.hpp"
#include "utility.hpp"
//#include "cincl.hpp"
//#include "convForest.hpp"

void buildForest(std::shared_ptr<Forest> iff, doubleframe* test_dt, const double alpha,int stopLimit,float rho,
		std::string output_name,ntstringframe* metadata,bool savePathLength);

//void buildForestPy(Forest &iff, doubleframe* test_dt, const double alpha,int stopLimit,float rho);

void saveScoreToFile(std::vector<double> &scores,
		std::vector<std::vector<double> > &pathLength,
		const ntstringframe* metadata, std::string fName,bool savePathLength);

#endif /* MAIN_HPP_ */
