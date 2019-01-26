#!/bin/sh
#
# simple tests for nproc.  needs to be manually checked.
#
# first test cc with all warnings
#
unset OMP_NUM_THREADS
unset OMP_THREAD_LIMIT
echo "----------------------------------------------------------------------"
echo "Build with all warnings, ignore copyright and sccsid unused variables."
echo "Also ignore errors and warnings outside nproc.c"
echo "----------------------------------------------------------------------"
cc -Wall -ansi -pedantic -o nproc nproc.c 
echo " "
echo "---------------------"
echo "make clean, then make"
echo "---------------------"
make clean
make 
echo " "
echo "------------------------"
echo "display man page to view"
echo "------------------------"
mandoc nproc.1
echo " "
echo "-----------"
echo "Default run"
echo "-----------"
./nproc
echo " "
echo "------------------------"
echo "Test -a and --all option"
echo "------------------------"
./nproc -a
echo "------------------------"
./nproc --all
echo " "
echo "---------------------------------------------------"
echo "run, ignore one processor, both -i N and --ignore=N"
echo "---------------------------------------------------"
./nproc -i 1
echo "---------------------------------------------------"
./nproc --ignore=1
echo "------------------------------------------------------------------"
echo "run, ignore 500 processor, both -i N and --ignore=N.  Should get 1"
echo "-----------------------------------------------------------------"
./nproc -i 500
echo "------------------------------------------------------------------"
./nproc --ignore=500
echo " "
echo "-----------------------"
echo "run, both -h and --help"
echo "-----------------------"
./nproc -h
echo "-----------------------"
./nproc --help
echo " "
echo "--------------------------"
echo "run, both -v and --version"
echo "--------------------------"
./nproc -v
echo "--------------------------"
./nproc --version
echo " "
echo "---------------------------------------------------------"
echo "ERROR: run, ignore -1 processor, both -i N and --ignore=N"
echo "---------------------------------------------------------"
./nproc -i -1
echo "---------------------------------------------------------"
./nproc --ignore=-1
echo " "
echo "---------------------------------------------------------"
echo "ERROR: run, ignore "z" processors, both -i N and --ignore=N"
echo "---------------------------------------------------------"
./nproc -i z
echo "---------------------------------------------------------"
./nproc --ignore=z
echo " "
echo "----------------------------------------"
echo "ERROR: run, bad flag, both -f and --flag"
echo "----------------------------------------"
./nproc -f
echo "----------------------------------------"
./nproc --flag
echo " "
echo "---------------------------------------------------------"
echo "ERROR: run, bad flag, both -f N and --flag=N"
echo "---------------------------------------------------------"
./nproc -f 4
echo "---------------------------------------------------------"
./nproc --flag=4
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run, OMP_NUM_THREADS=1, OMP_THREAD_LIMIT=1"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=1
export OMP_THREAD_LIMIT=1
./nproc
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run -a, OMP_NUM_THREADS=1, OMP_THREAD_LIMIT=1"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=1
export OMP_THREAD_LIMIT=1
./nproc -a
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run --all, OMP_NUM_THREADS=1, OMP_THREAD_LIMIT=1"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=1
export OMP_THREAD_LIMIT=1
./nproc --all
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run -a, OMP_NUM_THREADS=1, OMP_THREAD_LIMIT=2"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=1
export OMP_THREAD_LIMIT=2
./nproc -a
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run, OMP_NUM_THREADS=1, OMP_THREAD_LIMIT=5"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=1
export OMP_THREAD_LIMIT=5
./nproc
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run, OMP_NUM_THREADS=5, OMP_THREAD_LIMIT=5"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=5
export OMP_THREAD_LIMIT=5
./nproc
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run -i 2, OMP_NUM_THREADS=2, OMP_THREAD_LIMIT=2"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=2
export OMP_THREAD_LIMIT=2
./nproc -i 2
echo " "
echo "---------------------------------------------------------"
echo "OpenMP: run -i 1, OMP_NUM_THREADS=2, OMP_THREAD_LIMIT=3"
echo "---------------------------------------------------------"
export OMP_NUM_THREADS=2
export OMP_THREAD_LIMIT=3
./nproc -i 1
echo " "
echo "----------"
echo "make clean"
echo "----------"
make clean
echo "-----------------"
echo "make pdf man page"
echo "-----------------"
mandoc -T pdf nproc.1 >nproc.1.pdf
echo " "
echo "******************************* Complete *******************************"
unset OMP_NUM_THREADS
unset OMP_THREAD_LIMIT