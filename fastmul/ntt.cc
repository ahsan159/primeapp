// Copyright [2018] by Muhammad Ahsan
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "vecutil.hh"
#include "primitive.hh"
#include "ntt.hh"
#include <omp.h>


using namespace std;
using namespace std::chrono;


void msg1()
{
  cout << "This is the msg" << endl;
  return;
}

int* mod = NULL;
int* invmod = NULL;
int* modPower = NULL;
int* invModPower = NULL;
int modNum = -1;
int root = -1;
int invRoot = -1;
int rootLength = -1;
int invRootLength = -1;

int processModulo(int length, int modulo, int unity)
{
  cout << "initialzing ... " << endl;
  processNumber(modulo);
  int unityOrder = isRoot(unity);
  cout << "unity Order: " << unityOrder << endl;
  modNum = modulo;
  root = unity;
  
  rootLength = unityOrder;
  cout << "inv Root: " ;
  invRoot = findInvRoot();
  cout << invRoot << endl;
  invRootLength = findInvLength();
  cout << "inv Length: ";
  cout << invRootLength << endl;

  //exit(0);
  
  if (length > unityOrder/2) {
    return -1;
  }

  int maxPower = unityOrder;
  modPower = new int[maxPower];
  invModPower = new int[maxPower];
  memset(modPower, 0, maxPower*sizeof(int));
  modPower[0] = 1;
  invModPower[0] = 1;
  for (long i = 1; i < maxPower; i++) {    
    modPower[i] = (modPower[i-1]*root)%modulo;
    long l = invModPower[i-1]*invRoot;
    invModPower[i] = (l)%modulo;
  }

  cout << "Modes Calculated " << endl;
  //cout << "FFT matrix Created " << endl;
  return 1;
}

int* execNTT(int* input, int length)
{
  int* inputNTT = new int[rootLength];
  int* output = new int[rootLength];
  int i = 0;
  memset(inputNTT, 0, rootLength*sizeof(int));   
  while (length > 0) {
    inputNTT[i++] = input[length-1];
    length--;
  }
  int reduceVar = 0;
  for (size_t row = 0; row < rootLength; row++) {
    output[row] = 0;
    reduceVar = 0;
#pragma omp parallel for reduction(+:reduceVar)
    for (size_t col = 0; col < rootLength; col++) {
      size_t pow = (col*row)%rootLength;
      reduceVar = (modPower[pow]*inputNTT[col] + reduceVar)%modNum ;
    }
    output[row] = reduceVar%modNum;
    //printf("\r%ld", row);
  }
  for (size_t row = 0; row < rootLength; row++) {
    printf("%d,%d\n",inputNTT[row],output[row]);
  }
  return output;
}

int* execInvNTT(int* input)
{
  int* output = new int[rootLength];

  memset(output,0,rootLength*sizeof(int));
  cout << "modnum" << modNum << endl;
  cout << "root" << root << endl;
  cout << "rootlength" << rootLength <<endl;
  cout << "invroot" << invRoot << endl;
  cout << "invrootlength" << invRootLength << endl;

  int reduceVar = 0;
  for (size_t row = 0; row < rootLength; row++) {
    reduceVar = 0;
#pragma omp parallel for reduction(+:reduceVar)
    for (size_t col = 0; col < rootLength; col++) {
      size_t pow = (col*row)%rootLength;
      reduceVar = (invModPower[pow]*input[col] + reduceVar)%modNum;
      //output[row] = output[row]%modNum;
    }
    output[row] = (invRootLength*reduceVar)%modNum;
  }
  //cout << endl;
  cout << "NTT computation complete" << endl;
  return output;
}


long* execInvNTT(long* input)
{
  long* output = new long[rootLength];

  memset(output,0,rootLength*sizeof(long));
  cout << "modnum" << modNum << endl;
  cout << "root" << root << endl;
  cout << "rootlength" << rootLength <<endl;
  cout << "invroot" << invRoot << endl;
  cout << "invrootlength" << invRootLength << endl;

  long reduceVar = 0;
  for (size_t row = 0; row < rootLength; row++) {
    reduceVar = input[0];
#pragma omp parallel for reduction(+:reduceVar)
    for (size_t col = 1; col < rootLength; col++) {
      size_t pow = ((rootLength - col)*row)%rootLength;
      reduceVar = (modPower[pow]*input[col] + reduceVar)%modNum;
      if (reduceVar< 0) {
	printf("reduced at %ld \t %ld\n", row,col);
      }
      //output[row] = output[row]%modNum;
    }
    output[row] = (invRootLength*reduceVar)%modNum;
  }
  for (size_t row = 0; row < rootLength; row++) {
    printf("%ld,%ld\n",input[row],output[row]);
  }
  //cout << endl;
  return output;
}

string generateNumber(int* input)
{  
  int num = 0;
  int carry = 0;
  stringstream ss;
  for (int i = 0; i < rootLength; i++) {
    num = (carry + input[i])%10;
    carry= (carry + input[i])/10;
    //printf("%10d \t %10d \t %10d\n", num, carry, input[i]);
    ss << num;
  }
  string saveStr = ss.str();
  string str = "";
  int i = saveStr.length()-1;
  while (saveStr[i--]=='0');
  i++;
  while (i>=0)
    str += saveStr[i--];
    
  //cout << num << endl;
  //int* output = new int[1];
  //output[0] = num;
  return str;
}

string generateNumber(long* input)
{  
  long num = 0;
  int mul = 1;
  long carry = 0;
  stringstream ss;
  for (int i = 0; i < rootLength; i++) {
    num = (carry + input[i])%10;
    carry= (carry + input[i])/10;
    //printf("%10ld \t %10ld \t %10ld\n", num, carry, input[i]);
    ss << num;
  }
  string saveStr = ss.str();
  string str = "";
  int i = saveStr.length()-1;
  while (saveStr[i--]=='0');
  i++;
  while (i>=0)
    str += saveStr[i--];
    
  //cout << num << endl;
  //int* output = new int[1];
  //output[0] = num;
  return str;
}


int ssaConversion(vector<int>& inVec, int* array)
{
  int vSize = inVec.size();
  long aSize = vSize*8;
  //array = new int[aSize];
  memset(array, 0, sizeof(int)*aSize);
  long j = 0;
  for (int i = inVec.size()-1; i >=0; i--) {
    int d = inVec[i];
    int buf[8] = {0};
    buf[0] = d&0XF0000000;
    buf[1] = d&0X0F000000;
    buf[2] = d&0X00F00000;
    buf[3] = d&0X000F0000;
    buf[4] = d&0X0000F000;
    buf[5] = d&0X00000F00;
    buf[6] = d&0X000000F0;
    buf[7] = d&0X0000000F;
    array[j++] = (buf[0]>>28)&0X0000000F;
    array[j++] = (buf[1]>>24)&0X0000000F;
    array[j++] = (buf[2]>>20)&0X0000000F;
    array[j++] = (buf[3]>>16)&0X0000000F;
    array[j++] = (buf[4]>>12)&0X0000000F;
    array[j++] = (buf[5]>>8)&0X0000000F;
    array[j++] = (buf[6]>>4)&0X0000000F;
    array[j++] = (buf[7])&0X0000000F;
  }
  return aSize;
}


int findInvRoot()
{
  return inverse(root);
}

int findInvLength()
{
  return inverse(rootLength);
}

void printModMatrix()
{
  cout << endl;
  for (int row = 0; row < rootLength; row++) {
    int rowNum = row*rootLength;
    cout << "Row(" << setw(3) << row << "):";
    for (int col = 0; col < rootLength; col++) {
      cout << setw(3) << " " << setw(3) << mod[rowNum + col];
    }
    cout << endl;
  }
}

void printInvModMatrix()
{
  cout << endl;
  for (int row = 0; row < rootLength; row++) {
    int rowNum = row*rootLength;
    cout << "Row(" << setw(3) << row << "):";
    for (int col = 0; col < rootLength; col++) {
      cout << setw(3) << " " << setw(3) << invmod[rowNum + col];
    }
    cout << endl;
  }
}
