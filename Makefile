CC=/usr/local/common/gcc-4.9.0/bin/gcc
CFLAGS=--std=c99 -D_GNU_SOURCE -Wall -Werror -g
<<<<<<< HEAD
PP=g++
PFLAGS= --std=c++0x -Wall -Werror -g
=======
PP=/usr/local/common/gcc-4.9.0/bin/g++
PFLAGS=--std=c++11 -Wall -Werror -g
LDFLAGS=-Wl,-rpath,/usr/local/common/gcc-4.9.0/lib64/
>>>>>>> 4a9c1790a33a07c45c2ccccc75964a0affe71ecf

all: iforest.exe

C/%.o: C/%.c C/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

cincl.o: C/common.o C/object.o C/strfun.o C/readwrite.o C/argparse.o C/argparse_iforest.o C/frames.o
	ld -r C/common.o C/object.o C/strfun.o C/readwrite.o C/argparse.o C/argparse_iforest.o C/frames.o -o cincl.o

%.o: %.cpp %.hpp
	$(PP) $(PFLAGS) $(LDFLAGS) -c $< -o $@
iforest.exe: cincl.o IsolationForest.o Tree.o utility.o main.o
	$(PP) $(PFLAGS) $(LDFLAGS) -o iforest.exe cincl.o IsolationForest.o Tree.o utility.o main.o
fresh:
	make clean
	make all

clean:
	rm -rf *.o*
	rm -rf *.exe
	rm -rf C/*.o*
	rm -rf C/*.exe
