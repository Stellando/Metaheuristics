#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

// 粒子結構
struct Particle {
    double x, y;           // 目前位置
    double vx, vy;         // 速度
    double best_x, best_y; // 個人最佳位置
    double best_value;     // 個人最佳適應度
    double value;          // 當前適應度

    Particle() {
        // Ackley 定義域 [-20, 20]
        x = ((double)rand() / RAND_MAX) * 40.0 - 20.0;
        y = ((double)rand() / RAND_MAX) * 40.0 - 20.0;
        vx = vy = 0.0;
        best_x = x;
        best_y = y;
        best_value = std::numeric_limits<double>::infinity();
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
        std::ofstream fout("result_PSO" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 1初始化粒子
        const int num_particles = 30; // 粒子數量
        //呼叫INIT()來初始化粒子
        std::vector<Particle> particles(num_particles);
        for (int j = 0; j < num_particles; ++j) {
            particles[j] = Particle();
        }
        // 2. 計算每個粒子的適應度
        for (auto& p : particles) {
            p.value = Evaluation(p.x, p.y);
            if (p.value < p.best_value) {
                p.best_value = p.value;
                p.best_x = p.x;
                p.best_y = p.y;
            }
            //cout << "Particle Position: (" << p.x << ", " << p.y << ") Value: " << p.value << endl;
        }
        // 計算全局最佳位置
        double global_best_x = particles[0].best_x; 
        double global_best_y = particles[0].best_y;
        double global_best_value = particles[0].best_value;
        for (const auto& p : particles) {
            if (p.best_value < global_best_value) {
                global_best_value = p.best_value;
                global_best_x = p.best_x;
                global_best_y = p.best_y;
            }
        }
        //cout << "Global Best Position: (" << global_best_x << ", " << global_best_y << ") with value: " << global_best_value << endl;
        
        // 3. 主迴圈
        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;
            // 更新每個粒子的速度和位置
            for (auto& p : particles) {
                // 更新速度
                p.vx = 0.5 * p.vx + rate * (p.best_x - p.x) + rate * (global_best_x - p.x);
                p.vy = 0.5 * p.vy + rate * (p.best_y - p.y) + rate * (global_best_y - p.y);
                // 限制速度範圍
                double vmax = 20.0; // 最大速度
                if (p.vx > vmax) p.vx = vmax;
                if (p.vx < -vmax) p.vx = -vmax;
                if (p.vy > vmax) p.vy = vmax;
                if (p.vy < -vmax) p.vy = -vmax;

                // 更新位置
                p.x += p.vx;
                p.y += p.vy;

                // 確保位置在定義域內
                if (p.x < -20.0) p.x = -20.0;
                if (p.x > 20.0) p.x = 20.0;
                if (p.y < -20.0) p.y = -20.0;
                if (p.y > 20.0) p.y = 20.0;

                // 計算新的適應度
                p.value = Evaluation(p.x, p.y);
                
                // 更新個人最佳位置
                if (p.value < p.best_value) {
                    p.best_value = p.value;
                    p.best_x = p.x;
                    p.best_y = p.y;
                }
            }
            // 更新全局最佳位置
            global_best_value = particles[0].best_value;
            global_best_x = particles[0].best_x;
            global_best_y = particles[0].best_y;
            for (const auto& p : particles) {
                if (p.best_value < global_best_value) {
                    global_best_value = p.best_value;
                    global_best_x = p.best_x;
                    global_best_y = p.best_y;
                }
            }
  
            cout<<"best"<<global_best_x<<" "<<global_best_y<<" "<<global_best_value<<std::endl;
            fout << nfes <<" "<<global_best_value<< std::endl;
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
