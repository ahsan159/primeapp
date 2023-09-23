// Copyright by Muhammad Ahsan [2018]

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <climits>
#include <omp.h>

#include "vecutil.hh"
#include "primitive.hh"
#include "ntt.hh"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
 
  auto currTime = system_clock::now();
  time_t currTime_t = system_clock::to_time_t(currTime);
  cout << ctime(&currTime_t);
  cout <<"this is the program start: " << INT_MAX << endl;

  vector<int> num;
  int p = readPrevious(&num, "prime9941.txt");
  int* aa = new int[num.size()*8];
  long l = ssaConversion(num, aa);
  cout << "Total Length: " << l << endl;
  //cout << "Processing Parameters: " << processModulo(l, 199999, 16) << endl;
  cout <<"Processing Parameters: " <<  processModulo(l, 10903, 14) << endl;
  //cout <<"Processing Parameters: " <<  processModulo(l, 109961, 12) << endl;
  //cout <<"Processing Parameters: " <<  processModulo(l, 8191, 11) << endl;
  //printModMatrix();
  //printInvModMatrix();



  int* aNTT = execNTT(aa, l);
  
  long* cNTT = new long[rootLength];
#pragma omp parallel for 
  for (int i = 0; i < rootLength; i++) {
    long k = aNTT[i];
    cNTT[i] = (k*k)%modNum;
  }

   
  long* c = execInvNTT(cNTT);
  
  cout << "Mul Output: " << generateNumber(c) << endl;
  
  auto currTime2 = system_clock::now();
  time_t currTime2_t = system_clock::to_time_t(currTime2);
  cout << ctime(&currTime2_t);
  return 0;
}
