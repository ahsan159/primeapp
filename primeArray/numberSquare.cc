// Copyright by Muhammad Ahsan [2022]

// 03 September, 202 09:30:00 AM
// This file will calculate the square of prime number quickly. By exploiting shift
// and rotate

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>

using std::string;
using std::endl;
using std::cout;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::iterator;
using std::chrono::system_clock;
using std::ctime;
using std::time_t;
using std::setfill;
using std::setw;
using std::hex;
using std::dec;
using std::for_each;
using std::stringstream;

int readPrevious(vector<int>*, string);
inline void shiftLeft(vector<int>*);

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Invalid input format" << endl;
        exit(0);
    }
    vector<int> squareNumber;

    cout << "program exiting" << endl;
    return 0;
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