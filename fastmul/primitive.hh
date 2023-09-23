#ifndef __PRIMITIVE_HH_
#define __PRIMITIVE_HH_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

extern int numberInProcess;
extern int maxRootOrder;
extern vector<int> rootList;
extern vector<int> orderList;

void msg();
int isRoot(int);
int rootOrder(int);
int isPrimitive(int);
int processNumber(int);
int findAllRoots();
vector<int> readNumber(string);
vector<int> modulo(vector<int>&, vector<int>&);
int inverse(int);

#endif
