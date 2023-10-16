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
unsigned int powMod(unsigned int, unsigned int, unsigned int);
void digitShift(vector<unsigned int>&);
int CompareArray(vector<unsigned int>& a, vector<unsigned int>& b);
vector<unsigned int> AddArray(vector<unsigned int>&a, vector<unsigned int>&b);
vector<unsigned int> MultiplicativeInverse(vector<unsigned int> a);

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

    cout << "size of double: " << sizeof(double) << endl;
    cout << "max of double: " << __DBL_MAX__ << endl;

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

    cout << "Vector is: " << endl;
    printDecimalVector(&decimalVector);
    cout << endl;

    vector<unsigned int> vector2 = decimalVector;

    // digitShift(decimalVector);

    cout << "Vector is: " << endl;
    printDecimalVector(&decimalVector);
    cout << endl;

    cout << "Comparison is: " << CompareArray(decimalVector,vector2) << endl;
    vector<unsigned int> addr = AddArray(decimalVector,decimalVector);
    cout << "Addition Result is: " << endl;
    printDecimalVector(&addr);
    cout << endl;

    MultiplicativeInverse(decimalVector);

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

void digitShift(vector<unsigned int>& array)
{
    vector<unsigned int>::reverse_iterator itr = array.rbegin();
    unsigned int carry = 0;
    while (itr != array.rend())
    {
      // real shifting operation by multiplication and remainder
      // cout << *itr << endl;
      unsigned int multiply = *itr*10;
      *itr = multiply%100000000 + carry;
      carry = multiply/100000000;
      // cout << *itr << endl;
      // cout << carry << endl;
      itr++;
    }
    if (carry!=0)
    {
      // array.push_back(0);
    array.insert(array.begin(),carry);
    }
}

int CompareArray(vector<unsigned int>& a, vector<unsigned int>& b)
{
  // return 1 if a>>b
  // return -1 if a<<b
  // return 0 if a==b
  // this requires clean array i.e. first elements must not be 
  // zeros
  if (a.size() > b.size())
  {
    return 1;
  }
  else if (a.size() < b.size())
  {
    return -1;
  }
  vector<unsigned int>::iterator itra = a.begin();
  vector<unsigned int>::iterator itrb = b.begin();
  while (itra!=a.end() && itrb!=b.end())
  {
    if (*itra > *itrb)
    {
      return 1;
    }
    else if (*itra < *itrb)
    {
      return -1;
    }
    itra++;
    itrb++;
  }
  return 0;
}

vector<unsigned int> AddArray(vector<unsigned int>&a, vector<unsigned int>&b)
{
  vector<unsigned int> result;
  vector<unsigned int> larger = b;
  vector<unsigned int> smaller = a;
  if(a.size() > b.size())
  {
    larger = a;
    smaller = b;
  }
  unsigned int carry = 0;
  vector<unsigned int>::reverse_iterator itrl = larger.rbegin();
  vector<unsigned int>::reverse_iterator itrs = smaller.rbegin();
  // first step add two array until smaller reach its limit
  while (itrs != smaller.rend())
  {
    unsigned int r = *itrl + *itrs  + carry;
    carry = r/100000000;
    r = r%100000000;
    result.insert(result.begin(),r);
    itrl++;
    itrs++;
  }
  // add remaining larger array
  while(itrl!=larger.rend())
  {
    unsigned int r = *itrl + *itrs  + carry;
    carry = r/100000000;
    r = r%100000000;
    result.insert(result.begin(),r);
    itrl++;
  }
  // double check carry if two array are of same size and are causing 
  // carry overflow
  if (carry!=0)
  {
    result.insert(result.begin(), carry);
  }
  return result;
}

vector<unsigned int> MultiplicativeInverse(vector<unsigned int> a)
{
  vector<unsigned int> result;
  result.insert(result.begin(),a.size(),0);
  if (*(a.begin()) >= 10000000)
  {
    result.insert(result.begin(),1);
  }
  else
  {
    *(result.begin()) = 1;
    while (CompareArray(result,a)!=1)
    {
      digitShift(result);
    }
  }
  cout << "test" << endl;
  printDecimalVector(&result);
  cout << endl;
  return result;
}

unsigned int powMod(unsigned int x, unsigned int y, unsigned int m)
 {
     // implementation of x^y % m
     // if return -1 then there is error
     if (x < 0 || m <= 0)
     {
         return -1;
     }
     if (!((0 <= x) && (x < m)))
     {
        return -1;
     }
     unsigned int result = 1;
     while (y != 0)
     {
         if ((y & 1) != 0)
         {
             result = result * x;
             result = result % m;
         }
         x = x * x;
         x = x % m;
         y = y >> 1;
     }
     return result;
}