echo "This is used for building under windows os"
mkdir build 
cd build 
cmake .. -G "MinGW Makefiles"
mingw32-make.exe
echo "build completed"
