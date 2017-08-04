/*
 * utitlity.h
 *
 *  Created on: Mar 27, 2015
 *      Author: Tadeze
 */

#include "utility.hpp"
using namespace std;
namespace util {

	default_random_engine gen;
void initialize()
{
	//std::cout<<util::debug<<" True if debug";
    if(util::debug>=0)
		gen.seed(util::debug);  //Debugging
    else
		gen.seed(time(NULL));  //Production
}
template <typename T>
T randomT(T min, T max)
{
	uniform_real_distribution<T> dist (min,max);
	return dist(gen);

}

double randomD (double min,double max)
 {
	 uniform_real_distribution<double> dist (min,max);
	 return dist(gen);
 }


int randomI(int min, int max) 
{

	uniform_int_distribution<unsigned> dist(min,max);
	return dist(gen);

}

int randomEx(int min,int max,set<int>& exlude)
{
	int num;
	num =randomI(min,max);            //(int) (min + (rand() % (max - min+1)));
	return exlude.find(num)!=exlude.end()?randomEx(min,max,exlude):num;
			
}
void sampleI(int min,int max, int nsample,vector<int> &samples)
{
	int cnt=0;
	int rndI;
	set<int> duplicate;
	while(cnt<nsample)
	{
		rndI = randomEx(min,max,duplicate);
		samples.push_back(rndI);
		duplicate.insert(rndI);
		cnt++;
	}

}

double score(double depth,int n)
{
	return pow(2,-depth/avgPL(n));
}

template<typename T>
void swapInt(int a, int b, T* x)
 {
	int hold;
	hold = x[a];
	x[a] = x[b];
	x[b] = hold;
}

//template<typename T>
/*
 * Sample mean of points
 */
double mean(vector<double> points) {
	double sum = 0;
	for (int f = 0; f < (int) points.size(); f++)
		sum += points[f];
	return sum / (double) points.size();
}

/*
 * Calculate sample variance from vector of doublen numbers
 */
double variance(vector<double> &x){
 	double sum=0.0;
	double mn=mean(x);
	for(double elem : x)
	{
	 sum +=pow(elem-mn,2);
	}
	return sum/(double)(x.size()-1);
}

/* T-confidence interval
 *
 */
double tconf(vector<double> &points,double sigma=0.95)
{
	double sd = sqrt(variance(points));
	double tvalue = 1.64*sd/(double)sqrt(points.size());
	return tvalue;
}


/*
 * Read csv file into vector,
 * used for testing
 */

vector<vector<double> > readcsv(const char* filename, char delim = ',',
		bool header = true) {
	vector < vector<double> > values;
	vector<double> valueline;
	ifstream fin(filename);
	string item;
	if (header)  //if header available
		getline(fin, item);

	for (string line; getline(fin, line);) {
		istringstream in(line);
		while (getline(in, item, delim)) {
			valueline.push_back(atof(item.c_str()));
		}
		values.push_back(valueline);
		valueline.clear();
	}
	return values;
}

/*
 * Calculate the cumulative distribution function
 */
map<double,double> ecdf(vector<double> points) {
	map<double,double> cdfm;

	sort(points.begin(), points.end());
	int j = -1;
	double len = (double) points.size();
	for (unsigned i = 0; i < points.size(); ++i) {
		if (i == 0 || points[i - 1] != points[i]) {
			j++;
			cdfm.insert({points.at(i),(double) (i + 1) / len});
		}
		else

			{
			cdfm[points.at(i)]=(double) (i + 1) / len;
			}
	}
	return cdfm;

}


template <typename T>
vector<T> flatten(const vector<vector<T>>& v) {
    size_t total_size = 0;
    for (const auto& sub : v)
        total_size += sub.size();
    vector<T> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

double avgPL(int n) {

	return (((n - 1) <= 0) ?
			0.0 :
			((2.0 * (log((double) (n - 1)) + 0.5772156649))
					- (2.0 * (double) (n - 1)) / (1.0 + (double) (n - 1))));

}



/*
 * Compute the Anderson-Darling distance for the depth distribution of each point
 * input: 2-d depth row X tree_depth
 * return 1-D score of each point
 */

vector<double> ADdistance(vector<vector<double> > depths, bool weightToTail =
		false) {
	//flatten 2-d to 1-d and compute alldepth ECDF of using all points depths
	vector<double> alldepth = flatten(depths);
	map<double, double> Fxm = ecdf(alldepth); //all depth cdf

	vector<double> scores;
	/*
	 * compute score of each point
	 * sort the depth, compute its ECDF
	 */
	for (vector<double> x : depths) {
		sort(x.begin(), x.end());
		map<double, double>::iterator iter = Fxm.begin();
		map<double, double> fxn = ecdf(x);  //empirical cdf of the point
		double sum = 0;
		for (double elem : x) {
			double val;
			while (iter != Fxm.end()) {
				//cout<<elem<<"\t"<<iter->first<<"\t"<<iter->second<<endl;
				if (iter->first > elem) //x.at(i))
						{

					val = (--iter)->second; //= ((i==0)?.0:(--iter)->second);
					break;
				}
				++iter;
			}
			if (iter == Fxm.end())
				val = 0.99999999; //close to 1 at the end of the distribution
			double cdfDiff = max((fxn[elem]-val), 0.0);
			sum += weightToTail ? cdfDiff / sqrt(val) : cdfDiff; //distance of each point in the vector


		}
		scores.push_back(sum);
	}
		return scores;

	}
}

/** Matrix class
 *
 * @param D
 * @param N
 * @return
 */
template <typename  T>
util::Matrix<T>::Matrix(size_t rows, size_t cols):nRows(rows),
										 nCols(cols),
										 mData(rows*cols){}
template <typename  T>
double& util::Matrix<T>::operator()(size_t i, size_t j){
	return mData[i*nCols + j];
}
template <typename  T>
double util::Matrix<T>::operator()(size_t i, size_t j) const {
	return mData[i*nCols + j];
}


/* Generate 2-D data
 *
 */

std::vector<std::vector<double> > syntheticData(int D, int N)
{
	std::vector<std::vector<double> > data;
	for (int k=0;k<N;k++)
	{
		std::vector<double> row(D);
		for(int j=0;j<D;j++)
			row.push_back(util::randomD(0,2));
		data.push_back(row);
	}
	return data;
}

/* UTITLITY_H_ */

