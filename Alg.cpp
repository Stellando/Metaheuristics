#include "Alg.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate;

    cout << "bit: " << Bit << " run: " << Run << " iter: " << Iter << " rate: " << rate << endl;

    for (int i = 0; i < Run; i++)
    {
        int current = 0;
        int value = 0;
        cout << "Run: " << i + 1 << endl;

        
        std::ofstream fout("result_Run" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        std::vector<int> sol = Init(); // ✅ 正確接收 Init 結果

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;
            Evaluation(sol, value, current, fout); // ✅ 傳參數進去
        }

        fout.close();
    }
}

std::vector<int> Alg::Init()
{
    std::vector<int> sol(Bit);
    for (int k = 0; k < Bit; ++k) {
        sol[k] = rand() % 2;
    }
    return sol;
}

void Alg::Evaluation(std::vector<int>& sol, int& value, int& current, std::ofstream& fout)
{
    value = OneMaxProblem(sol, Bit);
    if (value > current) {
        current = value;
        cout << "Current Best Value: " << current << endl;
        fout << "Current Best Value: " << current << std::endl;
    }
}

void Alg::Reset()
{
    // 清除舊資料或狀態（目前為空實作）
}
