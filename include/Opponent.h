#pragma once
#include "define.h"
#include <list>
#include <string>
#include <memory>

#include <tuple>

namespace MahjongHelper
{

    class Block;
    class Opponent
    {
    public:
        /// <summary>
        /// 手牌区
        /// </summary>
        HandTileList Hands;

        /// <summary>
        /// 副露数
        /// </summary>
        int Melds = 0;

        std::string GetHands();
        std::string GetReadyHands();

        list<Tile> ReadyHandJudge();
        tuple<bool, list<shared_ptr<Tile>>, Block, Tile> JointBlocks(Block frontBlock, Block followBlock);
        list<Tile> ThirteenOrphansJudge();
        vector<Block> GetBlocks();
    };
}
