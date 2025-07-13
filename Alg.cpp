#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>
#include <ctime>

using namespace std;

// 隨機數生成器
static std::random_device rd;
static std::mt19937 gen((unsigned int)time(0)); // 用時間初始化
static std::uniform_real_distribution<> dis(-32.768, 32.768); // Ackley function 定義域
static std::normal_distribution<> perturb(0.0, 1.0); // 擾動分佈

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate;

    cout << "Bit: " << Bit << " Run: " << Run << " Iter: " << Iter << " Rate: " << rate << endl;

    // 群體大小
    const int population_size = 50;

    // 學生（解）結構
    struct Student {
        double x, y;
        double value;
        double prev_x, prev_y; // 前一迭代的位置
        double prev_value;     // 前一迭代的適應度
    };
    vector<Student> population(population_size);

    for (int i = 0; i < Run; i++)
    {
        cout << "Run: " << i + 1 << endl;
        std::ofstream fout("result_LearningGroup" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 初始化群體
        for (int j = 0; j < population_size; j++) {
            population[j].x = dis(gen);
            population[j].y = dis(gen);
            population[j].value = Evaluation(population[j].x, population[j].y);
            population[j].prev_x = population[j].x;
            population[j].prev_y = population[j].y;
            population[j].prev_value = population[j].value;
            if (population[j].value < global_best.value) {
                global_best = {population[j].x, population[j].y, population[j].value};
            }
        }

        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "Algorithm running... " << nfes << endl;

            // 計算每個學生的變化方向和幅度
            vector<double> weights(population_size);
            vector<double> delta_x(population_size), delta_y(population_size);
            vector<pair<double, int>> delta_f_abs(population_size); // 儲存 |Δf| 和索引
            double sum_weights = 0.0;
            double weighted_x = 0.0, weighted_y = 0.0;

            for (int j = 0; j < population_size; j++) {
                // 隨機擾動生成新解
                double perturb_scale = rate * (1.0 - nfes / (double)Iter); // 隨迭代縮小擾動
                population[j].x += perturb_scale * perturb(gen);
                population[j].y += perturb_scale * perturb(gen);
                // 限制定義域
                population[j].x = max(-32.768, min(32.768, population[j].x));
                population[j].y = max(-32.768, min(32.768, population[j].y));
                population[j].value = Evaluation(population[j].x, population[j].y);

                // 計算變化方向和幅度
                double delta_f = population[j].value - population[j].prev_value;
                delta_x[j] = population[j].x - population[j].prev_x;
                delta_y[j] = population[j].y - population[j].prev_y;
                delta_f_abs[j] = {abs(delta_f), j};
                weights[j] = (delta_f < 0) ? -rate * abs(delta_f) : rate * abs(delta_f);
            }

            // 僅考慮變化幅度前半的學生
            sort(delta_f_abs.begin(), delta_f_abs.end(), greater<pair<double, int>>());
            int half_size = population_size / 2;
            for (int j = 0; j < half_size; j++) {
                int idx = delta_f_abs[j].second;
                sum_weights += abs(weights[idx]);
                weighted_x += weights[idx] * delta_x[idx];
                weighted_y += weights[idx] * delta_y[idx];
            }

            // 推測最佳位置
            double best_x = sum_weights > 1e-16 ? weighted_x / sum_weights : weighted_x;
            double best_y = sum_weights > 1e-16 ? weighted_y / sum_weights : weighted_y;
            // 限制定義域
            best_x = max(-32.768, min(32.768, best_x));
            best_y = max(-32.768, min(32.768, best_y));
            cout << "Best Position: (" << best_x << ", " << best_y << ")" << endl;
            // 逐步靠近最佳位置
            double alpha = 0.8 - 0.78 * (nfes / (double)Iter); // 從 0.8 減到 0.02
            rate *=0.5; 
            cout<<"rate: "<<rate<<endl;
            vector<Student> new_population(population_size);
            for (int j = 0; j < population_size; j++) {
                // 線性插值：(1 - α) * current + α * best
                new_population[j].x = (1.0 - alpha) * population[j].x + alpha * best_x;
                new_population[j].y = (1.0 - alpha) * population[j].y + alpha * best_y;
                // 加入小擾動以保持多樣性
                double perturb_scale = rate * (1.0 - nfes / (double)Iter);
                //將rate呈現指數下降

                //perturb_scale *= exp(-0.1 * nfes / (double)Iter); // 指數衰減
                new_population[j].x += perturb_scale * perturb(gen) * 0.1; // 減小擾動幅度
                new_population[j].y += perturb_scale * perturb(gen) * 0.1;
                // 限制定義域
                new_population[j].x = max(-32.768, min(32.768, new_population[j].x));
                new_population[j].y = max(-32.768, min(32.768, new_population[j].y));
                new_population[j].value = Evaluation(new_population[j].x, new_population[j].y);
                new_population[j].prev_x = new_population[j].x;
                new_population[j].prev_y = new_population[j].y;
                new_population[j].prev_value = new_population[j].value;

                // 更新全局最佳解
                if (new_population[j].value < global_best.value) {
                    global_best = {new_population[j].x, new_population[j].y, new_population[j].value};
                }
            }

            // 更新群體
            population = new_population;

            cout << "Iteration: " << nfes + 1 << ", Global Best Value: " << global_best.value << endl;
            fout << nfes << " " << global_best.value << endl;
        }

        fout.close();
    }
}

std::vector<int> Alg::Init()
{
    global_best = {dis(gen), dis(gen), std::numeric_limits<double>::max()};
    global_best.value = Evaluation(global_best.x, global_best.y);
    return std::vector<int>();
}

double Alg::Evaluation(double x, double y)
{
    const double pi = 3.14159265358979323846;
    double term1 = -20.0 * exp(-0.2 * sqrt(0.5 * (x * x + y * y)));
    double term2 = -exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y)));
    return term1 + term2 + exp(1.0) + 20.0;
}

void Alg::Reset()
{
    global_best = {0.0, 0.0, std::numeric_limits<double>::max()};
}