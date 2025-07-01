#include "Alg.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <algorithm>

using namespace std;

// 個體結構
struct Individual {
    std::vector<int> genes;
    int fitness;

    Individual(int size) {
        genes.resize(size);
        for (int& g : genes) {
            g = rand() % 2;
        }
        fitness = 0;
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
        int current = 0;
        int value = 0;
        cout << "Run: " << i + 1 << endl;

        std::ofstream fout("result_GA" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 1. 初始化族群
        std::vector<Individual> population;
        for (int j = 0; j < 50; ++j) { // 可調整族群數量
            Individual ind(Bit);
            ind.fitness = Evaluation(ind.genes);
            population.push_back(ind);
        }

        // 2. 適應度評估（已於初始化中完成）

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // 3. 選擇（分為前半群與後半群，各自找出最高者）
            int half = population.size() / 2;

            auto best_front = max_element(population.begin(), population.begin() + half, [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness;
            });
            auto best_back = max_element(population.begin() + half, population.end(), [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness;
            });
            // best_front 與 best_back 就是兩群中各自的最佳個體

            // 4. 交配與突變：產生新個體
            Individual child(Bit);
            for (int b = 0; b < Bit; ++b) {
                double r = (double)rand() / RAND_MAX;
                if (r < rate) { // 交配率，來源於 best_front
                    child.genes[b] = best_front->genes[b];
                } else if (r < 2 * rate) { // 交配率，來源於 best_back
                    child.genes[b] = best_back->genes[b];
                } else { // 剩下的機率做突變
                    child.genes[b] = rand() % 2;
                }
            }
            // 計算新個體適應度
            child.fitness = Evaluation(child.genes);

            // 將 child 替換掉群體中最差的個體
            auto worst = min_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness;
            });
            *worst = child;

            // 7. 終止條件（待實作）

            // 這裡暫時只是印出目前族群中最好的個體
            auto best = max_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness;
            });
            current = best->fitness;
            fout << nfes << " " << current << std::endl;
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
