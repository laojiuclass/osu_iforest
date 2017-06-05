
echo "Build command for iforest"
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
		cmake -DCMAKE_BUILD_TYPE=$buildType ..
		make
else
		echo " Please type either Release or Debug as an argument."
fi

