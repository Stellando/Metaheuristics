#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include <list>
#include <deque>
#include <set>

using namespace std;

// 儲存一個解（bit 串）及其適應度
struct Solution {
    std::vector<int> bits; // 解的 bit 向量
    int value;             // 此解的 OneMax 適應度

    Solution(int Bit) : bits(Bit), value(0) {}

    // 初始化隨機解
    void random_init() {
        for (auto& b : bits) {
            b = rand() % 2;
        }
    }
};

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate;

    cout << "bit: " << Bit << " run: " << Run << " iter: " << Iter << " rate: " << rate << endl;

    for (int i = 0; i < Run; i++)
    {
        cout << "Run: " << i + 1 << endl;
        std::ofstream fout("result_Tabu" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 1. 初始化隨機解
        Solution current(Bit);
        current.random_init();
        current.value = Evaluation(current.bits);

        Solution best = current;

        // 2. Tabu List 設定
        const int tabu_tenure = 10; // Tabu List 長度
        std::deque<std::vector<int>> tabu_list;

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // 3. 產生鄰域（bit flip）
            Solution best_candidate = current;
            bool found_better = false;

            for (int k = 0; k < Bit; ++k) {
                Solution neighbor = current;
                neighbor.bits[k] = 1 - neighbor.bits[k]; // 單點翻轉
                neighbor.value = Evaluation(neighbor.bits);

                // 檢查是否在 Tabu List
                bool is_tabu = false;
                for (const auto& tabu_bits : tabu_list) {
                    if (neighbor.bits == tabu_bits) {
                        is_tabu = true;
                        break;
                    }
                }

                // Aspiration: 若比目前 best 還好則允許
                if ((!is_tabu && neighbor.value > best_candidate.value) ||
                    (neighbor.value > best.value)) {
                    best_candidate = neighbor;
                    found_better = true;
                }
            }

            // 4. 移動到最佳鄰居
            current = best_candidate;

            // 5. 更新全域最佳
            if (current.value > best.value) {
                best = current;
            }

            // 6. 更新 Tabu List
            tabu_list.push_back(current.bits);
            if (tabu_list.size() > tabu_tenure)
                tabu_list.pop_front();

            // 7. 輸出
            fout << nfes << " " << best.value << std::endl;
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
/*
// Ackley function 評分
double Alg::Evaluation(double x, double y)
{
    const double pi = 3.14159265358979323846;
    double term1 = -20.0 * exp(-0.2 * sqrt(0.5 * (x * x + y * y)));
    double term2 = -exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y)));
    return term1 + term2 + exp(1.0) + 20.0;
}
*/

int Alg::Evaluation(const std::vector<int>& sol)
{
    return OneMaxProblem(sol, Bit);
}

void Alg::Reset()
{
    // 清除舊資料或狀態（目前為空實作）
}
