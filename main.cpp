/*
 * main.cpp
 *
 @Created on: Mar 22, 2015
 * @Author: Tadeze
 *
 *
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

#include "iforestlib/main.hpp"
//log file
//ofstream util::logfile("treepath.csv");
#include "cereal/archives/binary.hpp"
#include "cereal/types/utility.hpp"

/**
 *
 * @param scores : Generated anomaly score of training data
 * @param pathLength : All tree depths across the forest
 * @param metadata : A prefix column to include in the output file
 * @param fName : String name of output file
 * @param savePathLength : A boolean that toggle whether to save depth to file/not
 */
void saveScoreToFile(std::vector<double> &scores, std::vector<std::vector<double> > &pathLength,
                     const ntstringframe *metadata, std::string fName, bool savePathLength = false) {
    metadata = metadata;
    //Compute the AUC of the score
    std::vector<double> groundtruth(scores.size(), 0);
    std::ofstream outscore(fName);
    if (!savePathLength)
        // outscore << "groundtruth,score\n";
        outscore << "score\n";
    for (int j = 0; j < (int) scores.size(); j++) {
        outscore << scores[j];
        if (savePathLength) {
            //for generating all depth
            for (int i = 0; i < (int) pathLength[1].size(); i++) {
                outscore << ',' << pathLength[j][i];
            }
        }
        outscore << "\n";
    }
    outscore.close();
}
/*!
 *
 * @param iff : An instance of a Forest object.
 * @param alpha : Anomaly proportion for training Isolation Forest using adaptive trees (0,1)
 * @param stopLimit : Stop growing trees after seeing stopLimit agreement
 * @param rho : Relaxation parameter for stopping condition (0,1)
 * @param epoch : Number of epoch to grow if batch tree growing chosen.
 */
void buildForest(std::shared_ptr<Forest> iff,  const double alpha, int stopLimit, float rho,
                                int epoch) {
    if (iff->ntree > 0) //if fixed tree chosen.
        iff->fixedTreeForest(epoch);
    else {
        if (rho < 0) {
            int treeRequired = iff->adaptiveForest(alpha, stopLimit);
            std::cout << "\n# of Tree required from k-agreement \n " << treeRequired;
        } else {
            const int initial_tree = 50;
            int treeRequired = iff->confTree(alpha, rho, initial_tree);
            std::cout << "\n# of Tree required from rho based stopping\n " << treeRequired;
        }

    }
}
/**
 *
 * @param iff : A pointer to a trained Forest object
 * @param test_dt - A Test data for scoring
 * @param oob : Use Out of bag estimate for scoring, boolean value True/False. Default False
 * @param savePathLength : Save pathLength of all trees
 * @param metadata : Column to include in the output file
 * @param outputName : Output file name.
 */
void scoreData(std::shared_ptr<Forest> iff,doubleframe* test_dt,bool oob,bool savePathLength,
               const ntstringframe* metadata, std::string outputName){
    std::vector<double> scores;
    std::vector<std::vector<double> > pathLength;
    if(oob){
        scores = iff->outOfBagScore(test_dt);
        pathLength = iff->oOBPathLength(test_dt);
    }else {
         scores= iff->AnomalyScore(test_dt); //generate anomaly score
         pathLength = iff->pathLength(
                test_dt); //generate Depth all points in all trees
    }
    saveScoreToFile(scores, pathLength, metadata, outputName, savePathLength);

}


bool Tree::rangeCheck;  //range check for Tree score calculation.
int util::debug;

int main(int argc, char *argv[]) {

    /*parse argument from command line*/
    parsed_args *pargs = parse_args(argc, argv);
    int seed = pargs->seed;
    util::debug = seed;
    util::initialize();
    ntstring input_name = pargs->input_name;
    ntstring output_name = pargs->output_name;
    ntstring test_name = pargs->test_name;
    ntstring load_forest = pargs->load_forest;
    ntstring save_forest = pargs->save_forest;

    d(int) *metacol = pargs->metacol;
    int ntree = pargs->ntrees;
    int nsample = pargs->sampsize;
    int maxheight = pargs->maxdepth;
    bool header = pargs->header;
    bool verbose = pargs->verbose;
    verbose = verbose; //Clears warning for now.
    bool rsample = nsample != 0;
    bool stopheight = maxheight != 0;
    int stopLimit = pargs->adaptive;
    bool rangecheck = pargs->rangecheck;
    bool rotate = pargs->rotate;
    bool pathlength = pargs->pathlength;
    float rho = pargs->precision;
    float alpha = pargs->alpha;
    int epoch = pargs->epoch;
    bool oob = pargs->oobag;
   
    //Input file to dataframe
    bool explanation = pargs->explanation;
    ntstringframe *csv = read_csv(input_name, header, false, false);
    ntstringframe *metadata = split_frame(ntstring, csv, metacol, true);
    doubleframe *dt = conv_frame(double, ntstring, csv); //read data to the global variable
    //Test file to data frame
    ntstringframe *csv_test = read_csv(test_name, header, false, false);
    metadata = split_frame(ntstring, csv_test, metacol, true);
    doubleframe *test_dt = dt;
    Tree::rangeCheck = rangecheck;

    // Check if test data is given
    if (test_name == input_name)
        test_dt = dt;
    else
        test_dt = conv_frame(double, ntstring, csv_test); //read data to the global variable

    nsample = nsample == 0 ? dt->nrow : nsample;


    std::shared_ptr<Forest> iff;
    //std::unique_ptr<Forest> iff;
    /**
     * Check if load or save option is triggered
     * Read model
     */
    if (load_forest != NULL) { //if filename given
        std::ifstream ifile{load_forest};
        {
            if (!ifile.is_open()) {
                throw std::runtime_error{" could not be opened"};
            }
            //cereal::JSONInputArchive iarchive{ifile};
            cereal::BinaryInputArchive iarchive(ifile); // Create an input archive
            iarchive(iff);
        }
    }
    else {
        if (rotate)
            iff = std::make_shared<RForest>(ntree, dt, nsample, maxheight, stopheight, rsample);
        else
            iff = std::make_shared<IsolationForest>(ntree, dt, nsample, maxheight, stopheight, rsample); //build iForest

        buildForest(iff, alpha, stopLimit, rho, epoch);
    }

    // Score using trained forest
    scoreData(iff, test_dt, oob, pathlength, metadata, output_name);
    
    // if explanation switch is choosen, output explanation for the test_dt. 
    if (explanation) {
        std::string explanationName = std::string(output_name) + "_explanation.csv";
        std::ofstream oexp(explanationName);
        iff->featureExplanation(test_dt, oexp);
        oexp.close();
    }

    // Save model

    if (save_forest != NULL) {
        std::string filenamex{save_forest};
        {
            std::ofstream file{filenamex};
            if (!file.is_open()) {
                throw std::runtime_error{filenamex + " could not be opened"};
            }
            //cereal::JSONOutputArchive archive{file};
            cereal::BinaryOutputArchive archive{file};
            archive(iff);
        }
    }
    return 0;
}












