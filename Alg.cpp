#include "Alg.h"
#include <fstream>

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

        //世代數(iteration)跟計算次數(evaluation)二擇一使用
        //跟其他演算法比較通常都是使用evaluation，故建議使用evaluation
        for (int j = 0; j < Iter; j++) /*Iteration (世代數) */
        {
            /*
            演算法流程
            */
        }
        int value = 0;
        while (nfes < Iter) /*Evaluation (計算次數)*/
        {
            cout<<"algorithm running..."<<nfes<<endl;
            // 隨機產生一個解
            std::vector<int> sol(Bit);
            for (int k = 0; k < Bit; ++k) {
                sol[k] = rand() % 2; // 0 或 1
            }
            
            Evaluation(sol, value); // 評估這個解
            if (value > current) {
                current = value; // 更新當前最佳解      
                cout << "Current Best Value: " << current << endl;
                fout << "Current Best Value: " << current << std::endl;

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
