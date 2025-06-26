#ifndef ALG_H
#define ALG_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
using namespace std;

class Alg : OneMax
{
public:
    void RunALG(int, int, int, double);
    void Reset();
    std::vector<int> Init(); // ✅ 改成回傳 vector<int>
    int Evaluation(const std::vector<int>& sol);

private:
    // Input from Command-line Argument
    int Bit;
    int Run;
    int Iter;
    double rate;

    // calculate the evaluation

};

#endif