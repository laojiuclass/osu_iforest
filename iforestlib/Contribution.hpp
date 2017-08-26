//
// Created by tadeze on 8/6/17.
//

#ifndef OSU_IFOREST_CONTRIBUTION_HPP
#define OSU_IFOREST_CONTRIBUTION_HPP

#include <vector>
#include <map>

struct Contrib{
    int feature;
    std::map<int,std::vector<double> > contributions;
    Contrib(int ncols):feature(ncols) { //initialize all features.
        for(int i=0;i<feature;i++) {
            std::vector<double> contr;
            contr.push_back(0.0);
            contributions.insert({i, contr});

        }
    }

    void addcont(int index, double depth){
        if(contributions.count(index)<1) {
            std::vector<double> contr;
            contr.push_back(depth);
            contributions.insert({index,contr});
        }
        else{
            contributions[index].push_back(depth);
        }
    }
    std::map<int,double> featureContribution(){
        std::map<int,double> explanation;


        for(const auto & contr : contributions){
            double expl=0.0;
            expl = 1.0/contr.second[0];
            //for(auto depth : contr.second)
            //   expl += 1.0/depth;
            explanation.insert({contr.first,expl});

        }
        return explanation;
    }

};
//typedef  struct Contrib contrib;
using contrib = struct Contrib;
#endif //OSU_IFOREST_CONTRIBUTION_HPP
