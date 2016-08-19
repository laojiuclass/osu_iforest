#include "argparse_iforest.h"

#define NOPTS 17
#define IOPT 0
#define OOPT 1
#define MOPT 2
#define TOPT 3
#define SOPT 4
#define DOPT 5
#define HOPT 6
#define VOPT 7
#define AOPT 8
#define ROPT 9
#define COPT 10
#define POPT 11
#define XOPT 12
#define GOPT 13
#define JOPT 14
#define QOPT 15
#define EOPT 16
d(option)* option_spec() {
    d(option)* opts = vecalloc(option,NOPTS);
    opts[IOPT] = (option){
        .sarg = 'i',
        .larg = "infile",
        .name = "FILE",
        .desc = "Specify path to input data file. (Required).",
        .default_value = NULL,
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[OOPT] = (option){
        .sarg = 'o',
        .larg = "outfile",
        .name = "FILE",
        .desc = "Specify path to output results file. (Required).",
        .default_value = NULL,
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[XOPT] = (option){
        .sarg = 'x',
        .larg = "testfile",
        .name = "FILE",
        .desc = "Specify path to test file. (optional).",
        .default_value = NULL,
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
  
    opts[MOPT] = (option){
        .sarg = 'm',
        .larg = "metacol",
        .name = "COLS",
        .desc = "Specify columns to preserve as meta-data. (Separated by ',' Use '-' to specify ranges).",
        .default_value = NULL,
        .value = NULL,
        .isflag = false,
        .flagged = false
    };

    opts[TOPT] = (option){
        .sarg = 't',
        .larg = "ntrees",
        .name = "N",
        .desc = "Specify number of trees to build.(Value 0 indicates to use adaptive tree growing) ",
        .default_value = "100",
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[SOPT] = (option){
        .sarg = 's',
        .larg = "sampsize",
        .name = "S",
        .desc = "Specify subsampling rate for each tree. (Value of 0 indicates to use entire data set).",
        .default_value = "2048",
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[DOPT] = (option){
        .sarg = 'd',
        .larg = "maxdepth",
        .name = "MAX",
        .desc = "Specify maximum depth of trees. (Value of 0 indicates no maximum).",
        .default_value = "0",
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[HOPT] = (option){
        .sarg = 'H',
        .larg = "header",
        .name = NULL,
        .desc = "Toggle whether or not to expect a header input.",
        .default_value = "true",
        .value = NULL,
        .isflag = true,
        .flagged = true
    };
    opts[VOPT] = (option){
        .sarg = 'v',
        .larg = "verbose",
        .name = NULL,
        .desc = "Toggle verbose output.",
        .default_value = "false",
        .value = NULL,
        .isflag = true,
        .flagged = false
    };

    opts[AOPT] = (option){
           .sarg = 'a',
           .larg = "adaptive",
           .name = NULL,
           .desc = "Number of common successive top K for adaptive process.",
           .default_value = "5",
           .value = NULL,
           .isflag = false,
           .flagged = false
       };
    opts[ROPT] = (option){
           .sarg = 'r',
           .larg = "rotate",
           .name = NULL,
           .desc = "Toggle whether to rotate data or not.",
           .default_value = "false",
           .value = NULL,
           .isflag = true,
           .flagged = false
       };

     opts[POPT] = (option){
           .sarg = 'p',
           .larg = "pathlength",
           .name = NULL,
           .desc = "Toggle whether to display  depth of all trees (Default is false)",
           .default_value = "false",
           .value = NULL,
           .isflag = true,
           .flagged = false
       };
    
    opts[COPT]=(option){
        .sarg='c',
        .larg="columns",
        .name="N",
        .desc="specify number of columns to use.",
        .default_value="0",
        .value=NULL,
        .isflag=false,
        .flagged=false

    };
    opts[GOPT] = (option){
           .sarg = 'g',
           .larg = "rangecheck",
           .name = NULL,
           .desc = "Toggle whether to use range check or not during testing (Default false) ",
           .default_value = "false",
           .value = NULL,
           .isflag = true,
           .flagged = false
       };

    opts[JOPT] = (option){
        .sarg = 'j',
        .larg = "precision",
        .name = "P",
        .desc = "Specify rho precision confidence interval for stopping criteria Value (0.01 to 0.08) works",
        .default_value = "-3.0",
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
    opts[QOPT] = (option){
            .sarg = 'q',
            .larg = "alpha",
            .name = "Q",
            .desc = "Anomaly proportion. Default 0.01",
            .default_value = "0.01",
            .value = NULL,
            .isflag = false,
            .flagged = false
        };
  opts[EOPT] = (option){
        .sarg = 'e',
        .larg = "epoch",
        .name = "E",
        .desc = "Specify number of epoch when batch mode training is selected. Default 30",
        .default_value = "0",
        .value = NULL,
        .isflag = false,
        .flagged = false
    };
     
    return opts;
}

parsed_args* validate_args(d(option*) opts) {
    parsed_args* pargs = talloc(parsed_args,1);
    pargs->input_name = opts[IOPT].value;
    if (pargs->input_name==NULL) err_and_exit(1,"Must specify path to input with option -i/--infile.\n");
    pargs->output_name = opts[OOPT].value;
    if (pargs->output_name==NULL) err_and_exit(1,"Must specify path to output with option -o/--outfile.\n");
  
    pargs->test_name = opts[XOPT].value; //set test file if available
   if(pargs->test_name==NULL)pargs->test_name = opts[IOPT].value;  //if not specified set test file to input file
    if (opts[MOPT].value) {
        pargs->metacol = parse_multi_ints(opts[MOPT].value);
        if (pargs->metacol==NULL) {
            err_and_exit(1,"Invalid specification of meta columns.");
        }
        for_each_in_vec(i,cn,pargs->metacol,(*cn)--;)
    }
    
    if (str_conv_strict(&(pargs->ntrees),int,opts[TOPT].value)) {
        err_and_exit(1,"Expected integer as number of trees.\n");
    }
  
    if (pargs->ntrees<0) {
        err_and_exit(1,"Number of trees must be at least 1.\n");
    }
    if (str_conv_strict(&(pargs->sampsize),int,opts[SOPT].value)) {
        err_and_exit(1,"Expected integer as sample size.\n");
    }
    if (pargs->sampsize<3&&pargs->sampsize!=0) {
        err_and_exit(1,"Sample size must be at least 3.\n");
    }
    if (str_conv_strict(&(pargs->maxdepth),int,opts[DOPT].value)) {
        err_and_exit(1,"Expected integer as maximum depth.\n");
    }
    if (pargs->maxdepth<0) {
        err_and_exit(1,"Maximum depth can't be negative.\n");
    }
   if (str_conv_strict(&(pargs->adaptive),int,opts[AOPT].value)) {
        err_and_exit(1,"Expected integer as stopping Limit.\n");
    }
    
    
     if (str_conv_strict(&(pargs->epoch),sint,opts[EOPT].value)) {
       err_and_exit(1,"Expected integer as number of epoch.\n");
       }
   
   if (pargs->epoch<0) {
        err_and_exit(1,"Number of epoch must be at least 1.\n");
        }
  
    
    pargs->sampsize = strtol(opts[SOPT].value,NULL,10);
    pargs->maxdepth = strtol(opts[DOPT].value,NULL,10);
    pargs->header = opts[HOPT].flagged;
    pargs->verbose = opts[VOPT].flagged;
    pargs->adaptive =  strtol(opts[AOPT].value,NULL,10);  //added by Tadesse
    pargs->columns = strtol(opts[COPT].value,NULL,10);
    pargs->rotate=opts[ROPT].flagged;
    pargs->pathlength = opts[POPT].flagged;
    pargs->rangecheck = opts[GOPT].flagged;  
    pargs->precision = strtof(opts[JOPT].value,NULL);
    pargs->alpha= strtof(opts[QOPT].value,NULL);
    
    //strtof();
    return pargs;
}
