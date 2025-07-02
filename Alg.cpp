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
        for (int j = 0; j < 50; ++j) { // 建議增加族群數量以維持多樣性
            Individual ind(Bit);
            ind.fitness = Evaluation(ind.genes);
            population.push_back(ind);
        }

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // 1. 先依 fitness 由大到小排序
            std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
                return a.fitness > b.fitness;
            });

            // 2. 取前10%作為父母群
            int elite_count = std::max(1, (int)(population.size() * 0.1));
            std::vector<Individual> parents(population.begin(), population.begin() + elite_count);

            // 3. 產生新族群（每次隨機從父母群選兩個交配）
            std::vector<Individual> new_population;
            while (new_population.size() < population.size()) {
                // 隨機選兩個父母
                int idx1 = rand() % elite_count;
                int idx2 = rand() % elite_count;
                while (idx2 == idx1 && elite_count > 1) idx2 = rand() % elite_count;

                Individual parent1 = parents[idx1];
                Individual parent2 = parents[idx2];

                Individual child(Bit);
                for (int b = 0; b < Bit; ++b) {
                    double r = (double)rand() / RAND_MAX;
                    if (r < rate) {
                        child.genes[b] = parent1.genes[b];
                    } else if (r < 2 * rate) {
                        child.genes[b] = parent2.genes[b];
                    } else {
                        child.genes[b] = rand() % 2;
                    }
                }
                child.fitness = Evaluation(child.genes);
                new_population.push_back(child);
            }

            // 4. 用新族群取代舊族群
            population = new_population;

            // 5. 找出目前最佳個體並輸出
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
