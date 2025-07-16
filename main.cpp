#include "Alg.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
int main(int argc, char *argv[])
{    
    srand(static_cast<unsigned>(time(0))); // 加這行
    cout<<"main"<<endl;
    //system("pause");
    // 檢查參數是否足夠
    if (argc < 5) {
        cout << "參數不足！請輸入：Bit Run Iter rate\n";
        system("pause");
        return 1;
    } 

    int Bit = atoi(argv[1]);     // 位元數
    int Run = atoi(argv[2]);     // 執行次數
    int Iter = atoi(argv[3]);    // 世代數
    double rate = atof(argv[4]); // 演算法參數
    int dim = atoi(argv[5]);     // 維度
    
    Alg algorithm;
    algorithm.RunALG(Bit, Run, Iter, rate , dim);
    
    system("pause");
}