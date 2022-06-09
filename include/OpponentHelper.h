#pragma once
#include "define.h"
#include "Opponent.h"

#include <limits>

namespace MahjongHelper
{
    /// <summary>
    /// 两张手牌间关系
    /// </summary>
    /// <param name="hands">手牌</param>
    /// <param name="num">前张牌序号</param>
    static int GetRelation(HandTileList& hands, int num)
    {
        if (num + 1 >= hands.size())
        {
            return INT_MAX;
        }
        auto it = hands.begin();
        advance(it, num + 1);
        //if (it == hands.end())
        //{
        //    return INT_MAX;
        //}

        return (*it)->Val - (*--it)->Val;
    }

    /// <summary>
    /// 摸牌
    /// </summary>
    /// <param name="hands">手牌</param>
    /// <param name="tile">进张</param>
    /// <returns>插入牌的位置</returns>
    static int TileIn(HandTileList& hands, shared_ptr<Tile> tile)
    {
        auto ru = 0;
        if (hands.back()->Val < tile->Val)
        {
            hands.emplace_back(tile);
            return hands.size() - 1;
        }
        //找到进张插入的位置
        for (auto it = hands.begin(); it != hands.end(); ++it)
        {
            if (tile->Val <= (*it)->Val)
            {
                hands.insert(it, tile);
                break;
            }
            ++ru;
        }
        return ru;
    }
}