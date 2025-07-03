#include "Alg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

const int SEG_SIZE = 8; // 每段8位元
inline int get_dim(int Bit) { return (Bit + SEG_SIZE - 1) / SEG_SIZE; }

// 粒子結構（多維度）
struct Particle {
    std::vector<uint32_t> position;      // 多維座標，每維8bit
    std::vector<int> velocity;           // 多維速度（可用int/double）
    std::vector<uint32_t> best_position; // 個人最佳
    int best_value;                      // 個人最佳適應度
    int value;                           // 當前適應度

    Particle(int dim) {
        position.resize(dim);
        velocity.resize(dim);
        best_position.resize(dim);
        best_value = -std::numeric_limits<int>::infinity();
        value = -std::numeric_limits<int>::infinity();
    }
};

// 將多維 position 轉回 bit 向量
std::vector<int> position_to_bits(const std::vector<uint32_t>& pos, int Bit) {
    std::vector<int> bits(Bit);
    for (int d = 0; d < pos.size(); ++d) {
        for (int k = 0; k < SEG_SIZE && d * SEG_SIZE + k < Bit; ++k) {
            bits[d * SEG_SIZE + k] = (pos[d] >> k) & 1;
        }
    }
    return bits;
}

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate;

    cout << "bit: " << Bit << " run: " << Run << " iter: " << Iter << " rate: " << rate << endl;

    int dim = get_dim(Bit);
    uint32_t max_seg = 0xFF; // 8位元最大值
    if (Bit % SEG_SIZE != 0) {
        max_seg = (1U << (Bit % SEG_SIZE)) - 1;
    }

    for (int i = 0; i < Run; i++)
    {
        cout << "Run: " << i + 1 << endl;
        std::ofstream fout("result_PSO" + std::to_string(i + 1) + ".txt", std::ios::out | std::ios::trunc);

        Reset();

        // 1. 初始化粒子群
        int swarm_size = 3;
        std::vector<Particle> swarm;
        for (int j = 0; j < swarm_size; ++j) {
            Particle p(dim);
            for (int d = 0; d < dim; ++d) {
                if (d == dim - 1 && Bit % SEG_SIZE != 0)
                    p.position[d] = rand() % (max_seg + 1);
                else
                    p.position[d] = rand() % 256; // 產生8位元隨機數
                p.velocity[d] = 0;
            }
            p.best_position = p.position;
            std::vector<int> bits = position_to_bits(p.position, Bit);
            p.value = Evaluation(bits);
            p.best_value = p.value;
            swarm.push_back(p);
        }
        cout<<"dim: " << dim << " swarm_size: " << swarm_size << endl;
        // 2. 設定全域最佳
        std::vector<uint32_t> global_best_position = swarm[0].best_position;
        int global_best_value = swarm[0].best_value;
        for (const auto& p : swarm) {
            if (p.best_value > global_best_value) {
                global_best_value = p.best_value;
                global_best_position = p.best_position;
            }
        }
        cout << "Global Best Value: " << global_best_value << endl;

        double w = 0.9;
        double c1 = 2;
        double c2 = 2;

        // 3. 主迴圈
        for (int nfes = 0; nfes < Iter; nfes++)
        {
            cout << "algorithm running... " << nfes << endl;

            // (1) 更新速度與位置
            for (auto& p : swarm) {
                for (int d = 0; d < dim; ++d) {
                    p.velocity[d] = static_cast<int>(
                        w * p.velocity[d]
                        + c1  * (int(p.best_position[d]) - int(p.position[d]))
                        + c2  * (int(global_best_position[d]) - int(p.position[d]))
                    );
                    //cout<<"global_best_position: " << global_best_position[d] << " p.position: " << p.position[d] << " p.velocity: " << p.velocity[d] << endl;
                    // 更新位置
                    int new_pos = int(p.position[d]) + p.velocity[d];
                    // 邊界檢查
                    if (d == dim - 1 && Bit % SEG_SIZE != 0) {
                        if (new_pos < 0) new_pos = 0-new_pos;
                        if (new_pos > (int)max_seg) new_pos = max_seg;
                    } else {
                        if (new_pos < 0) new_pos = 0-new_pos; 
                        if (new_pos > (int)0xFF) new_pos = 0xFF; // 8位元最大值
                    }
                    //cout << "d: " << d << " old_pos: " << p.position[d] << " new_pos: " << new_pos << endl;
                    p.position[d] = static_cast<uint32_t>(new_pos);
                }
            }

            // (2) 計算適應度與更新個人最佳
            for (auto& p : swarm) {
                std::vector<int> bits = position_to_bits(p.position, Bit);
                p.value = Evaluation(bits);
                if (p.value > p.best_value) {
                    p.best_value = p.value;
                    p.best_position = p.position;
                }
            }
            cout << "NFES: " << nfes << " Best Value: " << global_best_value << endl;

            // (3) 更新全域最佳
            for (const auto& p : swarm) {
                if (p.best_value > global_best_value) {
                    global_best_value = p.best_value;
                    global_best_position = p.best_position;
                }
            }

            fout << nfes << " " << global_best_value << std::endl;
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
