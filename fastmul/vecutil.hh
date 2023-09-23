#ifndef __VECUTIL_HH_
#define __VECUTIL_HH_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

void bcdAdd(vector<int>&,vector<int>&,vector<int>&);
void bcdSubtract(vector<int>&, vector<int>&, vector<int>&);
int bcdAddInteger(int, int, int&, int);
int bcdSubtractInteger(int&, int&);
int compare(vector<int>&, vector<int>&);
void cleanVector(vector<int>&);
vector<int> mul10(vector<int>&);
int bcdMul(int&, int, int&, int);
void mulVec(vector<int>&, vector<int>&, vector<int>&);
int isZero(vector<int>*);
void binaryAddition(vector<int>*);
void binarySubtraction(vector<int>*);
int readPrevious(vector<int>*, string);

inline void printHex(vector<int>* input) {
  vector<int>::reverse_iterator itr = input->rbegin();
  while (itr != input->rend()) {
    cout <<hex << setw(8) << setfill('0') << *itr++;
  }
  cout <<endl;
}

inline void printHex(vector<int>* input, const char* str) {
  cout <<str <<":";
  printHex(input);
}

inline void printHexFile(vector<int>* input, const char* str) {
  ofstream inFile(str, std::ios::out);
  vector<int>::reverse_iterator itr = input->rbegin();
  while (itr != input->rend()) {
    inFile << hex <<setw(8) <<setfill('0') << *itr++;
  }
  inFile.close();
}

inline void printHexFileDabble(vector<int>* input, const char* str, int inP) {
  ofstream inFile(str, std::ios::out);
  inFile << inP << endl;
  vector<int>::reverse_iterator itr = input->rbegin();  
  while(itr != input->rend()) {
    inFile << hex << setw(8) << setfill('0') << *itr++;
  }
  inFile.close();
}


inline int bcdAddInteger(int input1, int input2, int & result ,int c = 0) {
  //  cout << hex << input1 << endl;
  //  cout << hex << input2 << endl;
  //cout << "Adding Integer " << endl;
  int carry = 0;
  unsigned int buf1[8] = {0};
  unsigned int buf2[8] = {0};
  unsigned int buf[8]  = {0};

  buf1[0] = 0X0000000F&input1;
  buf1[1] = 0X000000F0&input1;
  buf1[2] = 0X00000F00&input1;
  buf1[3] = 0X0000F000&input1;
  buf1[4] = 0X000F0000&input1;
  buf1[5] = 0X00F00000&input1;
  buf1[6] = 0X0F000000&input1;
  buf1[7] = 0XF0000000&input1;

  buf2[0] = 0X0000000F&input2;
  buf2[1] = 0X000000F0&input2;
  buf2[2] = 0X00000F00&input2;
  buf2[3] = 0X0000F000&input2;
  buf2[4] = 0X000F0000&input2;
  buf2[5] = 0X00F00000&input2;
  buf2[6] = 0X0F000000&input2;
  buf2[7] = 0XF0000000&input2;

  buf[0] = buf1[0]+buf2[0] + c;
  if (buf[0]>0X00000009) {
    buf[0] = (buf[0] + 0X00000006)&0X0000000F;
    buf[1] = buf[1] + 0X00000010;
  }
  buf[1] = buf[1]+buf1[1]+buf2[1];
  if (buf[1]>0X00000090) {
    buf[1] = (buf[1] + 0X00000060)&0X000000F0;
    buf[2] = buf[2] + 0X00000100;
  }
  buf[2] = buf[2]+buf1[2]+buf2[2];
  if (buf[2]>0X00000900) {
    buf[2] = (buf[2] + 0X00000600)&0X00000F00;
    buf[3] = buf[3] + 0X00001000;
  }
  buf[3] = buf[3]+buf1[3]+buf2[3];
  if (buf[3]>0X00009000) {
    buf[3] = (buf[3] + 0X00006000)&0X0000F000;
    buf[4] = buf[4] + 0X00010000;
  }
  buf[4] = buf[4]+buf1[4]+buf2[4];
  if (buf[4]>0X00090000) {
    buf[4] = (buf[4] + 0X00060000)&0X000F0000;
    buf[5] = buf[5] + 0X00100000;
  }
  buf[5] = buf[5]+buf1[5]+buf2[5];
  if (buf[5]>0X00900000) {
    buf[5] = (buf[5] + 0X00600000)&0X00F00000;
    buf[6] = buf[6] + 0X01000000;
  }
  buf[6] = buf[6]+buf1[6]+buf2[6];
  if (buf[6]>0X09000000) {
    buf[6] = (buf[6] + 0X06000000)&0X0F000000;
    buf[7] = buf[7] + 0X10000000;
  }
  buf1[7] = (buf1[7]>>4)&0X0F000000;
  buf2[7] = (buf2[7]>>4)&0X0F000000;
  buf[7]  = (buf[7] >>4)&0X0F000000;
  buf[7] = buf1[7] + buf2[7] + buf[7];
  if (buf[7] > 0X09000000) {
    buf[7] += 0X06000000;
    buf[7] = (buf[7]<<4)&0XF0000000;
    carry = 1;    
  } else {
    buf[7] = (buf[7]<<4)&0XF0000000;
  }
  result = buf[0]|buf[1]|buf[2]|buf[3]|buf[4]|buf[5]|buf[6]|buf[7];
  //cout << dataOutput << endl;
  //cout << "exiting" << endl;
  return carry;
}

inline void shiftLeft(vector<int>* input) {
  // cout <<"shift left program" <<endl;
  int buf = 0X00;
  int bufOld = 0X00;
  vector<int>::iterator itr = input->begin();
  while (itr != input->end()) {
    buf = *itr&0X80000000;
    buf = (buf >> 31)&0X01;
    *itr = (*itr << 1)&0XFFFFFFFE;
    *itr |= bufOld;
    bufOld = buf;
    // cout <<hex <<*itr <<endl;
    itr++;
  }
}

inline vector<int> mulX(vector<int>& in, int val) {
  vector<int> output;
  vector<int>::iterator itr = in.begin();
  int carry = 0;
  while (itr != in.end()) {
    int value = 0X00000000;
    carry = bcdMul(*itr, val, value, carry);
    output.push_back(value);
    itr++;
  }
  if (carry > 0) {
    output.push_back(carry);
  }
  return output;
}

inline int bcdMul(int& m1, int val, int& output, int carryIn = 0) {
  int carry = 0;
  unsigned int buf1[8] = {0};
  unsigned int buf[8]  = {0};
  buf1[0] = m1      &0X0000000F;
  buf1[1] = (m1>>4 )&0X0000000F;
  buf1[2] = (m1>>8 )&0X0000000F;
  buf1[3] = (m1>>12)&0X0000000F;
  buf1[4] = (m1>>16)&0X0000000F;
  buf1[5] = (m1>>20)&0X0000000F;
  buf1[6] = (m1>>24)&0X0000000F;
  buf1[7] = (m1>>28)&0X0000000F;

  int in2 = carryIn;
  buf[0] = in2 + (buf1[0]*val);
  in2 = buf[0]/10;
  buf[0] = buf[0]%10;
  buf[1] = in2 + (buf1[1]*val);
  in2 = buf[1]/10;
  buf[1] = buf[1]%10;
  buf[2] = in2 + (buf1[2]*val);
  in2 = buf[2]/10;
  buf[2] = buf[2]%10;
  buf[3] = in2 + (buf1[3]*val);
  in2 = buf[3]/10;
  buf[3] = buf[3]%10;
  buf[4] = in2 + (buf1[4]*val);
  in2 = buf[4]/10;
  buf[4] = buf[4]%10;
  buf[5] = in2 + (buf1[5]*val);
  in2 = buf[5]/10;
  buf[5] = buf[5]%10;
  buf[6] = in2 + (buf1[6]*val);
  in2 = buf[6]/10;
  buf[6] = buf[6]%10;
  buf[7] = in2 + (buf1[7]*val);
  carry = buf[7]/10;
  buf[7] = buf[7]%10;

  buf[0] = (buf[0]<<0 )&0X0000000F;
  buf[1] = (buf[1]<<4 )&0X000000F0;
  buf[2] = (buf[2]<<8 )&0X00000F00;
  buf[3] = (buf[3]<<12)&0X0000F000;
  buf[4] = (buf[4]<<16)&0X000F0000;
  buf[5] = (buf[5]<<20)&0X00F00000;
  buf[6] = (buf[6]<<24)&0X0F000000;
  buf[7] = (buf[7]<<28)&0XF0000000;

  output = buf[0]|buf[1]|buf[2]|buf[3]|buf[4]|buf[5]|buf[6]|buf[7];
  return carry;
}

inline int bcd2Normal(int in) {
  int result = 0;
  int buffer;
  for (int i = 0,j = 1; i < 8; ++i,j*=10) {
    buffer = (in>>4*i)&0XF;
    result += buffer*j;
  }
  return result;
}

inline void incVector(vector<int>* input) {
  int carry = 0X00, result;
  vector<int>::iterator itr = input->begin();  
  carry = bcdAddInteger(*itr,1,result,carry);
  *itr = result;
  itr++;
  while (itr != input->end()) {
    carry = bcdAddInteger(*itr,0,result,carry);
    *itr = result;
    itr++;
    if (carry == 0) {
      break;
    }
  }
  if (carry) {
    input->push_back(carry);
  }
}

inline void decVector(vector<int>* input) {
  int carry = 0X00, result;
  vector<int> ::iterator itr = input->begin();
  while(itr != input->end()) {
    carry = bcdAddInteger(*itr,0X99999998,result,carry);
    //cout << "carry: " << carry << "\tresult: " << hex <<result << endl;
    if (carry) {
      *itr = ++result;
      break;
    } else {
      if (result == (int)0X99999999) {
	*itr = 0X99999999-result;
	break;
      } else {
	*itr = ++result;	
      }
    }
    itr++;
  }
}

#endif
