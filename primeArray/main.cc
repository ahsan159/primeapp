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
//#include <omp.h>
#include <csignal>
#include <unistd.h>

#include "vecutil.hh"

using namespace std;
using namespace std::chrono;

void doubleDabble(int);
void shiftLeft(vector<int>*);
int add3Integer(int);
void printHex(vector<int>*);
void printHex(vector<int>*, const char*);
void printHexFile(vector<int>*, const char*);
void printHexFileDabble(vector<int>*, const char*, int);
vector<int>* binaryPower(vector<int>*);
void binaryAddition(vector<int>*);
void binarySubtraction(vector<int>*);
int isZero(vector<int>*);
int readPrevious(vector<int>*, string);
void increaseDabble(vector<int>&, int);
void doubleDabble(vector<int>&, int);
void setSignal(int);
int isPrime(int);
void printArray(vector<int>*,int*);

volatile sig_atomic_t CtrlCSignal;

int primeP;
int pDabble;

int main(int argc, char *argv[]) {
  signal(SIGINT, setSignal);
  CtrlCSignal = 0;
  string outputFile = "primeAppOutput.txt";
  primeP = 1;
  string inputFile = "input.txt";
  int runMode = -1;
  if (argc == 1) {
    cout <<"1 Apr 2018" << endl;
    cout <<"For new calculation use ./primeApp -n <number> -f <optional_output>" <<endl;
    cout <<"For old calculation use ./primeApp -p <number> <input_file> -f <optional_output>" <<endl;
    exit(0);
  } else if (argc == 3) {
    primeP = atoi(argv[2]);
    runMode = 1;
  } else if (argc == 5) {
    primeP = atoi(argv[2]);
    outputFile = argv[4];
    runMode = 1;
  } else if (argc == 4) {
    primeP = atoi(argv[2]);
    inputFile = argv[3];
    runMode = 0;
  } else if (argc == 6) {
    primeP = atoi(argv[2]);
    inputFile = argv[3];
    outputFile = argv[5];
    runMode = 0;
  } else {
    cout <<"Invalid Input Format!!!" << endl;
    exit(0);
  }  
  auto currTime = system_clock::now();
  time_t currTime_t = system_clock::to_time_t(currTime);
  cout << ctime(&currTime_t);
  cout <<"this is the program start: " << INT_MAX << endl;
  //int primeP = atoi(argv[1]);
  vector<int> read;
  if (runMode == -1) {
    cout <<"Unhandled Error!!!" << endl;
    exit(0);
  } else if (runMode == 1) {
    doubleDabble(read,primeP);
    cout <<"Now writting file....";
    printHexFileDabble(&read, outputFile.c_str(), primeP);
    cout<< "done" << endl;
  } else if (runMode == 0) {
    //doubleDabble(primeP);
    pDabble = readPrevious(&read, inputFile.c_str());
    //printHex(&read);
    cout<< "Dabble read for prime: " << pDabble << endl;
    auto readTime = system_clock::now();
    std::chrono::duration<double> readTimeC = readTime - currTime;
    cout << "Time to read Dabble: " << readTimeC.count() << endl;
    increaseDabble(read,primeP - pDabble);
    cout <<"Now writting file....";
    printHexFileDabble(&read, outputFile.c_str(), primeP);
    cout << "done" << endl;
  }

  cout <<read.size()<<endl;

  int *array;
  array = new int(read.size());
  printArray(&read,array);
  for (int i = read.size()-1;i>=0;i--) {
    cout << setw(8) << setfill('0') <<array[i];
  }
  cout << endl;
  
  auto currTime2 = system_clock::now();
  time_t currTime2_t = system_clock::to_time_t(currTime2);
  cout << ctime(&currTime2_t);
  return 0;
}

inline void printArray(vector<int>* input, int* output) {
  vector<int>::iterator itr = input->begin();
  int i = 0;
  cout << "before while" <<endl;
  while(itr != input->end()) {
    char buf[9];
    int ibuf;
    //cout <<"here1" <<endl;
    sprintf(buf,"%X",*itr);
    //cout <<"here2" <<endl;
    sscanf(buf,"%d",&ibuf);
    //cout <<"here3" <<endl;
    output[i++] = ibuf;
    itr++;
  }
  cout << "conversion done" <<endl;
}

void doubleDabble(int input) {
  vector<int> output;
  output.push_back(0);
  int i = 1;
  int max_iterations = input;
  double tadd = 0;
  double tshift = 0;
  while(i++ < max_iterations) {
    auto loopStart = system_clock::now();
    output.at(0) = output.at(0)|1;
    for_each(output.begin(), output.end(), [](auto& x){x=add3Integer(x);});
    auto loopadd = system_clock::now();
    std::chrono::duration<double> addCount = loopadd - loopStart;
    tadd += addCount.count();
    shiftLeft(&output);
    if (output.at(output.size() - 1) > 0X40000000) {
	output.push_back(0);
    }
    auto loopshift = system_clock::now();
    std::chrono::duration<double> shiftCount = loopshift - loopStart;
    tshift += shiftCount.count();
  }
  //cout << endl;
  output.at(0) = output.at(0)|1;
  //cout << input << endl;
  //printHex(&output);
  cout << "Add   time: " << tadd << endl;
  cout << "Shift time: " << tshift << endl;
}

void doubleDabble(vector<int>& output,int input) {
  output.push_back(0);
  int i = 1;
  int max_iterations = input;
  double tadd = 0;
  double tshift = 0;
  while(i++ < max_iterations) {
    auto loopStart = system_clock::now();
    output.at(0) = output.at(0)|1;
    for_each(output.begin(), output.end(), [](auto& x){x=add3Integer(x);});
    auto loopadd = system_clock::now();
    std::chrono::duration<double> addCount = loopadd - loopStart;
    tadd += addCount.count();
    shiftLeft(&output);
    if (output.at(output.size() - 1) > 0X40000000) {
	output.push_back(0);
    }
    auto loopshift = system_clock::now();
    std::chrono::duration<double> shiftCount = loopshift - loopStart;
    tshift += shiftCount.count();
  }
  output.at(0) = output.at(0)|1;
  //printHex(&output);
  cout << "Add   time: " << tadd << endl;
  cout << "Shift time: " << tshift << endl;
}

void increaseDabble(vector<int>& output, int inc) {
  int i = 0;
  int max_iterations = inc;
  while(i++ < max_iterations) {
    //auto t1 = system_clock::now();
    output.at(0) = output.at(0)|1;
    for_each(output.begin(),output.end(), [](auto& x){x=add3Integer(x);});
    shiftLeft(&output);
    if (output.at(output.size() - 1) > 0X40000000) {
      output.push_back(0);
      /*
      if (i%100000 == 0) {
	cout << i << " of " << max_iterations << " performed" << endl;
      }
      */
    }
    //auto t2 = system_clock::now();
    //std::chrono::duration<double> tDiff = t2 - t1;
    //cout << "Shift Time: " << i << "\t" << tDiff.count() << " ms" << endl;
    //cout << i << ":" << max_iterations << endl;
    if (CtrlCSignal == 1) {
      cout << i << endl;
      primeP = pDabble + i;
      break;
    }
    if (isPrime(pDabble + i)) {
      stringstream ss;
      ss << "autoprime"<<pDabble+i<<".txt";
      string str = ss.str();
      vector<int> pPrint(output);
      pPrint.at(0) = pPrint.at(0)|1;
      cout << "Printing prime file: " << ss.str() << " ... ";
      printHexFileDabble(&pPrint,str.c_str(),pDabble+i);
      cout << "done" << endl;
    }        
  }
  output.at(0) = output.at(0)|1;
  //printHex(&output);
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

void setSignal(int) {
  CtrlCSignal = 1;
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

vector<int>* binaryPower(vector<int>* input) {
  // this function will implement
  // 2^P(i) - 1 = P(i+1)
  cout << "Binary Power Function" <<endl;
}

void binaryAddition(vector<int>* input) {
  // printHex(input, "Before");
  int carry = 0X00;
  vector<int>::iterator itr = input->begin();
  if (*itr == 0XFFFFFFFF) {
    carry = 1;
    *itr = 1 + *itr;
  } else {
    *itr = 1 + *itr;
    carry = 0;
  }
  itr++;
  while (carry == 1) {
    if (*itr == 0XFFFFFFFF) {
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

inline int add3Integer(int in)
{
  int out = 0X00;
  unsigned int add[8] = {0};

  add[0] = in&0X0000000F;
  add[0] = add[0] > 0X00000004?add[0]+0X00000003:add[0];

  add[1] = in&0X000000F0;
  add[1] = add[1] > 0X00000040?add[1]+0X00000030:add[1];

  add[2] = in&0X00000F00;
  add[2] = add[2] > 0X00000400?add[2]+0X00000300:add[2];

  add[3] = in&0X0000F000;
  add[3] = add[3] > 0X00004000?add[3]+0X00003000:add[3];

  add[4] = in&0X000F0000;
  add[4] = add[4] > 0X00040000?add[4]+0X00030000:add[4];

  add[5] = in&0X00F00000;
  add[5] = add[5] > 0X00400000?add[5]+0X00300000:add[5];

  add[6] = in&0X0F000000;
  add[6] = add[6] > 0X04000000?add[6]+0X03000000:add[6];

  add[7] = in&0XF0000000;
  add[7] = add[7] > 0X40000000?add[7]+0X30000000:add[7];

  out = add[0]|add[1]|add[2]|add[3]|add[4]|add[5]|add[6]|add[7];
  return out;
  
}

int isPrime(int pCheck) {
  //int pCheck = atoi(argv[1]);
  int checkRange = ceil(sqrt(pCheck));
  if (pCheck < 4) {
    //cout << "Prime" << endl;
    return 0;
  }
  if (pCheck%2 == 0) {
    //cout <<"Not Prime" << endl;
    return 0;
  }
  int isPrime = 1;
  for (int i = 3; i < checkRange; i+=2) {
    if (pCheck%i == 0) {
      isPrime = 0;
      break;
    }
  }
  return isPrime;
}
