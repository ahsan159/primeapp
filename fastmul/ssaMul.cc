// Copyright by Muhammad Ahsan [2018]

// 22-Sep-2021
// 1. New NTT and inv NTT using arrays
// 2. added omp functionality during transform
// 3. selection of unity root and modulo selection
//    code rewritting started. As previous codes
//    only hit and trial

// 24-Sep-2021
// 1. Code for factorization added which can provide
//    both factors and non-factors
// 2. Code for finding generator has been added
// 3. https://www.nayuki.io/page/number-theoretic-transform-integer-dft#:~:text=Compute%20the%20inverse%20NTT%20with,)%20mod%20n)%20mod%20N
// 4. Following theory is implemented
//    find n = size of array
//    find m = maximum value in array
//    find M = m*m*n + 1
//    find N = k*n + 1 where N>=M & isprime(N)=1
//    factorize 'N-1'
//    use non-factors as generator candidates (c)
//    use factors as test (f)
//    find if (c^n mod N = 1)
//    if above hold true test this as generator
//    find if (c^f mod N = 1) then generator is not good
//    generator
//    find good generator (w) or (g)  where (c^n mod N = 1)
//    && (c^f mod N != 1) this w or g is root of unity or
//    g^k  is w or g
//    rest is NTT transform which is vector multiplication



#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <climits>
#include <omp.h>

//#include "vecutil.hh"
#include "primitive.hh"
#include "ntt.hh"


#define dlen 8
using namespace std;
using namespace std::chrono;

int readFile(vector<int>&,char*);
int printNumberToFile(vector<int>&, char*);
void printNumber(vector<int>&);
int addNumber(vector<int>&,vector<int>&,vector<int>&);
int mulNumber(vector<int>&, vector<int>&, vector<int>&);
int mulScalar(vector<int>&, vector<int>&, long);
//int* transform(int*, int[dlen][dlen], int);
//int* transform(int*, long[dlen][dlen], int);
int iUnity(int, int);
int iN(int, int);
int transform(long*, long*, long**,int, int, int);
int itransform(long*, long*, long**, int, int, int);
int mulVector(int*, int*, int*, int, int);
int mulVector(long*, long*, long*, int, int);
int mulVector(vector<int>, vector<int>, vector<int>, int);
int isPrime(long);
vector<long> factorize(long p,int);


int isPrime(long num)
{
  if (num%2==0) { return 0;}
  double numSqrt = sqrt((double)num);
  long numSqrtDouble = (long)numSqrt + 1;
  for (long i = 3; i < numSqrtDouble; i+=2) {
    if (num%i == 0) { return 0;}    
  }
  return 1;
}

int readFile(vector<int>& vec, char* file)
{
  ifstream infile(file, std::ios::in);
  if (!infile) {
    return -1;
  }
  while (!infile.eof()) {
    char s[8];
    infile.read(s,8);
    vec.push_back(stoi((string)s));
    //cout << s << ",";
  }
  //cout << endl;
  vec.pop_back();
  return 0;
}

void printNumber(vector<int> & vec)
{
  vector<int>::iterator itr = vec.begin();
  while(itr!=vec.end()) {
    cout<<setw(8) << setfill('0') <<*itr;
    itr++;
  }
}

int printNumberToFile(vector<int>& vec, char* fileName)
{
  ofstream outFile(fileName, ios::out);
  if (!outFile) {
    return -1;
  }
  vector<int>::iterator itr = vec.begin();
  while(itr!=vec.end()) {
    outFile<<setw(8) << setfill('0') <<*itr;
    itr++;
  }
  return 0;
}

int addNumber(vector<int>& a, vector<int>& b, vector<int>& c)
{
  if(a.size()>b.size()) {
    while (b.size() != a.size()) {
      b.insert(b.begin(),0);
    }
  } else if(a.size() < b.size()) {
    while(b.size() != a.size()) {
      a.insert(b.begin(),0);
    }
  }
  //cout << a.size() << "," << b.size() << endl;
  int carry = 0;
  vector<int>::reverse_iterator itr1 = a.rbegin();
  vector<int>::reverse_iterator itr2 = b.rbegin();
  //vector<int>::reverse_iterator itr3 = c.rbegin();
  c.clear();
  while(itr1 != a.rend()) {
    int i = carry + *itr1 + *itr2;
    carry = i/100000000;
    i = i%100000000;
    c.insert(c.begin(),i);
    itr1++;
    itr2++;
    }
  if (carry != 0) {
    c.insert(c.begin(),carry);
  }
  return 0;
}

int mulNumber(vector<int>& a, vector<int>& b, vector<int>& c)
{
  vector<int> d, e, z, o;
  d.clear();
  c.clear();
  e.clear();
  z.clear();
  //vector<int>::reverse_iterator itr = b.rbegin();
  
  //#pragma omp parallel for
  for (vector<int>::reverse_iterator itr = b.rbegin(); itr!=b.rend(); itr++) {
    //cout << *itr << endl;
    mulScalar(a,d,*itr);
    d.insert(d.end(),z.begin(),z.end());
    z.push_back(0);
    //printNumber(d);
    //cout << endl;
    addNumber(d, c, e);
    //printNumber(e);
    //cout << endl;
    c = e;
  }
  //c = o;
  return 0;
}

int mulScalar(vector<int>& a, vector<int>& b, long scalar)
{
  b.clear();
  vector<int>::reverse_iterator itr = a.rbegin();
  int carry = 0X00;
  while(itr!=a.rend() ) {
    long i = (long)((*itr)*scalar) + carry;
    carry = i/100000000;
    b.insert(b.begin(), i%100000000);
    itr++;
  }
  if (carry != 0) {
    b.insert(b.begin(), carry);
  }
  return 0;
}

int iUnity(int unity, int mod) {
  int j;
  for (int i = 1; i < mod; i++) {
    j = (i*unity)%mod;
    if (j == 1) {
      return i;
    }
  }
  return -1;
}

int iN(int N, int mod) {
  int j;
  for (int i = 1; i < mod; i++) {
    j = (i*N)%mod;
    if (j == 1) {
      return i;
    }
  }
  return -1;
}

int transform(long* array, long* output,long** roots, int len, int mod)
{
  //int *output = new int[len];
  //cout << "data length: " <<  len << endl;
  /*
  for (int i = 0; i < len; i++) {
    cout << array[i];
  }  
  cout << endl;

  for (int i = 0; i < dlen; i++) {
    for (int j = 0; j < dlen; j++ ) {
      cout << setw(5) << setfill (' ') << roots[i][j] ;
    }
    cout << endl;
  }
  */
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    output[i] = 0;
    for (int j = 0; j < len; j++) {
      output[i] += (roots[i][j]*array[j])%mod;
      output[i] = output[i]%mod;
    }
  }
  /*
  cout << "Outputing: "<< endl;
  for (int i = 0; i < len; i++) {
    cout << setw(5) << setfill(' ') << output[i];
  }  
  cout << endl;
  */
  //array = output;
  return 0;
}

int mulVector(int* a, int* b, int* c, int len, int mod) {
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    c[i] = (a[i]*b[i])%mod;
  }
  return 0;
}

int mulVector(long* a, long* b, long* c, int len, int mod) {
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    c[i] = (a[i]*b[i])%mod;
  }
  return 0;
}

int mulVector(vector<int> a, vector<int> b, vector<int> c, int len, int mod)
{
  vector<int>::iterator itra = a.begin();
  vector<int>::iterator itrb = b.begin();
  c.clear();
  while(itra!=a.end() && itrb!=b.end()) {
    c.push_back((*itra * *itrb)%mod);
    itra++;
    itrb++;
  }
  return 0;
}    

int itransform(long* a, long* output,  long** roots, int len, int mod, int invN)  
{
  transform(a, output, roots, len, mod);
#pragma omp parallel for
  for (int i = 0; i < len; i++) {
    output[i] = (output[i]*invN)%mod;
  }
}

vector<long> factorize(long p, int in=0)
{
  vector<long> roots;
  if (in ==0) {
    if (p%2 == 0) {roots.push_back(2);}
    double numSqrt = p/2;
    //long numSqrtDouble = (long)numSqrt + 1;
    for (long i = 3; i <= numSqrt+1; i+=1) {
      if (p%i == 0) { roots.push_back(i);}    
    }
    return roots;
  } else {
    double numSqrt = p/2;
    for (long i = 2; i <=numSqrt; i++) {
      if ((p%i)!=0) {roots.push_back(i);}
    }
  }
}


int main(int argc, char* argv[]) {
  cout <<_OPENMP<< endl;
  cout << LONG_MAX << endl;
  cout << sizeof(std::uint16_t) << endl;
  cout << sizeof(std::size_t) << endl;
  long t = 99999999;
  cout << t*t << endl;
  
  cout << "primility: " << isPrime(t*t) << endl;
  
  vector<int> readVector;
  readVector.clear();
  string sFile = "prime127.txt";
  cout << readPrevious(&readVector,sFile) <<endl;
  //cout << readPreviousData(&v2,sFile) <<endl; 
  for (int i = 0; i < readVector.size(); i++) {
    stringstream ss2;
    ss2 << hex << readVector[i] << dec;
    readVector[i] = stoi(ss2.str());
  }
  long WN = 99999999;
  long M = (long)readVector.size()*2;
  long N  = 500;
  long g; // good generator
  M = 99*99*N + 1;
  //M = 2*N + 1;
  //long N  = 8;
  long k = -1;
  long p = -1;

  // find the prime number 
  for (k = 1; k < M; k++ ) {
    p = k*N + 1;
    if (p >= M) {
      if (isPrime(p)) {
	break;
      }
    }
  }

  cout << "prime: " << p << endl;
  cout << "k:" << k << endl;

  //p = 673;
  vector<long> candidate = factorize(p-1,1); // non-factors
  vector<long> generator; // will store good generators
  generator.clear();
  vector<long> factors = factorize(p-1,0); // factors for testing
  cout << "Factors: "<< endl;


  // print factors
  for (vector<long>::iterator itr = factors.begin(); itr!=factors.end(); itr++) {
    cout << setw(8) <<*itr;
  }
  cout << endl;
  cout << "Max : " << factors[factors.size()-1] << endl;

  // find generator from candidates
  for (vector<long>::iterator itr = candidate.begin();itr!=candidate.end();itr++) { //start checking generator functions in this loop
    //cout << "Testing: " << *itr << endl;
    long res = 1;
    for (long mod = 0; mod<N; mod++) {
      //find generator^N mod p == 1 
      res = res*(*itr);
      res = res%p;
    }
    if(res==1) { // if c^N mod p == 1 this is indeed possible good generator
      //cout << *itr << " is candidate" << endl;
      generator.push_back(*itr);
    }
  }
  int w = -1;
  // verifying generators from candidates
  for (vector<long>::iterator itr = generator.begin();itr!=generator.end();itr++) { //start checking generator functions in this loop
    //cout << "Testing candidate: " << *itr << endl;
    int i = -1;
    for(vector<long>::iterator itr2 = factors.begin(); itr2!=factors.end() && *itr2 <N;itr2++) { //test each factor for unity c^f mod p == 1
      //if unity is result do not use generator
      long res = 1;
      for (long mod = 0; mod<*itr2; mod++) {
	//find generator^factor mod p
	res = res*(*itr);
	res = res%p;
      }
      //cout << "Generator: " << *itr << " Factor: " << *itr2 << " result: " << res << endl;
      if(res%p == 1 || res%p == 0) {
	i++;
	break;
      }
    }
    if(i==-1) {
      cout << *itr << " is generator" << endl;
      w = *itr;
      //w = 85;
    }
  }
  //cout << "prime is : " << p << endl;
  int len = N;
  //M = 337;
  //int w = 287;
  M = p;
  
  long* nvec = new long[len];
  long n;
  long **vec = new long*[len];
  long **ivec = new long*[len];

  
  long rt = 1;
  for (int i = 0; i < len; i++) {
    nvec[i] = (long)rt%M; // calculate roots
    rt = rt*w%M;
  }
  n = (long)nvec[len-1]*w;
  //cout << "kth root" << n << endl;
  n = n%M;
  cout << "prime : " << p <<endl;
  cout << "Unity: " << w << endl;
  cout << "kth root: " << n << endl;
  for (int i = 0; i<len;i++) {
    vec[i] = new long[len];
    ivec[i] = new long[len];
    ivec[i][0] = 1;
    for (int j = 0;j<len;j++) {
      int temp = (i*j)%len;
      vec[i][j] = nvec[temp];     //vector modulo
      ivec[i][len-j] = vec[i][j]; //vector inverse modulo
    }
  }  

  /*
  // print roots and inverse roots of unity
  for (int i = 0; i<len;i++) {
    for (int j = 0;j<len;j++) {
      cout << setw(5) << setfill(' ') << vec[i][j];
    }
    cout <<endl;    
  }
  cout << endl;
  for (int i = 0; i<len;i++) {
    for (int j = 0;j<len;j++) {
      cout << setw(5) << setfill(' ') << ivec[i][j];
    }
    cout <<endl;
  }
  cout << endl;
  */
  
  // sample calculations
  string s1 = "1234";
  string s2 = "5678";
  int slen = s1.length();
  //int alen = 2*s1.length();
  int alen = len;
  long *aNum = new long[alen];
  long *bNum = new long[alen];

  
  cout << endl;
  for (int i = 0; i < alen; i++) {
    aNum[i] = 0;
    bNum[i] = 0;
  }
  cout << endl;

 
  //cout << s1.length() << endl;
  for (int i = 0; i < s1.length(); i++) {
    aNum[i] = ((int)s1[slen-1-i]-0X30);
    aNum[alen-1-i] = 0;
    bNum[i] = ((int)s2[slen-1-i]-0X30);
    bNum[alen-1-i] = 0;
  }

  
  aNum[0] = 34;
  aNum[1] = 12;
  aNum[2] = 0;
  aNum[3] = 0;

  bNum[0] = 78;
  bNum[1] = 56;
  bNum[2] = 0;
  bNum[3] = 0;
  
  cout << endl;
  for (int i = 0; i < alen; i++) {
    cout << aNum[i];
  }
  cout << endl;

  cout << endl;
  for (int i = 0; i < alen; i++) {
    cout << bNum[i];
  }
  cout << endl;
  
  cout << endl;
  long* aNtt = new long[alen];
  long* bNtt = new long[alen];
  // transform number "a"
  transform(aNum, aNtt, vec, alen, M);
  
  cout << "writing transform: " << endl;
  for (int i = 0; i < alen; i++) {
    cout << setw(5) << aNtt[i];
  }
  cout << endl;
  
  // transform number "b"
  transform(bNum, bNtt,  vec, alen, M);
  
  cout << "writing transform: " << endl;
  for (int i = 0; i < len; i++) {
    cout << setw(5) << bNtt[i];
  }  
  cout << endl;
  

  // elementwise multiplication of results
  long* cNtt = new long[alen];
  mulVector(aNtt, bNtt, cNtt, alen, M);
  /*
  cout << "writing transform: " << endl;
  for (int i = 0; i < len; i++) {
    cout << setw(5) << cNtt[i];
  }  
  cout << endl;
  */
  // find modulo inverse of "N" and "w"
  int invN = iN(alen, M);
  int invUnity = iUnity(w, M);
  cout << "Inverse: unity " << invUnity << endl;
  cout << "Inverse: N     " <<  invN << endl;

  // find inverse transform
  long* rNtt = new long[len];
  itransform(cNtt, rNtt, ivec, len, M, invN);
  long res = 0;
  long r = 1;
  for (int i = 0; i < len; i++) {
    res += rNtt[i]*r;
    r = r*100;
    cout << dec << setw(5) << rNtt[i];
  }
  cout << endl;
  cout << "result: " << res << endl;
  
  vector<int> v1,v2,v3;
  v1.insert(v1.begin(), readVector.rbegin(), readVector.rend());
  v2.insert(v2.begin(), readVector.rbegin(), readVector.rend());
  cout << "Data length: " << readVector.size() << endl;
  cout << "Poly length: " << readVector.size()*2 << endl;
  printNumber(v1);
  cout <<endl;
  addNumber(v1,v2,v3);
  //printNumber(v3);
  cout << endl;
  mulNumber(v1, v2, v3);
  printNumber(v3);
  cout << endl;
  
  return 0;
}
