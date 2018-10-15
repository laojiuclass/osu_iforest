#include "tree.hpp"
#include "gtest/gtest.h"
#include "cincl.hpp"

class TreeTest : public ::testing::Test
{
protected:
    std::shared_ptr<Tree> tr;
    std::shared_ptr<util::dataset> dataset;


     virtual void SetUp() {
    	 //Let read data from
         std::string filename("/home/tadeze/projects/iForestCodes/pyiForestCmake/test/unittest/synth2d.dt");
        // std::string filename("test2d.csv");
         //std::string filename = common::filename();

         std::vector<std::vector<double> > data = util::readcsv((char*) &filename[0],',',true);
    	 dataset = common::makeDataset(data);
    	 tr = std::make_shared<Tree>();
   		std::vector<int> dataIndex;//(dataset->nrow);
   		for(int i=0;i<dataset->nrow;i++)
            dataIndex.push_back(i);
   		tr->iTree(dataIndex,dataset,0,0,false);
     }

     virtual void TearDown() {
       // delete dataset;
        //delete tr;
     }
};


TEST_F(TreeTest,makeDataset){  //Check dimension of the data
 ASSERT_EQ(dataset->ncol,2);
 ASSERT_EQ(dataset->nrow,105);
}

TEST_F(TreeTest, treeCreation){
ASSERT_GT(tr->maxTreeDepth(),6); //check ok for now.
}

TEST_F(TreeTest,pathLength){
 double depth = tr->pathLength(dataset->data[8]);
 //double depth2 = tr->featureContribution(dataset->data[8]);
 EXPECT_GT(depth,1);
 EXPECT_LT(depth,30);
 //EXPECT_EQ(depth,depth2);
}

TEST_F(TreeTest,featurecontribution){
    //double depth = tr->pathLength(dataset->data[8]);
    auto depth2 = tr->featureContribution(dataset->data[8]);
    //for(const auto& mp : depth2.featureContribution())
      EXPECT_EQ(depth2.featureContribution()[2],2);
    //EXPECT_GT(depth,10);
    //EXPECT_LT(depth,30);
   // EXPECT_EQ(depth,depth2);
}
TEST_F(TreeTest,Treerange){
	bool rangecheck = tr->rangeCheck;
	ASSERT_TRUE(rangecheck);
}

//Check the anomaly has smaller depth
TEST_F(TreeTest,compareDepth){
 std::vector<double> alldepth;
 for(int i=0;i<dataset->nrow;i++)
 alldepth.push_back(tr->pathLength(dataset->data[i]));
 //auto larg = std::max_element(alldepth.begin(),alldepth.end());
 //auto small = std::min_element(alMdepth.begin(),alldepth.end());
 std::sort(alldepth.begin(),alldepth.end());

 EXPECT_LT(alldepth[0],20);
 EXPECT_EQ(alldepth[1],3);
 EXPECT_GT(alldepth[50],5);
//EXPECT_EQ(tr->pathLength(dataset->data[2]),tr->featureContribution(dataset->data[2]));
}
