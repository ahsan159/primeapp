// Copyright [2025] by Muhammad Ahsan
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <iterator>

#include "utils.hh"

using namespace std;
using namespace std::chrono;

void printMsg(string str)
{
    cout << str << endl;
}

int readPrevious(vector<int> *output, string file)
{
    int outPrime = 0;
    ifstream infile(file.c_str(), std::ios::in);
    if (!infile)
    {
        cout << "Unable to read file!!!" << file << endl;
        exit(0);
    }
    string data, input;
    try
    {
        getline(infile, input);
        getline(infile, data);
        outPrime = atoi(input.c_str());
        size_t sTotal = data.size() - 1;
        // cout << data << endl;
        for_each(data.begin(), data.end(), [](auto &x)
                 { x = x - 0X30; });
        for (int i = sTotal; i > 0;)
        {
            int buf[8] = {0};
            buf[0] = ((int)data[i--] * 1);
            buf[1] = ((int)data[i--] * 10);
            buf[2] = ((int)data[i--] * 100);
            buf[3] = ((int)data[i--] * 1000);
            buf[4] = ((int)data[i--] * 10000);
            buf[5] = ((int)data[i--] * 100000);
            buf[6] = ((int)data[i--] * 1000000);
            buf[7] = ((int)data[i--] * 10000000);
            cout << buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7] << endl;
            output->push_back(buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]);
        }
    }
    catch (std::exception &e)
    {
        cout << "Unable to read file" << file << endl;
        exit(0);
    }
    return outPrime;
}

void printVector(vector<int> *input)
{
    for (vector<int>::reverse_iterator itr = input->rbegin(); itr != input->rend(); itr++)
    {
        cout << setw(8) << setfill('0') << *itr;
    }
}