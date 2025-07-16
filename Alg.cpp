#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>

using namespace std;

// 隨機數生成器
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> dis(-32.768, 32.768); // Ackley function 標準定義域
static std::uniform_real_distribution<> prob(0.0, 1.0);

struct Individual {
    std::vector<double> pos; // 多維位置
    double value;            // 適應度（Ackley function 值）

    Individual(int dim) {
        pos.resize(dim);
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(-32.768, 32.768);
        for (int d = 0; d < dim; ++d) {
            pos[d] = dis(gen); // 隨機初始化 pos
        }
        value = std::numeric_limits<double>::infinity();
    }
};

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate, int dim)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate; // 目前未使用，可用於自適應參數
    dim = dim;
    int population_size = 10 * dim; // 動態設置族群大小，建議 10 * dim 或 20 * dim

    cout << "Bit: " << Bit << " Run: " << Run << " Iter: " << Iter << " Rate: " << rate << " Dim: " << dim << endl;

    for (int i = 0; i < Run; i++)
    {
        cout << "Run: " << i + 1 << endl;
        std::ofstream fout("result_DE" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 1. 初始化族群
        std::vector<Individual> population;
        population.reserve(population_size); // 預先分配空間以提高效率
        for (int j = 0; j < population_size; ++j) {
            Individual ind(dim); // 創建新個體，pos 向量在構造函數中隨機生成
            ind.value = Evaluation(ind.pos, dim); // 計算適應度值
            population.push_back(ind); // 將個體加入族群
        }

        // 2. 找出初始全域最佳
        Individual global_best = population[0];

        for (const auto& ind : population) {
            //cout << "Initial individual value: " << ind.value << std::endl;
            if (ind.value < global_best.value)
                global_best = ind;
        }

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "Algorithm running... " << nfes << endl;

            // DE 主循環
            for (int j = 0; j < population_size; ++j) {
                // (a) 隨機選三個不同的個體 r1, r2, r3，且都不等於 j
                int r1, r2, r3;
                do { r1 = rand() % population_size; } while (r1 == j);
                do { r2 = rand() % population_size; } while (r2 == j || r2 == r1);
                do { r3 = rand() % population_size; } while (r3 == j || r3 == r1 || r3 == r2);

                // (b) 差分變異
                std::vector<double> trial(dim);
                for (int d = 0; d < dim; ++d) {
                    trial[d] = population[r1].pos[d] + 0.8 * (population[r2].pos[d] - population[r3].pos[d]);
                    // 邊界檢查：若超出 [-32.768, 32.768]，重新隨機生成
                    if (trial[d] < -32.768 || trial[d] > 32.768) {
                        trial[d] = dis(gen);
                    }
                }

                // (c) 交叉
                std::vector<double> new_pos(dim);
                for (int d = 0; d < dim; ++d) {
                    if (prob(gen) < 0.9) { // CR = 0.9
                        new_pos[d] = trial[d];
                    } else {
                        new_pos[d] = population[j].pos[d];
                    }
                }

                // (d) 適應度評估
                double new_value = Evaluation(new_pos, dim);
                // (e) 選擇
                if (new_value < population[j].value) {
                    population[j].pos = new_pos;
                    population[j].value = new_value;
                    //cout << "Updated individual " << j << " with new value: " << new_value << std::endl;
                }
                
                // (f) 更新全域最佳
                if (population[j].value < global_best.value) {
                    global_best = population[j];
                }
            }
            cout << "Iteration: " << nfes + 1 << ", Global Best Value: " << global_best.value << std::endl;
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

// 多維 Ackley function 評分
double Alg::Evaluation(const std::vector<double>& vec, int dim)
{
    if (vec.size() != static_cast<size_t>(dim)) {
        cerr << "Error: Vector size (" << vec.size() << ") does not match dimension (" << dim << ")" << endl;
        return std::numeric_limits<double>::max();
    }
    const double pi = 3.14159265358979323846;
    double sum_sq = 0.0;
    double sum_cos = 0.0;
    for (int i = 0; i < dim; ++i) {
        sum_sq += vec[i] * vec[i];
        sum_cos += cos(2 * pi * vec[i]);
    }
    double term1 = -20.0 * exp(-0.2 * sqrt(sum_sq / dim));
    double term2 = -exp(sum_cos / dim);
    return term1 + term2 + exp(1.0) + 20.0;
}

void Alg::Reset()
{
    // 清除舊資料或狀態（目前為空實作）
}