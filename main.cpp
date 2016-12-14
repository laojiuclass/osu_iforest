/*
 * main.cpp
 *
 @Created on: Mar 22, 2015
 * @Author: Tadeze
 * Main entry: accepts the the
 * @param argv
 *    Usage: iforest [options]
 *      -i FILE, --infile=FILE
 Specify path to input data file. (Required).
 -o FILE, --outfile=FILE
 Specify path to output results file. (Required).
 -m COLS, --metacol=COLS
 Specify columns to preserve as meta-data. (Separated by ',' Use '-' to specify ranges).
 -t N, --ntrees=N
 Specify number of trees to build.
Default value is 100.
 -s S, --sampsize=S
 Specify subsampling rate for each tree. (Value of 0 indicates to use entire data set).
 Default value is 2048.
 -d MAX, --maxdepth=MAX
 Specify maximum depth of trees. (Value of 0 indicates no maximum).
 Default value is 0.
 -H, --header
 Toggle whether or not to expect a header input.
 Default value is true.
 -v, --verbose
 Toggle verbose ouput.
 Default value is false.
 -h, --help
 Print this help message and exit.
 */

#include "iforest/main.hpp"
//log file
//ofstream util::logfile("treepath.csv");

//Save score to flat file
void saveScoreToFile(std::vector<double> &scores,std::vector<std::vector<double> > &pathLength,const ntstringframe* metadata, std::string fName,bool savePathLength=false)
{

  metadata=metadata;
  //Compute the AUC of the score 
  std::vector<double> groundtruth(scores.size(),0);
  // make 0/1 from label 

    std::ofstream outscore(fName);
  if(!savePathLength)
	// outscore << "groundtruth,score\n";
    outscore <<"score\n";	
  for (int j = 0; j < (int) scores.size(); j++)
    {
     /* Disable writing metadata   
      if (metadata)
        {
   
            outscore<<metadata->data[j][0]<<",";
          //if(metadata->data[j][0]=="anomaly" || metadata->data[j][0] ==1)
            //groundtruth[j] = 1;
         }
	*/	
	outscore  << scores[j]; //<<util::mean(pathLength[j])<<","<<rscores[j];
  if(savePathLength)
   {
      //for generating all depth 
    	for(int i=0;i<(int)pathLength[1].size();i++)
	      {	
		      outscore<<','<<pathLength[j][i];
	       }
    }
    outscore<<"\n";

	}

  outscore.close();

}


void buildForest(Forest &iff, doubleframe* test_dt, const double alpha,int stopLimit,float rho,
                 std::string output_name,ntstringframe* metadata,bool savePathLength,
        int epoch)
{
    if(iff.ntree>0)
      iff.fixedTreeForest(epoch) ;
    else
    {
     //int treeRequired = iff.adaptiveForest(ALPHA,stopLimit);
    	if(rho<0){
    	int treeRequired = iff.adaptiveForest(alpha,stopLimit);
    	std::cout<<"\n# of Tree required from k-agreement \n "<<treeRequired;
    	}else{
    		const int initial_tree=50;
    		int treeRequired = iff.confTree(alpha,rho,initial_tree);
    		std::cout<<"\n# of Tree required from rho based stopping\n "<<treeRequired;
    	}

    }
    std::vector<double> scores = iff.AnomalyScore(test_dt); //generate anomaly score
    std::vector<std::vector<double> > pathLength = iff.pathLength(test_dt); //generate Depth all points in all trees
saveScoreToFile(scores,pathLength,metadata,output_name,savePathLength);



}


 /* Display vector data
 */
void dispalyVec(std::vector<double> &data)
{
    for(double row : data)
    {
        std::cout<<row<<"\n";
    }
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



/* Static variable 
 */
bool Tree::rangeCheck ;  //range check for Tree score calculation.

int main(int argc, char* argv[]) 
{

    /*parse argument from command line*/
	parsed_args* pargs = parse_args(argc, argv);
	ntstring input_name = pargs->input_name;
	ntstring output_name = pargs->output_name;
    ntstring test_name = pargs->test_name;
    d(int)* metacol = pargs->metacol;
	int ntree = pargs->ntrees;
	int nsample = pargs->sampsize;
	int maxheight = pargs->maxdepth;
	bool header = pargs->header;
	bool verbose = pargs->verbose;
	verbose = verbose; //Clears warning for now.
	bool rsample = nsample != 0;
	bool stopheight = maxheight != 0;
	int  stopLimit = pargs->adaptive;
	bool rangecheck = pargs->rangecheck;	
    bool rotate = pargs->rotate;
    bool pathlength = pargs->pathlength;
    float rho  = pargs->precision;
    float alpha = pargs->alpha;
    int epoch  = pargs->epoch;
    //Input file to dataframe
    
    ntstringframe* csv = read_csv(input_name, header, false, false);
    ntstringframe* metadata = split_frame(ntstring, csv, metacol,true);
	doubleframe* dt = conv_frame(double, ntstring, csv); //read data to the global variable
    
   
    
    //Test file to data frame 
  ntstringframe* csv_test = read_csv(test_name, header, false, false);
  metadata = split_frame(ntstring, csv_test, metacol,true);
  doubleframe* test_dt=dt;
  
  if(test_name==input_name)
       test_dt = dt;
  else
      test_dt = conv_frame(double, ntstring, csv_test); //read data to the global variable
   
  nsample = nsample==0?dt->nrow:nsample;
   //const double ALPHA=0.01;
  Tree::rangeCheck = rangecheck;

 IsolationForest iff(ntree,dt,nsample,maxheight,stopheight,rsample); //build iForest
 buildForest(iff,test_dt,alpha,stopLimit,rho,output_name,metadata,pathlength,epoch);
    
 if(rotate)  //check for rotation forest
 {
    RForest rff(ntree,dt,nsample,maxheight,stopheight,rsample);
    std::string rot_output(output_name);
    buildForest(rff,test_dt,alpha,stopLimit,rho,"rotate_"+rot_output,metadata,pathlength,epoch);
 }
  //Anomaly score and path length
  util::logfile.close();
  return 0;
}












