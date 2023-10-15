// 24-September-2023
// updated version using array for accelerating with nvidia/openmp
// trying to copy the python function provided by the following website
// https://www.nayuki.io/res/number-theoretic-transform-integer-dft/numbertheoretictransform.py
// https://www.nayuki.io/res/number-theoretic-transform-integer-dft/numbertheoretictransform.js
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
unsigned int find_primitive_root(int degree, int totient, int modulo);
unsigned int find_generator(int totient, int modulo);
bool is_primitive_root(int value, int degree, int modulo);
int maxof(int,int);
bool isprime(int);
unsigned int max_in_array(vector<unsigned int>& array);
vector<unsigned int> unique_prime_factors(unsigned int value);

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
    // printDecimalVectorFileComma(&decimalVector,"commaFile.txt");
    //int n = 127;
    // pDabble = INT_MAX;
    cout << "vector length: " << decimalVector.size() << endl;
    if (isprime(pDabble))
    {
      cout << pDabble << " is prime" << endl;
    }
    else
    {
      cout << pDabble << " is not prime" << endl;
    }

    int mod = find_modulus(decimalVector.size(),max_in_array(decimalVector) );

    cout << "Modulus is: " << mod << endl;
    cout << "Max is: " << max_in_array(decimalVector) << endl;
    if (isprime(mod))
    {
      cout <<  mod << " is prime" << endl;
    }
    else
    {
      cout << mod << " is not prime" << endl;
    }

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
  int startingPoint = (ArrayLength -1 + MaxElement-1)/ArrayLength;
  if (startingPoint<1)
  {
    startingPoint = 1;
  }
  for (int i = 1, n = startingPoint*ArrayLength+1; i <= INT_MAX;i++,n+=ArrayLength)
  {
    if (isprime(n))
    {
      return n;
    }
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

unsigned int max_in_array(vector<unsigned int>& array)
{
  unsigned int starting_point = 0;
  vector<unsigned int>::iterator itr = array.begin();
  unsigned int max = starting_point;
  while (itr != array.end())
  {
    if (max<*itr)
    {
      max = *itr;
    }
    itr++;
  }
  return max;
}

unsigned int find_primitive_root(int degree, int totient, int modulo)
{
  // degree in length of array
  // totient in modulus -1 
  // modulo is modulus
}

unsigned int find_generator(int totient, int modulo)
{
  // totient is modulo - 1

  // returnig -1 will represent error
  if (! (1 <=totient && totient<modulo))
  {
    return -1;
  }
  for (unsigned int i = 1; i < modulo; i++)
  {
    if (is_primitive_root(i,totient,modulo))
    {
      return i;
    }
  }
  // if program reached this point 
  // it means no generator functions are found
  return -1;
}

bool is_primitive_root(int value, int degree, int modulo)
{
  
}

vector<unsigned int> unique_prime_factors(unsigned int value)
{}