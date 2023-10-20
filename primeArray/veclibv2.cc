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
      buf[0] = ((int)data[i--]<<0)&0X0000000F;
      buf[1] = ((int)data[i--]<<4)&0X000000F0;
      buf[2] = ((int)data[i--]<<8)&0X00000F00;
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

void printDecimalVector(vector<unsigned int>* input, int N)
 {
    vector<unsigned int>::iterator itr = input->begin();
    while (itr!=input->end())
    {
        cout << dec << setw(N) << setfill('0') << *itr++;
    }
    return;
 }

 inline void printDecimalVectorFile(vector<unsigned int>* input, const char* str)
 {
    ofstream outFile(str,std::ios::out);
    vector<unsigned int>::iterator itr = input->begin();
    while (itr!=input->end())
    {
        outFile << dec << setw(8) << setfill('0') << *itr++;
    }
    return;
 }

void printDecimalVectorFileComma(vector<unsigned int>* input, const char* str,int N)
 {
    ofstream outFile(str,std::ios::out);
    vector<unsigned int>::iterator itr = input->begin();
    while (itr!=input->end())
    {
        outFile << dec << setw(N) << setfill('0') << *itr++ << ",";
    }
    return;
 }

void getDecimalVector(vector<int>* hexVector, vector<unsigned int>* decVector)
 {
    decVector->clear();
    vector<int>::reverse_iterator itr = hexVector->rbegin();
    while(itr != hexVector->rend())
    {
        stringstream numStream1;
        numStream1<<hex<<setw(8)<<setfill('0')<<*itr++;
        // cout << numStream1.str() << endl;
        unsigned int i;
        i = stoul(numStream1.str(),nullptr,10);
        // cout << "out" << dec << setw(8) << setfill('0') << i << endl;
        decVector->push_back(i);
    }
 }

 void addNumbers(vector<unsigned int>& num1, vector<unsigned int>& num2, vector<unsigned int>& result)
 {
    // result.assign(num1.size(),0);
    int maxSize = result.size();
    // cout << "size: " << maxSize << endl;
    int i;
    // omp_set_num_threads(4);
       
    // #pragma omp parallel for 
    // for (i = 0; i < maxSize; i++)
    // {
    //     // cout << i << endl;
    //     result[i] = num1[i] + num2[i];
    // }


    int tid;
    #pragma omp parallel num_threads(maxSize)
    {
      tid = omp_get_thread_num();
      result[tid] = num1[tid] + num2[tid];
    }
    // printDecimalVector(&result);
 }
 
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