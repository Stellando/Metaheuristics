#include "Alg.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

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
        double T = 100; // 初始溫度

        //建立輸出檔案
        std::ofstream fout("result_SA" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();
        // 初始化解答
        std::vector<int> sol = Init(); 

        //做Transition
        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // 備份舊解與舊值
            std::vector<int> old_sol = sol;
            int old_value = value;

            // 更改處
            int idx = rand() % Bit;
            sol[idx] = 1 - sol[idx];



            // 呼叫 Evaluation 評估新解表現
            int new_value = Evaluation(sol);

            if (new_value > old_value)
            {
                value = new_value;
                current = new_value;
                cout << "Current Best Value: " << current << endl;
                fout <<nfes<< " " << current << std::endl;
            }
            else
            {
                // 模擬退火法：以一定機率接受新解
                double prob = exp((new_value - old_value) / T);
                double r = (double)rand() / RAND_MAX;
                if (r < prob) {
                    value = new_value;
                    // current 不更新，因為不是最佳解
                    // 保留 sol
                } else {
                    // 表現沒變好且沒被接受，還原
                    sol = old_sol;
                    value = old_value;
                }
            }
            T *= rate; // 降低溫度
            /*
            // 表現沒變好，還原
            sol = old_sol;
            value = old_value;
            */
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

int Alg::Evaluation(const std::vector<int>& sol)
{
    return OneMaxProblem(sol, Bit);
}


void Alg::Reset()
{
    // 清除舊資料或狀態（目前為空實作）
}
