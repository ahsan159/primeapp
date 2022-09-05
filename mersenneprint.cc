// Copyright by Muhammad Ahsan [2018]

// 17 April, 2018 09:30:00 AM
// This program will read dabble file and convert
// into mersenne format of 100 character per
// line.

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
void printMersenneHex(vector<int>*, const char*);

int main(int argc, char *argv[])
{
  string inFileName("input.txt"), outFileName("output.txt");
  if (argc != 3 ) {
    cout << "Invalid Format!!!" << endl;
    cout << "./mersenneprint <infile.txt> <outfile.txt> " << endl;
    exit(0);
  } else {
    inFileName = std::string(argv[1]);
    outFileName = std::string(argv[2]);
  }
  auto start = system_clock::now();
  time_t start_t = system_clock::to_time_t(start);
  cout << ctime(&start_t) << endl;

  cout << "Program Started" << endl;
  vector<int> num;
  cout << "Number read: " << readPrevious(&num, inFileName) << endl;
  printMersenneHex(&num, outFileName.c_str());
  
  auto endP = system_clock::now();
  time_t endP_t = system_clock::to_time_t(endP);
  cout << ctime(&endP_t) << endl;
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

inline void printMersenneHex(vector<int>* input, const char* str) {
  stringstream ss;
  vector<int>::reverse_iterator itr = input->rbegin();
  ss << hex << *itr++;
  while (itr != input->rend()){
    ss << hex << setw(8) << setfill('0') << *itr++;
  }
  int i = 0;
  ofstream outFile(str, std::ios::out);
  string strChar(ss.str());
  for (auto& x:strChar) {
    outFile << x;
    if (++i == 100) {
      outFile << endl;
      i = 0;
    }
  }
  outFile.close();
}
