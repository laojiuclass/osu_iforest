/*
 * Tree.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: Tadeze
 */
#include "tree.hpp"

// Check if the index is avaialbe in training the tree
// Used for out of bag scoring 

bool Tree::indexAvailable(int index) {
    return std::find(trainIndex.begin(), trainIndex.end(), index) != trainIndex.end();

}

//bool Tree::rangeCheck=true;
void Tree::iTree(std::vector<int> const &dIndex, const doubleframe *dt, int height, int maxheight, bool stopheight) {
    this->depth = height; //Tree height
    // Set size of the node
    nodeSize = dIndex.size();
    //stop growing if condition
    if (dIndex.size() <= 1 || (stopheight && this->depth > maxheight)) {
        //this->isLeaf = true;
        return;
    }
    //*** Need modification
    //Initialize minmax array for holding max and min of an attributes
    std::vector<std::vector<double> > minmax;
    std::vector<double> tmp;

    for (int j = 0; j < dt->ncol; j++) {
        tmp.push_back(dt->data[dIndex[0]][j]);
        tmp.push_back(dt->data[dIndex[0]][j]);
        minmax.push_back(tmp); //initialize max and min to random value
        tmp.clear();
    }

    //Compute max and min of each attribute
    for (unsigned i = 0; i < dIndex.size(); i++) {
        //vector<double> inst = data->data[i];
        for (int j = 0; j < dt->ncol; j++) {
            if (dt->data[dIndex.at(i)][j] < minmax[j].at(0))
                minmax[j].at(0) = dt->data[dIndex.at(i)][j];
            if (dt->data[dIndex.at(i)][j] > minmax.at(j).at(1))
                minmax[j].at(1) = dt->data[dIndex.at(i)][j];
        }

    }

    //use only valid attributes
    std::vector<int> attributes;
    for (int j = 0; j < dt->ncol; j++) {
        if (minmax[j][0] < minmax[j][1]) {
            attributes.push_back(j);
        }
    }
    //return if no valid attribute found
    if (attributes.size() == 0)
        return;
    //Randomly pick an attribute and a split point

    //int randx = randomI(0, attributes.size());

    this->splittingAtt = attributes[util::randomI(0, attributes.size() - 1)]; //randx];
    this->splittingPoint = util::randomD(minmax[this->splittingAtt][0], minmax[this->splittingAtt][1]);
    this->minAttVal = minmax[this->splittingAtt][0];
    this->maxAttVal = minmax[this->splittingAtt][1];
    std::vector<int> lnodeData;
    std::vector<int> rnodeData;
    //Split the node into two
    for (unsigned i = 0; i < dIndex.size(); i++) {
        if (dt->data[dIndex.at(i)][splittingAtt] >= splittingPoint &&
            (dt->data[dIndex.at(i)][splittingAtt] != minmax[this->splittingAtt][0])) {
            rnodeData.push_back(dIndex.at(i));
        } else {
            lnodeData.push_back(dIndex.at(i));
        }
    }
    leftChild = std::make_shared<Tree>();// new Tree(); //&dataL,height+1,maxheight);
    //leftChild->parent = this->shared_from_this();
    leftChild->iTree(lnodeData, dt, this->depth + 1, maxheight, stopheight);

    rightChild = std::make_shared<Tree>();//new Tree(); //&dataR,height+1,maxheight);
    //rightChild->parent = this->shared_from_this();
    rightChild->iTree(rnodeData, dt, this->depth + 1, maxheight, stopheight);

}


/*
 * takes instance as vector of double
 */
double Tree::pathLength(double *inst) {

    if (this->leftChild == nullptr || this->rightChild == nullptr) { ///referenced as null for some input data .
        return util::avgPL(this->nodeSize);
    }
    // Check range of input data
    double instAttVal = inst[this->splittingAtt];
    if (Tree::rangeCheck) {

        if (instAttVal < this->minAttVal && util::randomD(instAttVal, this->minAttVal) < this->minAttVal)
            return 1.0;
        if (instAttVal > this->minAttVal && util::randomD(instAttVal, this->maxAttVal) > this->maxAttVal)
            return 1.0;

    }

    //Logging the isolation process
    //	logfile<<tmpVar<<","<<this->splittingAtt<<","<<this->splittingPoint<<"\n";
    if (instAttVal >= this->splittingPoint) {


        return this->rightChild->pathLength(inst) + 1.0;

    } else {
        return this->leftChild->pathLength(inst) + 1.0;
    }
}

int Tree::getNodeSize() const {
    return nodeSize;
}

int Tree::getSplittingAtt() const {
    return splittingAtt;
}

double Tree::getSplittingPoint() const {
    return splittingPoint;
}

int Tree::getDepth() const {
    return depth;
}

double Tree::getMinAttVal() const {
    return minAttVal;
}

double Tree::getMaxAttVal() const {
    return maxAttVal;
}

/*
 * Change direction whenever feature f encountered
 *
double Tree::fPathLength(double *inst,int f)
{


return 1.0;

}
*/
//std::vector<std::vector<double>>
//std::map<int,double>
struct Contrib Tree::featureContribution(double *inst) const { //std::vector<double> &inst){

    auto root = this->shared_from_this();
    double instAttVal;
    double depth = 0.0;
    Contrib contribution(sizeof(inst) / sizeof(double) - 1);
    while ((root->rightChild != nullptr) || (root->leftChild != nullptr)) {
        instAttVal = inst[root->splittingAtt];

        //contributions[root->splittingAtt] = depth + util::avgPL(root->nodeSize);

        if (instAttVal >= root->splittingPoint)
            root = root->rightChild;
        else
            root = root->leftChild;
        depth = depth + 1.0;
        if (root->splittingAtt != -1)
            contribution.addcont(root->splittingAtt, depth + util::avgPL(root->nodeSize));

    }
    return contribution;//.featureContribution();

}

/*
 *  Serialize Tree into output stream
 *
 */

/*
const int NULL_TREE_CHILD = -999;
void Tree::serialize(std::ostream &s) const {

	// Define empty queute
	std::queue<const Tree*> qtree ;
	qtree.push(this);
	int i=0;
	while(!qtree.empty()) {
		const Tree* nextTree = qtree.front();
		qtree.pop();
		if(nextTree==NULL){
			//j["depth"] =  NULL_TREE_CHILD_DEPTH;
			s<<" "<< NULL_TREE_CHILD;
		}
		else {

			s<<" "<<nodeSize;
			s<<" "<<splittingAtt;
			s<<" "<<splittingPoint;
			s<<" "<<depth;
			s<<" "<<minAttVal;
			s<<" "<<maxAttVal;
			/*j["depth"] = nextTree->depth;
			j["splittingAtt"] = nextTree->splittingAtt;
			j["splittingPoint"] = nextTree->splittingPoint;
			j["depth"]= nextTree->depth;
			j["nodesize"]=nextTree->nodeSize;
			j["minAttVal"] = nextTree->minAttVal;
			j["maxAttVal"] = nextTree->maxAttVal;

			qtree.push(nextTree->leftChild);
			qtree.push(nextTree->rightChild);

		}

		i++;
	}
}
void Tree::deserialize(std::istream &s) const {

	std::queue<Tree*> qTree;
	int checkTree;
	Tree* root = assignTree((Tree*)this,s);
	if(root == NULL)
		return;
	qTree.push(root);
	while(!qTree.empty() && s!= nullptr){

		Tree* node = qTree.front();
		qTree.pop();
		//node->leftChild = new Tree();
		if(assignTree(node->leftChild,s)!=NULL) {
			qTree.push(node->leftChild);
		}
		if(assignTree(node->rightChild,s)!=NULL) {

			qTree.push(node->rightChild);

		}

	}

}

Tree* Tree::assignTree(Tree *tr, std::istream &s) const {
	int tempnodesize;
	s >> tempnodesize;
	if(tempnodesize == NULL_TREE_CHILD) {
		tr = NULL;

	}else {
		tr = new Tree();
		tr->nodeSize = tempnodesize;
		s >> tr->splittingAtt; // = (*rtree)["splittingAtt"];
		s >> tr->splittingPoint; //= (*rtree)["splittingPoint"];
		s >> tr->depth;
		s >> tr->minAttVal; //  = (*rtree)["minAttVal"];
		s >> tr->maxAttVal; // = (*rtree)["maxAttVal"];
	}
	return tr;



}

*/
/*
	while(iNode<numNodes){
		if(iNode==0){  //root node
			root = this;//new Tree();
			assignTree(root, &jsontree[iNode]);
			qTree.push(root);
			iNode++;
		}
		else {
			Tree* node = qTree.front();
			qTree.pop();
			json* jleft = &jsontree[iNode];//ootTree[iNode];
			json* jright=NULL;

			if(iNode<(numNodes-1))
				jright =&jsontree[iNode+1];

			if(jleft!=NULL && (*jleft)["depth"]>0) {
				node->leftChild = new Tree();
				assignTree(node->leftChild,jleft);
				qTree.push(node->leftChild);
			}
			if(jright!=NULL && (*jright)["depth"]>0){
				node->rightChild = new Tree();
				assignTree(node->rightChild,jright);
				qTree.push(node->rightChild);
			}
			iNode +=2;
		}
	}

}




*/















