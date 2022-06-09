#pragma once
#include "Block.h"
#include <iostream>
#include "OpponentHelper.h"

namespace MahjongHelper
{

    int Block::LastLoc()
    {
        return  FirstLoc + Len - 1;
    }

    vector<Group> Block::GetGroups(HandTileList& hands)
    {
        vector<Group> groups;
        groups.emplace_back(Group(FirstLoc));
        //判断块内每个关系---//
        for (auto i = FirstLoc; i < FirstLoc + Len - 1; ++i)
        {
            //当关系是连续，则记录多一个组---//
            if (GetRelation(hands, i) == 1)
            {
                groups.rbegin()->Len = i + 1 - groups.rbegin()->Loc;
                groups.emplace_back(Group(i + 1));
            }
        }
        groups.rbegin()->Len = FirstLoc + Len - groups.rbegin()->Loc;
        return groups;
    }

    bool Block::IntegrityJudge(HandTileList& hands, int eyesLoc)
    {
        auto groups = GetGroups(hands);
        //在此时没用，但在和牌算符时会用到---
        vector<ETileType> blockTiles(Len, ETileType::Sequence);
        //若有雀头，则将雀头认为是刻 ---
        if (eyesLoc != -1)
        {
            auto& eyesGroup = groups[eyesLoc];
            eyesGroup.Confirmed += 2;

            blockTiles[eyesGroup.Loc - FirstLoc] = ETileType::Triplet;
            blockTiles[eyesGroup.Loc - FirstLoc + 1] = ETileType::Triplet;
        }
        //每次循环记录一个组---
        for (auto i = 0; i < groups.size(); ++i)
        {
            //该组牌数---//
            switch (groups[i].Len - groups[i].Confirmed)
            {
            //刚好全部确定---//
            case 0:
                continue;
            //都是顺，确定后面2组分别有1张是顺//
            case 1:
                if (groups.size() > i + 2)
                {
                    ++groups[i + 1].Confirmed;
                    ++groups[i + 2].Confirmed;
                    continue;
                }
                break;
            //都是顺，确定后面2组分别有2张是顺//
            case 2:
                if (groups.size() > i + 2)
                {
                    ++groups[i + 1].Confirmed;
                    ++groups[i + 1].Confirmed;
                    ++groups[i + 2].Confirmed;
                    ++groups[i + 2].Confirmed;
                    continue;
                }
                break;
            //---3刻1顺，确定后面2组分别有1张是顺---//
            case 4:
                if (groups.size() > i + 2)
                {
                    ++groups[i + 1].Confirmed;
                    ++groups[i + 2].Confirmed;
                    blockTiles[groups[i].Loc - FirstLoc] = ETileType::Triplet;
                    blockTiles[groups[i].Loc - FirstLoc + 1] = ETileType::Triplet;
                    blockTiles[groups[i].Loc - FirstLoc + 2] = ETileType::Triplet;
                    continue;
                }
                break;
            //---3张是刻---//
            case 3:
                blockTiles[groups[i].Loc - FirstLoc] = ETileType::Triplet;
                blockTiles[groups[i].Loc - FirstLoc + 1] = ETileType::Triplet;
                blockTiles[groups[i].Loc - FirstLoc + 2] = ETileType::Triplet;
                continue;
            //---可能是负数---//
            default:
                break;
            }
            Integrity = eyesLoc == - 1 ? EIntegrityType::TypeEx : EIntegrityType::Type2;
            return false;
        }
        return true;
    }

    bool Block::IgnoreEyesJudge(HandTileList& hands)
    {
        for (int i = FirstLoc, tempGroupNum = 0; i < FirstLoc + Len - 1; ++i)
        {
            //当关系是连续，则组数加一---//
            if (GetRelation(hands, i) == 1)
                ++tempGroupNum;
            //当关系是相同，若是雀头完整型，则听牌---//
            else if (IntegrityJudge(hands, tempGroupNum))
                return true;
        }
        return false;
    }

    list<Tile> Block::Traversal(HandTileList& hands, const bool mode)
    {
        list<Tile> tempReturn;
        //可能的首张牌---//
        auto itFirstLoc = hands.begin();
        advance(itFirstLoc, FirstLoc);
        auto itLastLoc = itFirstLoc;
        advance(itLastLoc, Len - 1);
        if (itLastLoc == hands.end()
                || itFirstLoc == hands.end())
        {
            return tempReturn;
        }
        auto first = (*itFirstLoc)->Val - 1;
        //如果首张是一万、筒、索或字牌，则first没有前一张，加回hands[loc]---//
        if (((*itFirstLoc)->Val & 15) == 0 || (*itFirstLoc)->Val / 8 > 5)
            ++first;
        //可能的末张牌---//
        auto last = (*itLastLoc)->Val + 1;
        //如果末张是九万、筒、索或字牌，则得last没有后一张，减回hands[loc]---//
        if (((*itLastLoc)->Val & 15) == 8 || (*itLastLoc)->Val / 8 > 5)
            --last;

        Block tempBlock(0);

        tempBlock.Len = Len + 1;
        auto tempTile = first;
        ++itLastLoc;
        //每张牌都插入尝试一次（遍历）---//
        for (auto i = 0; i < last - first + 1; ++i, ++tempTile)
        {
            //重新复制所有牌---//
            HandTileList tempHands;
            tempHands.insert(tempHands.end(), itFirstLoc, itLastLoc);
            //插入尝试的牌---//
            TileIn(tempHands, make_shared<Tile>(tempTile));
            //雀面不完整型且遍历、去对后完整，则听牌---//
            //面子不完整型且遍历后完整，则听牌---//
            if (mode && tempBlock.IgnoreEyesJudge(tempHands) ||
                    !mode && tempBlock.IntegrityJudge(tempHands, -1))
                tempReturn.emplace_back(Tile(tempTile));
        }
        return tempReturn;
    }
}
