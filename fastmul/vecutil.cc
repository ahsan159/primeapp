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

using namespace std;
using namespace std::chrono;


void binaryAddition(vector<int>* input) {
  // printHex(input, "Before");
  int carry = 0X00;
  vector<int>::iterator itr = input->begin();
  if (*itr == (int)0XFFFFFFFF) {
    carry = 1;
    *itr = 1 + *itr;
  } else {
    *itr = 1 + *itr;
    carry = 0;
  }
  itr++;
  while (carry == 1) {
    if (*itr == (int)0XFFFFFFFF) {
      *itr = 1 + *itr;
      carry = 1;
      if ((itr+1) == input->end()) {
        input->push_back(1);
        carry = 0;
      }
    } else {
      *itr = 1 + *itr;
      carry = 0;
    }
    itr++;
  }
  // printHex(input, "After");
}

void  binarySubtraction(vector<int>* input) {
  int carry = 0X00;
  vector<int>::iterator itr = input->begin();
  if (*itr == 0X00) {
    carry = 1;
    *itr = -1 + *itr;
  } else {
    carry = 0;
    *itr = -1 + *itr;
  }
  itr++;
  while (carry == 1) {
    if (*itr == 0) {
      *itr = -1 + *itr;
      carry = 1;
    } else {
      carry = 0;
      *itr = -1 + *itr;
    }
    itr++;
  }
}

int isZero(vector<int>* input) {
  int output = 0;
  vector<int>::iterator itr = input->begin();
  while (itr != input->end()) {
    if (*itr != 0X00) {
      output++;
    }
    itr++;
  }
  return output;
}


int readPrevious(vector<int>* output, string file) {
  int outPrime = 0;
  ifstream infile(file.c_str(),std::ios::in);
  if(!infile) {
    cout <<"Unable to read file!!!" << file << endl;
    exit(0);
  }
  string data,input;
  try {
    getline(infile, input);
    getline(infile, data);
    outPrime = atoi(input.c_str());
    size_t sTotal = data.size() - 1 ;
    //cout << data << endl;
    //for_each(data.begin(), data.end(), [](auto& x){x=x-0X30;});
    for(int i = sTotal;i > 0;) {
      int buf[8] = {0};
      buf[0] = ((int)data[i--]<< 0)&0X0000000F;
      buf[1] = ((int)data[i--]<< 4)&0X000000F0;
      buf[2] = ((int)data[i--]<< 8)&0X00000F00;
      buf[3] = ((int)data[i--]<<12)&0X0000F000;
      buf[4] = ((int)data[i--]<<16)&0X000F0000;
      buf[5] = ((int)data[i--]<<20)&0X00F00000;
      buf[6] = ((int)data[i--]<<24)&0X0F000000;
      buf[7] = ((int)data[i--]<<28)&0XF0000000;
      output->push_back(buf[0]|buf[1]|buf[2]|buf[3]|buf[4]|buf[5]|buf[6]|buf[7]);
    }
  } catch(std::exception& e) {
    cout <<"Unable to read file" << file <<endl;
    exit(0);
  }
  return outPrime;
}

void bcdAdd(vector<int>& in1, vector<int>& in2, vector<int>& outputSum) {
  outputSum.clear();
  vector<int> input1(in1);
  vector<int> input2(in2);
  size_t size1 = input1.size();
  size_t size2 = input2.size();
  if (size1 != size2) {
    if (size1 > size2) {
      while (input2.size() < size1) {
	input2.push_back(0);
      }
    } else if (size2 > size1) {
      while (input1.size() < size2) {
	input1.push_back(0);
      }
    }
  }
  //input1.push_back(0);
  //input2.push_back(0);
  vector<int>::iterator itr1 = input1.begin();
  vector<int>::iterator itr2 = input2.begin();
  int carry = 0;
  while (itr1!= input1.end() || itr2!=input2.end()) {
    int sData = 0;
    carry = bcdAddInteger(*itr1, *itr2, sData, carry);
    outputSum.push_back(sData);
    itr1++;
    itr2++;
  }
  if (carry) {
    outputSum.push_back(0X00000001);
  }  
}


int compare(vector<int>&input1, vector<int>& input2)
{
  cleanVector(input1);
  cleanVector(input2);
  if(input1.size() > input2.size()) {
    return 1;
  } else if(input1.size() < input2.size()) {
    return -1;
  } else if (input1.size() == input2.size()) {
    vector<int>::reverse_iterator itr1 = input1.rbegin();
    vector<int>::reverse_iterator itr2 = input2.rbegin();
    while ((unsigned int)*itr1 == (unsigned int)*itr2) {
      itr1++;
      itr2++;
      if (itr1 == input1.rend()) {
	return 0;
      }
    }
    if ((unsigned int)*itr1 > (unsigned int)*itr2) {
      return 1;
    } else if ((unsigned int)*itr1 < (unsigned int)*itr2) {
      return -1;
    }
  }
  return 0;
}



void bcdSubtract(vector<int>& num1, vector<int>& num2, vector<int>& output) {
  output.clear();
  //printHex(&in1, "A: ");
  //printHex(&in2, "B: ");
  vector<int> in1(num1), in2(num2);
  int cmp = compare(in1, in2);
  if (in1.size() != in2.size() )  {
    if (in1.size() > in2.size()) {
      while (in1.size( ) != in2.size()) {
	in2.push_back(0);
      }
    } else if(in2.size() > in1.size()) {
      while (in1.size() != in2.size()) {
	in1.push_back(0);
      }
    }
  }      
  //cout << "Sub:" << dec <<cmp << endl;
  if (cmp == 1) {
    //cout <<"Simple: " << endl;
    for_each(in2.begin(), in2.end(), [](auto& x){x = 0X99999999 - x;});   
    bcdAdd(in1,in2,output);    
    incVector(&output);
    output.pop_back();
    for_each(in2.begin(), in2.end(), [](auto& x){x = 0X99999999 - x;});
  } else if (cmp == -1) {
    //cout << "Other: " << endl;
    for_each(in1.begin(), in1.end(), [](auto& x){x = 0X99999999 - x;});
    bcdAdd(in2,in1,output);    
    incVector(&output);
    output.pop_back();
    for_each(in1.begin(), in1.end(), [](auto& x){x = 0X99999999 - x;});
  } else if (cmp == 0) {
    output.push_back(0X00000000);
  } else {
    cout <<"Faulty inputs: " << endl;
    exit(0);
  }
}

void cleanVector(vector<int>& in) {
  while(*(in.rbegin()) == 0) {
    in.pop_back();
  }
}

vector<int> mul10(vector<int>& input) {
  vector<int> output(input);
  if ((unsigned int)*(output.rbegin()) > 0X00000000) {
    output.push_back(0X00000000);
  }
  int carry = 0, buffer;
  vector<int>::iterator itr = output.begin();
  while (itr != output.end()) {
    buffer = *itr;
    *itr = (*itr<<4)|carry;
    buffer = buffer>>28;
    carry = buffer&0XF;
    itr++;
  }
  cleanVector(output);
  return output;
}

void mulVec(vector<int>& in1, vector<int>& in2, vector<int>& output) {
  vector<int> num1(in1), num2(in2), buffer(num2);
  output.clear();
  vector<int> outSum(0),higherMul(num1), bufX, buf10;
  
  vector<int>::iterator itr = num2.begin();
  while (itr!= num2.end()) {
    int val1 = *itr&0XF;
    int val2 = *itr&0XF0;
    int val3 = *itr&0XF00;
    int val4 = *itr&0XF000;
    int val5 = *itr&0XF0000;
    int val6 = *itr&0XF00000;
    int val7 = *itr&0XF000000;
    int val8 = *itr&0XF0000000;

    val2 = (val2>>4)&0XF;
    val3 = (val3>>8)&0XF;
    val4 = (val4>>12)&0XF;
    val5 = (val5>>16)&0XF;
    val6 = (val6>>20)&0XF;
    val7 = (val7>>24)&0XF;
    val8 = (val8>>28)&0XF;
	
    
    bufX = mulX(higherMul,val1);    
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val2);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val3);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val4);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val5);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val6);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val7);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;
    
    bufX = mulX(higherMul,val8);
    bcdAdd(bufX,outSum,buffer);
    buf10 = mul10(higherMul);
    higherMul = buf10;
    outSum = buffer;

    //higherMul.insert(higherMul.begin(),0);
    itr++;
  }
  output.assign(outSum.begin(), outSum.end());
  cleanVector(output);
}



