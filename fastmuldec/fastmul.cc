// Copyright by Muhammad Ahsan [2025]

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <climits>
#include <omp.h>

#include "utils.hh"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    string fileName = "nprime127.txt";
    if (argc == 2)
    {
        // cout << argv[1] << endl;
        fileName.clear();
        fileName.append(argv[1]);
    }

    auto currTime = system_clock::now();
    time_t currTime_t = system_clock::to_time_t(currTime);
    cout << ctime(&currTime_t);
    cout << "this is the program start: " << INT_MAX << endl;
    cout << "size of int: " << sizeof(int) << endl;

    vector<int> output;
    readPrevious(&output, fileName.c_str());

    for (vector<int>::reverse_iterator itr = output.rbegin(); itr != output.rend(); itr++)
    {
        cout << setw(8) << setfill('0') << *itr;
    }
    cout << endl;

    printVector(&output);
    cout << endl;

    auto currTime1 = system_clock::now();
    time_t currTime_t1 = system_clock::to_time_t(currTime1);
    cout << ctime(&currTime_t1);
    cout << "this is the program start: " << INT16_MAX << endl;
    cout << "size of int: " << sizeof(short) << endl;

    return 0;
}