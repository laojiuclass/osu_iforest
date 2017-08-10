/*
 * IsolationForest.cpp
 *
 *  Created on: Mar, 2015
 *      Author: Tadeze
 */

#include "IsolationForest.hpp"

IsolationForest::IsolationForest(int _ntree, doubleframe *_df,
                                 int _nsample, int _maxheight, bool _stopheight, bool _rsample)
        : Forest(_ntree, _df, _nsample, _maxheight, _stopheight, _rsample) {


}

void IsolationForest::fixedTreeForest(int epoch) {
    if (epoch == 0)
        this->buildForest();
    else {
        int ntree_ep = batchForest(epoch);

        std::cout << "\n Number of trees required " << ntree_ep;
    }

}

int IsolationForest::batchForest(int epoch) {
    std::vector<int> sampleIndex;
    //
    //Based on the number of trees given and batch size (nsample) divide the data. Assume 1-epoch for now.
    // Let's assume the number of trees as epoch number in this case for now
    int ntrees = dataset->nrow / nsample;
    for (int ep = 0; ep < epoch; ep++) {

        sampleIndex.clear();
        getSample(sampleIndex, dataset->nrow, rsample, dataset->nrow);


        for (int n = 0; n < ntrees; n++) {

            //Sample and shuffle the data.
            //build tree

            auto tree = std::make_shared<Tree>();
            //	tree->rangeCheck = this->rangecheck;
            std::vector<int> batchIndex(sampleIndex.begin() + n * nsample, sampleIndex.begin() + (n + 1) * nsample);
            tree->iTree(batchIndex, dataset, 0, maxheight, stopheight);
            tree->trainIndex = batchIndex;
            this->trees.push_back(tree); //add tree to forest
            //	Tree::treeIndx++;
            batchIndex.clear();
        }

    }
    return (epoch * ntrees);
}



//Build forest using defined number of trees

void IsolationForest::buildForest() {

    std::vector<int> sampleIndex;
    //build forest through all trees
    for (int n = 0; n < this->ntree; n++) {

        //Sample and shuffle the data.
        sampleIndex.clear();
        getSample(sampleIndex, nsample, rsample, dataset->nrow);
        auto tree = std::make_shared<Tree>();
        tree->iTree(sampleIndex, dataset, 0, maxheight, stopheight);
        if (nsample < dataset->nrow)
            tree->trainIndex = sampleIndex;
        this->trees.push_back(tree);

    }
}
/*
 * @param alpha: anomaly proportion
 * @param stopLimit: Stoppig limit for growing treees
 */
int IsolationForest::adaptiveForest(double alpha, int stopLimit) {

    double tk = ceil(alpha * 2 * dataset->nrow);
    std::vector<int> sampleIndex(this->nsample);
    // std::cout<<tk<<std::endl;
    //logfile<<"point,tree,x1,x2\n";
    bool converged = false;
    int convCounter = 0;
    int ntree = 0;
    std::vector<int> topKIndex;
    std::vector<int> prevTopKIndex;
    std::vector<double> totalDepth(dataset->nrow, 0);
    double prob = 0.0;
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double> >, larger> pq;
    std::shared_ptr<Tree> tree;
    while (!converged) {
        pq = std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double> >, larger>();

        //get sample data
        getSample(sampleIndex, nsample, rsample, dataset->nrow);

        //Fill the sampleIndex with indices of the sample rotated data
        tree = std::make_shared<Tree>(); //new Tree();
        //    tree->rangeCheck = this->rangecheck;
        tree->iTree(sampleIndex, dataset, 0, maxheight, stopheight);
        this->trees.push_back(tree);
        ntree++;
        double d, dbar;
        topKIndex.clear();
        for (int inst = 0; inst < dataset->nrow; inst++) {
            d = this->getdepth(dataset->data[inst], tree);
            totalDepth[inst] += d;
            dbar = totalDepth[inst] / ntree;
            pq.push(std::pair<int, double>(inst, dbar));
        }
        //util::logfile<<ntree<<",";
        for (int i = 0; i < tk; i++) {
            topKIndex.push_back(pq.top().first);
            //  	util::logfile<<pq.top().first<<",";
            pq.pop();

        }

        if (ntree < 2) {
            prevTopKIndex = topKIndex;
//            util::logfile<<prob<<"\n";
            continue;
        }

        prob = topcommonK(topKIndex, prevTopKIndex);
        //util::logfile<<prob<<"\n";
        prevTopKIndex = topKIndex;
        if (prob == 1)
            convCounter++;
        else
            convCounter = 0;
        converged = convCounter > stopLimit;


        if (ntree < 50)
            converged = false;

    }


    return ntree;
}

struct smaller {
    bool operator()(const std::pair<int, double> p1, const std::pair<int, double> p2) {
        return p1.second > p2.second;
    }
};

/*
int IsolationForest::confTree(double alpha, double rho, int init_tree) {
    //Build the RForest model
    double tk = ceil(alpha * dataset->nrow);
    std::vector<int> sample_index(this->nsample);

    bool converged = false;
    double ntree = 0;
    std::vector<double> total_depth(dataset->nrow, 0);
    std::vector<std::vector<double> > exp_depth(dataset->nrow, std::vector<double>(0));
    std::priority_queue<std::pair<int, double>,
            std::vector<std::pair<int, double> >, smaller> pq; //holds top shallowest depth

    double current_depth, dbar; //estimated depth

    while (!converged) {
        pq = std::priority_queue<std::pair<int, double>,
                std::vector<std::pair<int, double> >, smaller>();

        //get sample data
        getSample(sample_index, nsample, rsample, dataset->nrow);

        //Fill the sampleIndex with indices
        Tree *tree = new Tree();
        tree->iTree(sample_index, dataset, 0, maxheight, stopheight);
        this->trees.push_back(tree);
        ntree++;

        for (int inst = 0; inst < dataset->nrow; inst++) {
            current_depth = this->getdepth(dataset->data[inst], tree);
            total_depth[inst] += current_depth;
            dbar = total_depth[inst] / ntree;
            pq.push(std::pair<int, double>(inst, dbar)); //keep track of top k instance
            exp_depth[inst].push_back(dbar);  //store expected depth of instance
        }


        //########### Starts after init_tree number of trees ####################
        if (ntree < init_tree)   //star confidence interval after t trees
            continue;
        //util::logfile<<ntree<<",";
        std::vector<std::pair<int, double> > t_inter;
        for (int i = 0; i < tk; i++) {
            int _inst = pq.top().first;
            double _depth = util::tconf(exp_depth[_inst], 0.95);
            t_inter.push_back(std::pair<int, double>(_inst, _depth));
            //util::tconf( _depth )); //exp_depth[_inst], 0.95))); //t-confidence of top k
            pq.pop();

        }

        // top k lower interval depth score
        double dklower_score = util::score(
                exp_depth[t_inter[tk - 1].first][ntree - 1] - t_inter[tk - 1].second,
                this->nsample);  //bug here
        int counter = 0;
        for (int i = tk - 1; i >= 0; i--) {

            double score_depth = exp_depth[t_inter[i].first][ntree - 1] + t_inter[i].second;

            double d_upper_score = util::score(score_depth, this->nsample);

            if (d_upper_score < (1 - rho) * dklower_score)
                break;
            counter++;

        }

        if (counter == tk) //all upper confidence depth score are greater than kth depth lower score
            converged = true;

    }
    //increase number of trees
    return ntree;
}*/
