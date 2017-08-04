/*
 * utitlity.h
 *
 *  Created on: Mar 27, 2015
 *      Author: Tadeze
 */
#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include<iostream>
#include<fstream>
#include<sstream>
#include <cmath>
#include <cstdlib>
#include<ctime>
#include<algorithm>
#include<queue>
#include<string>
#include<iterator>
#include<vector>
#include<algorithm>
#include<map>
#include<set>
#include "cincl.hpp"
#include<random>
#include<utility>

////default_random_engine gen(time(NULL));
namespace util{

	void initialize();
int randomI(int min, int max);
int randomEx(int min,int max,std::set<int>& exlude);
void sampleI(int min, int max, int nsample, std::vector<int> &sampleIndx);
double avgPL(int n);
//inline
double randomD(double min, double max);
template <typename T>
T randomT(T min, T max);
void swapInt(int a, int b, int* x);
//template<typename T>
double variance(std::vector<double> &x);

//template<typename T>
double mean(std::vector<double> points);

double tconf(std::vector<double> &points, double sigma);

std::vector<std::vector<double> > readcsv(const char* filename, char delim,
		bool header);
//extern std::ofstream ffile; //("log.txt");
std::map<double,double> ecdf(std::vector<double> points);
std::vector<double> ADdistance(const std::vector<std::vector<double> > &depths, bool weightToTail);

//void convertVtoDf(std::vector<std::vector<double> > &sourceVec,doubleframe* df);
//Matrix class
	template <typename T>
	class Matrix{
	public:
		Matrix(size_t rows, size_t cols);
		double& operator()(size_t i, size_t j);
		double operator()(size_t i, size_t j) const;
		int nrows();
		size_t ncols();

	private:
		size_t nRows;
		size_t nCols;
		std::vector<T> mData;
	};

//extern doubleframe* dt;
//log file
extern std::ofstream logfile;
extern std::string tmpVar;
extern int debug;
double score(double depth,int n);
//extern Data *dt;
}
#endif
/* UTITLITY_H_ */

