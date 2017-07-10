


echo "Build command for iforest"


SERVER=`/bin/hostname|/bin/awk -F. '{print $2}'|tr "[a-z]" "[A-Z]"` 
echo $SERVER
if [[ $SERVER == "HPC" || $SERVERi == "EECS" ]]; then
# use the make file to compile it 
make clean
make

else   



	if [ $# -eq 0 ]
	then
		echo "No argument supplied. Defualt build command will be in release mode."
		
		buildType='Release'
	else
		buildType=$1
	fi
	echo $buildType
	if [[ $buildType == "Release" || $buildType == "Debug" ]]
	then
			if [ ! -d ./$buildType ]
			then
				mkdir $buildType
			fi
			cd $buildType
			echo "Building in $buildType mode"

		#export CC=/usr/local/common/gcc-6.3.0/bin/gcc
		#export CXX=/usr/local/common/gcc-6.3.0/bin/g++
		cmake .. -DCMAKE_BUILD_TYPE=$buildType 
		make
	else
			echo " Please type either Release or Debug as an argument."
	fi
fi
