#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

struct Individual {
    double x;      // 個體的 x 座標
    double y;      // 個體的 y 座標
    double value;  // 個體的適應度（Ackley function 值）

    Individual() {
        // 初始化於 Ackley 常見定義域 [-20, 20]
        x = ((double)rand() / RAND_MAX) * 40.0 - 20.0;
        y = ((double)rand() / RAND_MAX) * 40.0 - 20.0;
        value = std::numeric_limits<double>::infinity();
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
        std::ofstream fout("result_DE" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // ===== Differential Evolution (DE) 主流程開始 =====
        const int pop_size = 30; // 族群數量
        const double F = 0.8;    // 差分權重
        const double CR = 0.9;   // 交叉率

        // 1. 初始化族群
        std::vector<Individual> population(pop_size);
        for (int j = 0; j < pop_size; ++j) {
            population[j] = Individual();
            population[j].value = Evaluation(population[j].x, population[j].y);
        }

        // 2. 找出初始全域最佳
        Individual global_best = population[0];
        for (const auto& ind : population) {
            if (ind.value < global_best.value)
                global_best = ind;
        }

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // 3. DE主循環
            for (int j = 0; j < pop_size; ++j) {
                // (a) 隨機選三個不同的個體 r1, r2, r3，且都不等於 j
                int r1, r2, r3;
                do { r1 = rand() % pop_size; } while (r1 == j);
                do { r2 = rand() % pop_size; } while (r2 == j || r2 == r1);
                do { r3 = rand() % pop_size; } while (r3 == j || r3 == r1 || r3 == r2);

                // (b) 差分變異
                double trial_x = population[r1].x + F * (population[r2].x - population[r3].x);
                double trial_y = population[r1].y + F * (population[r2].y - population[r3].y);

                // (c) 邊界檢查
                trial_x = std::max(-20.0, std::min(20.0, trial_x));
                trial_y = std::max(-20.0, std::min(20.0, trial_y));

                // (d) 交叉
                double new_x = (rand() / (double)RAND_MAX < CR) ? trial_x : population[j].x;
                double new_y = (rand() / (double)RAND_MAX < CR) ? trial_y : population[j].y;

                // (e) 適應度評估
                double new_value = Evaluation(new_x, new_y);

                // (f) 選擇
                if (new_value < population[j].value) {
                    population[j].x = new_x;
                    population[j].y = new_y;
                    population[j].value = new_value;
                }

                // (g) 更新全域最佳
                if (population[j].value < global_best.value) {
                    global_best = population[j];
                }
            }
            cout<< "Iteration: " << nfes + 1 << ", Global Best Value: " << global_best.value << std::endl;
            fout << nfes << " " << global_best.value << std::endl;
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

// Ackley function 評分
double Alg::Evaluation(double x, double y)
{
    const double pi = 3.14159265358979323846;
    double term1 = -20.0 * exp(-0.2 * sqrt(0.5 * (x * x + y * y)));
    double term2 = -exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y)));
    return term1 + term2 + exp(1.0) + 20.0;
}

/*int Alg::Evaluation(const std::vector<int>& sol)
{
    return OneMaxProblem(sol, Bit);
}*/

void Alg::Reset()
{
    // 清除舊資料或狀態（目前為空實作）
}
