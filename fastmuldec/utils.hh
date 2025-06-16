// Copyright [2018] by Muhammad Ahsan
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace std::chrono;

void printMsg(string);
int readPrevious(vector<int> *output, string file);
void printVector(vector<int>*);