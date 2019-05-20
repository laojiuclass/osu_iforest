Required  gcc verstion 4.7 or greater. Tested on gcc version 4.9

**Build command**

1. clone the repository `https://github.com/tadeze/osu_iforest`
3. create debug folder inside the repository `mkdir debug`
4. change directory to debug `cd debug`
5. type `cmake ..`
4. type `make`
5. run the iforest inside the debug folder `./iforest`

**Usage**

iforest.exe -h will display the available switches.
     Usage: iforest [options]

 * -i FILE, --infile=FILE  Specify path to input data file. (Required).
 * -o FILE, --outfile=FILE  Specify path to output results file. (Required).
 * -m COLS, --metacol=COLS  Specify columns to preserve as meta-data. (Separated by ',' Use '-' to specify ranges).
 * -t N, --ntrees=N   Specify number of trees to build. Default value is 100.
 * -s S, --sampsize=S  Specify subsampling rate for each tree. (Value of 0 indicates to use entire data set). Default value is 2048.
 * -d MAX, --maxdepth=MAX Specify maximum depth of trees. (Value of 0 indicates no maximum). Default value is 0.
 * -H, --header    Toggle whether or not to expect a header input. Default value is true.
 * -v, --verbose   Toggle verbose ouput. Default value is false.
 * -z --seed Specify integer value of random seed.
 * -h, --help  Print this help message and exit.
 * -k --oob Toggle whether or not to use out-of-bag estimate for depth. Default value is false. If `-k` specified score or depths will be from oout of bag trees.

**Example**

1. Running iforest to inputfile.csv and skip column 1 and 8-10

  `iforest.exe -i inputfile.csv -o score.csv -s 512 -t 100 -m 1,8-10`
2. This can also be written as

  `iforest.exe -iostm inputfile.csv score.csv 512 100 1,8-10`

### Status
[![Build Status](https://travis-ci.org/tadeze/osu_iforest.svg?branch=master)](https://travis-ci.org/tadeze/osu_iforest)


**Reference**
```
@misc{dietterich2018anomaly,
    title={Anomaly Detection in the Presence of Missing Values},
    author={Thomas G. Dietterich and Tadesse Zemicheal},
    year={2018},
    eprint={1809.01605},
    archivePrefix={arXiv},
    primaryClass={cs.LG}
}
```
