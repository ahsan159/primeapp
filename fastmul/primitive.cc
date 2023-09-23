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

using namespace std;
using namespace std::chrono;

int numberInProcess = -1;
int maxRootOrder = -1;
vector<int> rootList;
vector<int> orderList;

void msg()
{
  cout << "This is the msg" << endl;
  return;
}

int processNumber(int number)
{
  numberInProcess = number;

  stringstream ss;
  ss << "./pCheck " << numberInProcess;
  
  FILE* primePipe(popen(ss.str().c_str(), "r"));

  string result;
  char buffer[128];
  if (!primePipe) {
    cout << "Cannot perform primility test" << endl;
    return -1;
  }

  while (fgets(buffer, sizeof(buffer), primePipe) != NULL) {
    result.append(buffer);
  }

  maxRootOrder = -1;
  if (result.find("Not") != string::npos) {
    maxRootOrder = -1;
  } else {
    maxRootOrder = numberInProcess - 1;
  }

  return 0;
}

int isRoot(int num)
{
  if (numberInProcess == -1) {
    return -1;
  }
  // 'num' is actually roots if they reach unity
  stringstream ss;
  ss << num;
  vector<int> unity, o, numVector;
  stringstream ssNIP;
  ssNIP << numberInProcess;
  numVector = readNumber(ssNIP.str().c_str());
  unity.push_back(0X00000001);
  vector<int> pNum = readNumber(ss.str());
  vector<int> mNum(pNum);
  for(int j = 1; j < numberInProcess; j++) {
    // 'j' is actually order of root
    o = modulo(numVector, mNum);
    if (isZero(&o)==0) {
      return -1;
    }
    if (compare(unity,o) == 0) {
      return j;
      if (maxRootOrder != -1 && j==maxRootOrder) {
	//cout << "\r" << i << " is primitive Root " << endl;
      }
      //cout << "\r found roots " << dec << root.size() << "/" << i  /*<< "\t\tat " << i */<< flush;
      break;
    }
    vector<int> multiplyVector;
    mulVec(o, pNum, multiplyVector);
    mNum = multiplyVector;
    //printHex(&mNum);
  }
  return -1;
}

int inverse(int number)
{
  for (long i = 1; i < numberInProcess; i++) {
    int p = (i*number)%numberInProcess;
    if (p == 1) {
      return i;
    }
  }
  return -1;
}

int rootOrder(int num)
{
  return -1;
}

int isPrimitive(int num)
{
  return -1;
}

int findAllRoots()
{
  return -1;
}

vector<int> readNumber(string str) {
  while (str.length()%8 != 0) {
    str = "0" + str;
  }
  //cout << str << endl;
  vector<int> output;
  for (int i = str.length() - 1; i >= 0;) {
    unsigned int i1 = str[i--]&0X0000000F;
    unsigned int i2 = str[i--]&0X0000000F;
    unsigned int i3 = str[i--]&0X0000000F;
    unsigned int i4 = str[i--]&0X0000000F;
    unsigned int i5 = str[i--]&0X0000000F;
    unsigned int i6 = str[i--]&0X0000000F;
    unsigned int i7 = str[i--]&0X0000000F;
    unsigned int i8 = str[i--]&0X0000000F; 
    i2 = i2<<4;
    i3 = i3<<8;
    i4 = i4<<12;
    i5 = i5<<16;
    i6 = i6<<20;
    i7 = i7<<24;
    i8 = i8<<28;
    int i0 = i1|i2|i3|i4|i5|i6|i7|i8;
    output.push_back(i0);
  }
  return output;
}

vector<int> modulo(vector<int>& a, vector<int>& b) {
  //printHex(&b);
  // determine b%a
  vector<int> num1(a), num2(b), output;
  output.push_back(0);
  if(compare(num1,num2)>0) {
    return num2;
  }
  while(compare(num1,num2) < 0 ) {
    num1.insert(num1.begin(), 0);
  }
  if (compare(num1,num2)==0) {
    return output;
  }
  num1.erase(num1.begin());
  //printHex(&num1);
  while (compare(num1,num2) < 0) {
    vector<int> num1buf[8];
    num1buf[0] = num1;
    num1buf[1] = mul10(num1buf[0]);
    num1buf[2] = mul10(num1buf[1]);
    num1buf[3] = mul10(num1buf[2]);
    num1buf[4] = mul10(num1buf[3]);
    num1buf[5] = mul10(num1buf[4]);
    num1buf[6] = mul10(num1buf[5]);
    num1buf[7] = mul10(num1buf[6]);
    //printHex(&num1buf[7]);
    
    for (int i = 7;i>=0;--i) {
      //cout << "comp: " ;
      int c = compare(num1buf[i],num2);
      //cout << "\t\t" << dec << c << "\t\t";
      //printHex(&num1buf[i]);
      if (c == 0) {
        return output;
      } else if(c < 0) {
        for(int j = 9; j > 0; --j) {
          vector<int> mulbuf(mulX(num1buf[i],j));         
          int c2 = compare(mulbuf,num2);
          //printHex(&mulbuf);
          if (c2==0) {
            //cout << j << "returning" <<endl;
            return output;
          } else if (c2 < 0) {
            vector<int> subbuf;
            //cout << j << endl;
            bcdSubtract(mulbuf,num2,subbuf);
            num2 = subbuf;
            break;
          }
        }
      }      
    }
    if (compare(num1,a) == 0) {
      //cout << "breaking" << endl;
      break;
    }
    num1.erase(num1.begin());    
  }
  return num2;
}
