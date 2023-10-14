#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iterator>
#include <atomic>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <map>
#include <climits>
#include <cmath>
#include <omp.h>
#include <csignal>
#include <unistd.h>

// #include "vecutil.hh"

using namespace std;
using namespace std::chrono;

int readPrevious(vector<int>*, string);
void printHex(vector<int>*);
void printHex(vector<int>*, const char*);
void printHexFile(vector<int>*, const char*);
void getDecimalVector(vector<int>*,vector<unsigned int>*);
void printDecimalVectorFile(vector<unsigned int>*,const char*);
void printDecimalVector(vector<unsigned int>*);
void addNumbers(vector<unsigned int>&, vector<unsigned int>&, vector<unsigned int>&);