#ifndef __NTT_HH_
#define __NTT_HH_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "vecutil.hh"
#include "primitive.hh"

using namespace std;

extern int* mod;
extern int* invmod;
extern int* modPower;
extern int* invModPower;
extern int modNum;
extern int root;
extern int invRoot;
extern int rootLength;
extern int invRootLength;

void msg1();
int findInvRoot();
int findInvLength();
int processModulo(int, int, int);
int* execNTT(int*, int);
int* execInvNTT(int*);
long* execInvNTT(long*);
string generateNumber(int*);
string generateNumber(long*);
void printModMatrix();
void printInvModMatrix();
int ssaConversion(vector<int>&, int*);
int ssaConversion(const char*, int*);

#endif
