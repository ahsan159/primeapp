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
int maxof(int, int);
bool isprime(int);
unsigned int max_in_array(vector<unsigned int> &array);
vector<unsigned int> unique_prime_factors(unsigned int value);
unsigned int powMod(unsigned int, unsigned int, unsigned int);
void digitShift(vector<unsigned int> &);
int CompareArray(vector<unsigned int> &a, vector<unsigned int> &b);
vector<unsigned int> AddArray(vector<unsigned int> &a, vector<unsigned int> &b);
vector<unsigned int> SubtractArray(vector<unsigned int> &a, vector<unsigned int> &b);
vector<unsigned int> MultiplicativeInverse(vector<unsigned int> a);
vector<unsigned int> ntt(vector<unsigned int>&,unsigned int, unsigned int);

vector<unsigned int> rearrangeVector(vector<unsigned int> &a,int N);

vector<unsigned int> rearrangeVector(vector<unsigned int> &a,int N)
{
  stringstream ss;
  vector<unsigned int>::iterator itr = a.begin();
  while (itr != a.end())
  {
    ss << setw(8) << setfill('0') << dec << *itr;
    itr++;
  }
  string stringVector = ss.str();
  //cout << "data :" <<stringVector << endl;
  vector<unsigned int> rearrangedVector;
  for (int i = 0; i < stringVector.length(); i+=N)
  {
    unsigned int ui = stoul(stringVector.substr(i,N),nullptr,10);
    rearrangedVector.push_back(ui);
  }
  return rearrangedVector;
}

int primeP;
int pDabble;

int main(int argc, char *argv[])
{
  cout << "size of int: " << sizeof(int) << endl;
  cout << "max of int: " << INT_MAX << endl;

  cout << "size of long: " << sizeof(long) << endl;
  cout << "max of long: " << LONG_MAX << endl;

  cout << "size of size_t: " << sizeof(size_t) << endl;
  cout << "max of size_t: " << SIZE_MAX << endl;

  cout << "size of double: " << sizeof(double) << endl;
  cout << "max of double: " << __DBL_MAX__ << endl;

  string outputFile = "outputFile.txt";
  string inputFile;
  if (argc == 1)
  {
    inputFile = "nprime8191.txt";
  }
  else if (argc == 2)
  {
    inputFile = argv[1];
  }
  primeP = 1;
  vector<int> read;
  pDabble = readPrevious(&read, inputFile.c_str());
  vector<unsigned int> decimalVector;
  getDecimalVector(&read, &decimalVector);
  // printDecimalVector(&decimalVector);
  vector<unsigned int> result;
  result.assign(decimalVector.size(), 0);
  // printDecimalVector(&result);
  cout << endl;
  vector<unsigned int> rdecimalVector = rearrangeVector(decimalVector,4);
  printDecimalVectorFileComma(&decimalVector,"commaFile.txt");
  printDecimalVectorFileComma(&rdecimalVector,"commaFile1.txt",4);
  // cout << "data4:";
  // printDecimalVector(&rdecimalVector,4);
  // cout << endl;
  // cout << "data0:";
  // printDecimalVector(&decimalVector,8);
  // cout << endl;
  // stringstream ss;
  // copy(rdecimalVector.begin(),rdecimalVector.end(),ostream_iterator<int>(ss<<setfill('0')<<setw(4)," "));
  // cout<<"data1:"<<ss.str() << endl;
  // stringstream ss1;  
  // copy(decimalVector.begin(),decimalVector.end(),ostream_iterator<int>(ss1<<setw(8)<<setfill('0')," "));
  // cout<<"data2:"<<ss1.str() << endl;
  // // int n = 127;
  // // pDabble = INT_MAX;
  // cout << "vector length: " << decimalVector.size() << endl;
  // if (isprime(pDabble))
  // {
  //   cout << pDabble << " is prime" << endl;
  // }
  // else
  // {
  //   cout << pDabble << " is not prime" << endl;
  // }

  int mod = find_modulus(rdecimalVector.size(), max_in_array(rdecimalVector));

  cout << "Modulus is: " << mod << endl;
  // cout << "Max is: " << max_in_array(decimalVector) << endl;
  if (isprime(mod))
  {
    cout << mod << " is prime" << endl;
  }
  else
  {
    cout << mod << " is not prime" << endl;
  }

  unsigned int root = find_primitive_root(rdecimalVector.size(),mod-1,mod);
  // unsigned int root = find_primitive_root(5,84105730,84105731);
  cout << "Primitive root is: " << endl;
  cout << root << endl;
  // cout << powMod(2,16821146,84105731)<<endl;
  // cout << 67108864%mod << endl;
  vector<unsigned int> upf = unique_prime_factors(8430);
  cout << endl;
  copy(upf.begin(), upf.end(),ostream_iterator<unsigned int>(cout,"\t"));
  cout << endl;

  vector<unsigned int> result1 = ntt(rdecimalVector,root,mod);
  copy(result1.begin(), result1.end(),ostream_iterator<int>(cout,", "));
  cout << endl;
  cout << result1.size() << endl;
  auto timePrint = system_clock::now();
  time_t timePrint_t = system_clock::to_time_t(timePrint);
  cout << ctime(&timePrint_t) << endl;

  // cout << "Vector is: " << endl;
  // printDecimalVector(&decimalVector);
  // cout << endl;

  // vector<unsigned int> vector2 = decimalVector;

  // digitShift(decimalVector);

  // cout << "Vector is: " << endl;
  // printDecimalVector(&decimalVector);
  // cout << endl;

  // cout << "Comparison is: " << CompareArray(decimalVector, vector2) << endl;
  // vector<unsigned int> addr = AddArray(decimalVector, decimalVector);
  // cout << "Addition Result is: " << endl;
  // printDecimalVector(&addr);
  // cout << endl;

  //MultiplicativeInverse(vector2);

  // digitShift(decimalVector);
  // vector<unsigned int> sub = SubtractArray(addr, decimalVector);
  // cout << "Subtraction Result is: " << endl;
  // printDecimalVector(&sub);
  // cout << endl;
  // below test proves that using double for guessing of 
  // quotient in divison will cause large errors
  // double a = 170141183460469231731687303715884105727;
  // double d = 1.0;
  // cout << setprecision(100) << d / a << endl;
  return 0;
}

vector<unsigned int> ntt(vector<unsigned int>& invector,unsigned int root, unsigned int modulo)
{
  unsigned int len = invector.size();
  cout << root  << "lend " << modulo << "len" << len << endl;
  vector<unsigned int> output;
  for (unsigned int i = 0; i < len; i++)
  {
    unsigned int sum = 0;
    for (unsigned int j = 0; j < len; j++)
    {
      unsigned int l = i*j%len;
      unsigned int l2 = (invector[j] * powMod(root,l,modulo))%modulo;
      sum = (sum + invector[j] * powMod(root,l,modulo))%modulo;
    }
    output.push_back(sum);
  }
  return output;
}

int find_modulus(int ArrayLength, int MaxElement)
{
  // N is the modulus, N must be prime
  // n is array length
  // m is maximum element
  // general approach to avoid any overflow in to set modulus is
  // N = n*m^2 + 1;
  // will return -1 if found any fault
  if (ArrayLength < 1 || MaxElement < 1)
  {
    return -1;
  }
  int startingPoint = (ArrayLength - 1 + MaxElement - 1) / ArrayLength;
  if (startingPoint < 1)
  {
    startingPoint = 1;
  }
  for (int i = 1, n = startingPoint * ArrayLength + 1; i <= INT_MAX; i++, n += ArrayLength)
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
  if (i > j)
  {
    return i;
  }
  return j;
}

bool isprime(int p)
{
  int p2 = p / 2;
  int i = 2;
  while (i <= p2)
  {
    if (p % i == 0)
    {
      return false;
      break;
    }
    i++;
  }
  return true;
}

unsigned int max_in_array(vector<unsigned int> &array)
{
  unsigned int starting_point = 0;
  vector<unsigned int>::iterator itr = array.begin();
  unsigned int max = starting_point;
  while (itr != array.end())
  {
    if (max < *itr)
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
  // totient = modulus -1
  // modulo is modulus
  if (!((0<=degree) && (degree<=totient) && (totient<modulo)))
  {
    return -1;
  }
  if (totient%degree != 0)
  {
    return -1;
  }
  unsigned int g = find_generator(totient,modulo);
  cout << "generator " << g << endl;
  return powMod(g,totient/degree,modulo);
}

unsigned int find_generator(int totient, int modulo)
{
  // totient = modulo - 1

  // returnig -1 will represent error
  if (!(1 <= totient && totient < modulo))
  {
    //cout << "returning from generator" << endl;
    return -1;
  }
  for (unsigned int i = 1; i < modulo; i++)
  {
    //cout << "testing proot" << i << endl;
    if (is_primitive_root(i, totient, modulo))
    {
      cout << "primitive root" << i << endl;
      return i;
    }
  }
  // if program reached this point
  // it means no generator functions are found
  return -1;
}

bool is_primitive_root(int value, int degree, int modulo)
{
  // check this function throughly again 17-oct-2023
  bool result = false;
  if (!((0 <= value) && (value < modulo)))
  {
    result = false;
  }
  if (!((1 <= degree) && (degree < modulo)))
  {
    result = false;
  }
  bool valueModulo = powMod(value,degree,modulo)==1;
  //cout << "pmd " << valueModulo << endl;
  // bool valuePrime = false;
  vector<unsigned int> uniqueFactors = unique_prime_factors(degree);
  vector<unsigned int>::iterator itr = uniqueFactors.begin();
  while (itr != uniqueFactors.end())
  {
    if (powMod(value,degree/(*itr),modulo)!=1)
    {
      result = true;
    }
    else
    {
      result = false;
      break;
    }
    itr++;
  }
  // bool valueModulo = powMod(value,degree,modulo)==1;
  //cout << "upf " << result << endl;
  return result&valueModulo;
}

vector<unsigned int> unique_prime_factors(unsigned int value)
{
  vector<unsigned int> result;
  unsigned int square_root = (int)sqrt((double)value);
  // result.push_back(-1);
  if (value < 1)
  {
    return result;
  }
  for (int i = 2; i <= square_root; i++)
  {
    if (value%i == 0)
    {
      result.push_back(i);
      while (value%i == 0)
      {
        value = value/i;
      }
      square_root = (int)sqrt((double)value);
    }
  }
  if (value>1)
  {
    result.push_back(value);
  }
  return result;
}

void digitShift(vector<unsigned int> &array)
{
  vector<unsigned int>::reverse_iterator itr = array.rbegin();
  unsigned int carry = 0;
  while (itr != array.rend())
  {
    // real shifting operation by multiplication and remainder
    // cout << *itr << endl;
    unsigned int multiply = *itr * 10;
    *itr = multiply % 100000000 + carry;
    carry = multiply / 100000000;
    // cout << *itr << endl;
    // cout << carry << endl;
    itr++;
  }
  if (carry != 0)
  {
    // array.push_back(0);
    array.insert(array.begin(), carry);
  }
}

int CompareArray(vector<unsigned int> &a, vector<unsigned int> &b)
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
  while (itra != a.end() && itrb != b.end())
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

vector<unsigned int> AddArray(vector<unsigned int> &a, vector<unsigned int> &b)
{
  vector<unsigned int> result;
  vector<unsigned int> larger = b;
  vector<unsigned int> smaller = a;
  if (a.size() > b.size())
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
    unsigned int r = *itrl + *itrs + carry;
    carry = r / 100000000;
    r = r % 100000000;
    result.insert(result.begin(), r);
    itrl++;
    itrs++;
  }
  // add remaining larger array
  while (itrl != larger.rend())
  {
    unsigned int r = *itrl + *itrs + carry;
    carry = r / 100000000;
    r = r % 100000000;
    result.insert(result.begin(), r);
    itrl++;
  }
  // double check carry if two array are of same size and are causing
  // carry overflow
  if (carry != 0)
  {
    result.insert(result.begin(), carry);
  }
  return result;
}

vector<unsigned int> SubtractArray(vector<unsigned int> &a, vector<unsigned int> &b)
{
  // this will implement subtraction using complementry method
  // this requires b << a
  vector<unsigned int> bCopy = b; // make a copy for complement
  while (bCopy.size() < a.size())
  {
    bCopy.insert(bCopy.begin(), 0);
  }
  // complement individual decimals
  vector<unsigned int>::iterator itr = bCopy.begin();
  while (itr != bCopy.end())
  {
    *itr = 99999999 - *itr;
    itr++;
  }
  // perform addition
  vector<unsigned int> result = AddArray(a, bCopy);

  // get carry and add in to last integer
  string str = to_string(*(result.begin()));
  int size = str.size() - 1;
  int m10 = 1;
  for (int i = 0; i < size; i++)
  {
    m10 = m10 * 10;
  }
  *(result.begin()) = *(result.begin()) % m10;
  unsigned int carry = 1;
  vector<unsigned int>::reverse_iterator ritr = result.rbegin();
  while (ritr != result.rend() && carry != 0)
  {
    *ritr = *ritr + carry;
    carry = *ritr / 100000000;
    *ritr = *ritr % 100000000;
    ritr++;
  }
  if (*(result.begin())==0)
  {
    result.erase(result.begin());
  }

  return result;
}

vector<unsigned int> MultiplicativeInverse(vector<unsigned int> a)
{
  vector<unsigned int> result;
  // step 1 make nominator larger than denominator
  result.insert(result.begin(), a.size(), 0);
  if (*(a.begin()) >= 10000000)
  {
    result.insert(result.begin(), 1);
  }
  else
  {
    *(result.begin()) = 1;
    while (CompareArray(result, a) != 1)
    {
      digitShift(result);
    }
  }
  // cout << "test" << endl;
  // printDecimalVector(&a);
  // cout << endl;
  // variable i presents the precision required
  // in our case precision will be 100 digits
  int i = 1;
  while (i++ <= 100)
  {
    // quot will be current decimal
    unsigned int quot = 0;
    while (CompareArray(result, a) == 1)
    {
      // keep comparing and subtracting unless nominator is
      // smaller that denominator

      // cout << endl;
      // printDecimalVector(&result);
      // cout << endl;

      result = SubtractArray(result, a);

      // cout << endl;
      // printDecimalVector(&result);
      // cout << endl;
      quot++;
    }
    cout << quot;
    // increment denominator by appending zero and calculate next
    // quotient digit
    digitShift(result);
  }
  cout << endl;
  // current quotient is not returned
  return result;
}

unsigned int powMod(unsigned int x, unsigned int y, unsigned int m)
{
  // implementation of x^y % m
  // if return -1 then there is error
  // cout << "here";
  // cout << x<<"," << y << "," << m << endl;
  if (y < 0 || m <= 0)
  {
    return -1;
  }
  if (!((0 <= x) && (x < m)))
  {
    // cout << "x:" << x << endl;
    x = ((x%m)+m)%m;
    // cout << "x:" << x << endl;
  }
  unsigned long result = 1;
  int loop = 0;
  while (y != 0)
  {
    if ((y & 1) != 0)
    {
      // cout << "r1: " << result << endl;
      result = result * x;
      // cout << "r2: " << result << endl;
      result = result % m;
      // cout << "r3: " << result << endl;
    }
    //  cout << "r: " << result << endl;
    //  cout << "x: " << x  <<endl;
    unsigned long xbuf = x;
    xbuf = xbuf*xbuf%m;
    x = (unsigned int)xbuf;
    //cout << "x: " << x  <<endl;
    y = y >> 1;
    //cout << "y:" << y << endl;
    //loop++;
  }
  //cout << "result is: " << result << endl;
  return result;
}