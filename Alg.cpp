#include "Alg.h"
#include <fstream>
#include <chrono> // 新增

void Alg::RunALG(int _Bit, int _Run, int _Iter, double _Rate)
{
    Bit = _Bit;
    Run = _Run;
    Iter = _Iter;
    rate = _Rate;

    cout <<"bit:"<< Bit << "run:" << Run << "iter:" << Iter << "rate:" << rate<< endl;
    for (int i = 0; i < Run; i++)
    {   
        cout << "Run: " << i + 1 << endl;
        std::ofstream fout("result_Run" + std::to_string(i + 1) + ".txt");

        int current=0;
        Reset();
        Init();

        // 用 chrono 計時
        auto start = std::chrono::steady_clock::now();
        int value = 0;
        while (true)
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            if (elapsed >= 180) break; // 運算 10 秒後停止

            cout<<"algorithm running..."<<nfes<<endl;
            //fout<<"algorithm running..."<<nfes<<endl;

            // 隨機產生一個解
            std::vector<int> sol(Bit);
            for (int k = 0; k < Bit; ++k) {
                sol[k] = rand() % 2; // 0 或 1
            }
            
            Evaluation(sol, value); // 評估這個解
            if (value > current) {
                current = value; // 更新當前最佳解      
                cout << "Current Best Value: " << current << endl;
                fout << "Current Best Value: " << current <<" run:"<<nfes<< std::endl;
            }
        }
        fout.close();
    }
}

void Alg::Evaluation(vector<int> sol, int &value)
{
    value = OneMaxProblem(sol, Bit);
    nfes++;
    //cout<< "Evaluation: " << nfes << " Value: " << value << endl;
}

void Alg::Reset()
{
    nfes = 0;
}

void Alg::Init()
{
    /*Initialize*/
}
