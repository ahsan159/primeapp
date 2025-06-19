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
void printVector(vector<int>*);
void printVector(vector<uint16_t>*);
int readPrevious(vector<int> *output, string file);
int readPrevious(vector<uint16_t> *output, string file);
int remainder(vector<uint16_t>* dividend, vector<uint16_t>* divisor);
int compare(vector<uint16_t>* a, vector<uint16_t>* b);
int removeZero(vector<uint16_t>* vec);