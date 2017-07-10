#ifndef ARGPARSE_IFOREST
#define ARGPARSE_IFOREST

#include "argparse.h"

struct parsed_args {
    char* input_name;
    char* output_name;
    char* test_name;
    d(int*) metacol;
    int ntrees;
    int sampsize;
    int maxdepth;
    bool header;
    int adaptive;
    bool rotate;
    bool pathlength;
    int columns;
    bool verbose;
    bool rangecheck;
    float precision;
    float alpha;
    int epoch;
    int seed;
    bool explanation;
};


#endif
