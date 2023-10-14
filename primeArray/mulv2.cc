// 24-September-2023
// updated version using array for accelerating with nvidia/openmp
// trying to copy the python function provided by the following website
// https://www.nayuki.io/res/number-theoretic-transform-integer-dft/numbertheoretictransform.py
// https://www.nayuki.io/page/number-theoretic-transform-integer-dft

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

#include "veclibv2.hh"

using namespace std;
using namespace std::chrono;

int find_modulus(int ArrayLength, int MaxElement);
int maxof(int,int);
bool isprime(int);

int primeP;
int pDabble;

int main(int argc, char* argv[])
{
    cout << "size of int: " << sizeof(int) << endl;
    cout << "max of int: " << INT_MAX << endl;

    cout << "size of long: " << sizeof(long) << endl;
    cout << "max of long: " << LONG_MAX << endl;

    cout << "size of size_t: " << sizeof(size_t) << endl;
    cout << "max of size_t: " << SIZE_MAX << endl;

    string outputFile="outputFile.txt";
    string inputFile;
    if (argc == 1) {
    inputFile = "nprime8191.txt";
    }
    else if (argc == 2)
    {
        inputFile = argv[1];
    }
    primeP = 1;
    vector<int> read;
    pDabble = readPrevious(&read,inputFile.c_str());
    vector<unsigned int> decimalVector;
    getDecimalVector(&read,&decimalVector);
    // printDecimalVector(&decimalVector);
    vector<unsigned int> result;
    result.assign(decimalVector.size(),0);
    // printDecimalVector(&result);
    cout<< endl;

    //int n = 127;
    pDabble = INT_MAX;
    if (isprime(pDabble))
    {
      cout << pDabble << " is prime" << endl;
    }
    else
    {
      cout << pDabble << " is not prime" << endl;
    }

    cout << "Modulus is: " << find_modulus(decimalVector.size(),1) << endl;

    auto timePrint = system_clock::now();
    time_t timePrint_t = system_clock::to_time_t(timePrint);
    cout << ctime(&timePrint_t) << endl;
    return 0;
}


 int find_modulus(int ArrayLength, int MaxElement)
 {
  // N is the modulus, N must be prime
  // n is array length
  // m is maximum element
  // general approach to avoid any overflow in to set modulus is
  // N = n*m^2 + 1;
  // will return -1 if found any fault
  if (ArrayLength<1 || MaxElement < 1)
  {
    return -1;
  }
  int startingPoint = ArrayLength -1 + MaxElement;
  while(startingPoint<=INT_MAX)
  {
    int n=startingPoint*ArrayLength + 1;
    if (isprime(n))
    {
      return n;
    }
    startingPoint++;
  }
  return -1;
 }

 int maxof(int i, int j)
 {
  if (i>j)
  {
    return i;
  }
  return j;
 }

bool isprime(int p)
{
  int p2=p/2;
  int i = 2;
  while (i<=p2)
  {
    if (p%i==0)
    {
      return false;
      break;
    }
    i++;
  }
  return true;
}