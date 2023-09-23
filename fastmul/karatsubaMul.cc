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

using namespace std;
using namespace std::chrono;

vector<int> karatsuba(vector<int>&, vector<int>&);

int main(int argc, char* argv[]) {
  auto currTime = system_clock::now();
  time_t currTime_t = system_clock::to_time_t(currTime);
  cout << ctime(&currTime_t);
  cout <<"this is the program start: " << INT_MAX << endl;

  vector<int> v1,v2,v3;
  cout << readPrevious(&v1, "prime9941.txt") << endl;
  //v1.push_back(0);
  v2 = v1;

  cout << "num1: " << v1.size() << endl;
  v3 = karatsuba(v1, v2);
  //mulVec(v1, v2, v3);
  cout<< "num3: " << v3.size() << endl;
  //printHexFile(&v3, "outputmul1.txt");
  cout << "num4: ";
  //
  //v3.push_back(0);
  printHex(&v3);
  auto currTime2 = system_clock::now();
  time_t currTime2_t = system_clock::to_time_t(currTime2);
  cout << ctime(&currTime2_t);
  return 0;
}

vector<int> karatsuba(vector<int>& a, vector<int>& b) {
  vector<int> output;
  if (a.size() < 5 || b.size() < 5) {
    mulVec(a,b,output);
    return output;
  }
  vector<int> num1H,num1L,num2H,num2L;
  output.push_back(0);
  int s1H = a.size();
  int s1L = 0;
  int s2H = b.size();
  int s2L = 0;
  if (s1H%2 == 0) {
    s1H = s1H/2;
    s1L = s1H;
  } else {
    s1H = s1H/2;
    s1L = s1H + 1;
  }
  if (s2H <= s1L) {
    mulVec(a, b, output);
    return output;
  } else {
    s2L = s1L;
    s2H = s2H - s2L;
  }
  //cout << dec << s2H << "\t" << s2L << endl;
  //cout << dec << s1H << "\t" << s1L << endl;
  num1H.assign(a.begin(),a.begin()+s1H);
  num1L.assign(a.begin()+s1H,a.end());
  num2H.assign(b.begin(),b.begin()+s2H);
  num2L.assign(b.begin()+s2H,b.end());
  
  vector<int> z1,z0,z2;
  vector<int> sum1,sum2;
  
#pragma omp parallel sections 
  {
#pragma omp section
    {
    z2 = karatsuba(num1L,num2L);
    }
#pragma omp section
    {
    z0 = karatsuba(num1H,num2H);
    }
#pragma omp section
    {
      bcdAdd(num1L, num2H, sum1);
      bcdAdd(num2L, num1H, sum2);
      z1 = karatsuba(sum1,sum2);
    }
  }

  vector<int> buf1,buf2;
  bcdAdd(z2,z0,buf1);
  bcdSubtract(z1,buf1,buf2);
  z1.clear();
  z1.assign(buf2.begin(),buf2.end());
  
  while(s1H-- > 0) {
    z2.insert(z2.begin(),0);
    z2.insert(z2.begin(),0);
    z1.insert(z1.begin(),0);
  }
  bcdAdd(z2,z1,buf1);
  bcdAdd(buf1,z0,output);
  return output;
}
