#ifndef ALG_H
#define ALG_H

#include "OneMax.cpp"
#include <iostream>
#include <vector>
using namespace std;

// 學生（解）結構
struct Student {
    double x, y;
    double value;
};

class Alg : OneMax
{
public:
    void RunALG(int, int, int, double, int);
    void Reset();
    std::vector<int> Init(); // ✅ 改成回傳 vector<int>
    //int Evaluation(const std::vector<int>& sol);
    double Evaluation(const std::vector<double>& vec, int dim);
    
    Student global_best; // 全域最佳解
    
private:
    // Input from Command-line Argument
    int Bit;
    int Run;
    int Iter;
    double rate;
    int dim; // 維度

    // calculate the evaluation
    
};

#endif