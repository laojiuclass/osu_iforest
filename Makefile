CC=gcc
CFLAGS=--std=c99 -D_GNU_SOURCE -Wall -Werror -g
PP=/usr/local/common/gcc-4.9.0/bin/g++
PFLAGS=--std=c++11 -Wall -Werror -g
LDFLAGS=-Wl,-rpath,/usr/local/common/gcc-4.9.0/lib64/
all: iforest

C/%.o: iforestlib/C/%.c iforestlib/C/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
cincl.o: C/common.o C/object.o C/strfun.o C/readwrite.o C/argparse.o C/argparse_iforest.o C/frames.o
	ld -r C/common.o C/object.o C/strfun.o C/readwrite.o C/argparse.o C/argparse_iforest.o C/frames.o -o cincl.o

%.o: iforestlib/%.cpp iforestlib/%.hpp
	$(PP) $(PFLAGS) $(LDFLAGS) -c $< -o $@
main.o: main.cpp iforestlib/main.hpp
	$(PP) $(PFLAGS) $(LDFLAGS) -c $< -o $@

iforest: cincl.o Forest.o RForest.o IsolationForest.o Tree.o utility.o main.o
	$(PP) $(PFLAGS) $(LDFLAGS) -o iforest cincl.o IsolationForest.o Tree.o utility.o Forest.o RForest.o main.o
fresh:
	make clean
	make all

clean:
	rm -rf *.o*
	rm -rf *.exe
	rm -rf C/*.o*
	rm -rf C/*.exe
