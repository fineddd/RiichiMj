// MahjongHelper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>

#include "define.h"

#include "Block.h"

#include "Opponent.h"
#include "OpponentHelper.h"

#include<time.h>

int main()
{
    SetConsoleOutputCP(65001);
    vector<int> handTile = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6 };

    MahjongHelper::Opponent ins;
    for (auto& tileNum : handTile)
    {
        ins.Hands.emplace_back(make_shared<MahjongHelper::Tile>(tileNum));
    }
    cout << ins.GetHands() << endl;
    cout << "" << endl;

    int n = 1 * 10000;
    int begin = time(NULL);
    std::cout << "TEST begin" << std::endl;

    for (int j = 0; j < n; ++j)
    {
        ins.GetReadyHands();
    }
    cout << ins.GetReadyHands() << endl;

    std::cout << "耗时:" << time(NULL) - begin << "秒" << std::endl;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
