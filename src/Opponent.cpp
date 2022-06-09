

#include "Opponent.h"
#include "OpponentHelper.h"
#include "Block.h"
#include <stdexcept>
#include <tuple>

namespace MahjongHelper
{
    std::string Opponent::GetHands()
    {
        std::string temp;
        for (auto& tile : Hands)
            temp += tile->GetTileName();
        return temp;
    }
    std::string Opponent::GetReadyHands()
    {
        Melds = 4 - (Hands.size() - 1) / 3;
        auto readyHands = ReadyHandJudge();
        if (readyHands.size() == 0)
            return "noting";
        string temp = "";
        for (auto& tile : readyHands)
            temp += tile.GetTileName();
        return temp;
    }

    /// <summary>
    /// 听牌判断（在摸牌前判断）
    /// </summary>
    /// <returns>听的牌</returns>
    inline list<Tile> Opponent::ReadyHandJudge()
    {
        list<Tile> readyHands;

        //如果没有副露（特殊牌型判断）---//
        if (Melds == 0)
        {
            auto readyHandsList = ThirteenOrphansJudge();
            if (readyHandsList.size() != 0)
                return readyHandsList;
        }

        auto errBlocks = GetBlocks();

        //不完整型块数---//
        switch (errBlocks.size())
        {
        //有一块不完整型（一块雀面不完整型（3n+1））---//
        //两杯口会在这里出现---//
        case 1:
            {
                //将此不完整型遍历---//
                readyHands.splice(readyHands.end(), errBlocks[0].Traversal(Hands, true));
                break;
            }
        //有两块不完整型（一块面子不完整型（3n+2），一块雀头完整型（3n+2））---//
        case 2:
            {
                if (errBlocks[1].IgnoreEyesJudge(Hands))
                    readyHands.splice(readyHands.end(), errBlocks[0].Traversal(Hands, false));
                if (errBlocks[0].IgnoreEyesJudge(Hands))
                    readyHands.splice(readyHands.end(), errBlocks[1].Traversal(Hands, false));
                break;
            }
        //有三块不完整型（两块半不完整型（3n+1），一块雀头完整型（3n+2））---//
        case 3:
            {
                //如果3n+2的不完整型夹在中间或不是雀头完整型，则无---//
                auto eyesIndex = 0;
                for (int i = 0; i < errBlocks.size(); ++i)
                    if (errBlocks[i].Integrity == Block::EIntegrityType::Type2)
                        eyesIndex = i;
                if (eyesIndex == 1 || !errBlocks[eyesIndex].IgnoreEyesJudge(Hands))
                    break;

                auto joinResult = JointBlocks(errBlocks[2 - eyesIndex], errBlocks[3 - eyesIndex]);
                if (get<0>(joinResult) == false)
                    break;

                //如果该牌组完整，则记听一面---//
                if (get<2>(joinResult).IntegrityJudge(get<1>(joinResult), -1))
                    readyHands.emplace_back(get<3>(joinResult));
                break;
            }
        //有两块不完整型（一块雀半完整型（3n），一块半不完整型（3n+1））---//
        case 4:
            {
                auto joint = errBlocks[0].FirstLoc < errBlocks[1].FirstLoc ?
                             JointBlocks(errBlocks[0], errBlocks[1]) :
                             JointBlocks(errBlocks[1], errBlocks[0]);
                if (get<0>(joint) == false)
                    break;
                //如果该牌组是雀头完整型，则记听一面---//
                if (get<2>(joint).IgnoreEyesJudge(get<1>(joint)))
                    readyHands.emplace_back(get<3>(joint));

                break;
            }
        }
        return readyHands;
    }
    /// <summary>
    /// 国士牌型判断
    /// </summary>
    /// <returns>听牌</returns>
    inline list<Tile>Opponent::ThirteenOrphansJudge()
    {
        list<Tile> tempReturn;
        //是否缺了某张幺九牌（0或1）---//
        auto shortage = false;
        //是否多了某张幺九牌（0或1）---//
        auto redundancy = false;
        auto shortTile = 0; //缺的幺九牌---//
        //判断十三张幺九牌的拥有情况---//
        int i = 0;
        for (auto& tile : Hands)
        {
            const auto temp = (shortage ? 1 : 0) - (redundancy ? 1 : 0);
            //如果和上张映射幺九牌一样---//
            if (tile->Val == (i + temp - 1) / 8)
            {
                //如果之前已经有一个多的牌---//
                if (redundancy)
                    return tempReturn;
                redundancy = true; //记录有多牌---//
            } //如果和下张映射幺九牌一样---//
            else if (tile->Val == (i + temp + 1) / 8)
            {
                //如果之前已经有一个缺牌则不是国士，否则记录缺牌---//
                if (shortage)
                    return tempReturn;
                shortage = true;
                shortTile = i / 8;
            } //有不是幺九牌即不符合国士---//
            else if (tile->Val != (i + temp) / 8)
                return tempReturn;

            ++i;
        }
        //若有多张，记听一面或记听一面（红中）（因为红中在最后不会被redundancy记录）---//
        if (redundancy)
            tempReturn.emplace_back(Tile(shortage ? shortTile : 96));
        //若不缺张则记听十三面---//
        else for (auto i = 0; i < 13; ++i)
                tempReturn.emplace_back(Tile(i / 8));
        return tempReturn;
    }

    /// <summary>
    /// 获取分块
    /// </summary>
    /// <returns>不完整的块数（最多3个）</returns>
    inline vector<Block> Opponent::GetBlocks()
    {
        vector<Block> errBlocks;
        vector<Block> blocks;
        blocks.emplace_back(Block(0));

        for (auto i = 0; i < Hands.size() - 1; ++i)
            //当关系不是相同或连续---//
            if (GetRelation(Hands, i) > 1)
            {
                //记录上一块的长度---//
                blocks.rbegin()->Len = i - blocks.rbegin()->FirstLoc + 1;
                //筛选完整型Lv->1---//
                switch (blocks.rbegin()->Len % 3)
                {
                case 0:
                    blocks.rbegin()->Integrity = Block::EIntegrityType::Type0;
                    break;
                case 1:
                    blocks.rbegin()->Integrity = Block::EIntegrityType::Type1;
                    break;
                case 2:
                    blocks.rbegin()->Integrity = Block::EIntegrityType::Type2;
                    break;
                default:
                    throw logic_error("unknown block type ");
                }
                //如果类型是不完整则记录---//
                if (blocks.rbegin()->Integrity != Block::EIntegrityType::Type0)
                    errBlocks.emplace_back(blocks.back());
                //若块序号达到(6 - 副露数)或有4个不完整型则无听---//
                if (blocks.size() + Melds == 6 || errBlocks.size() == 4)
                    return vector<Block>();
                //下一块，括号里是块内首张牌的序号---//
                blocks.emplace_back(Block(i + 1));
            }
        //最后一块的记录无法写进循环---//
        {
            blocks.rbegin()->Len = Hands.size() - blocks.rbegin()->FirstLoc;
            switch (blocks.rbegin()->Len % 3)
            {
            case 0:
                blocks.rbegin()->Integrity = Block::EIntegrityType::Type0;
                break;
            case 1:
                blocks.rbegin()->Integrity = Block::EIntegrityType::Type1;
                break;
            case 2:
                blocks.rbegin()->Integrity = Block::EIntegrityType::Type2;
                break;
            default:
                throw logic_error("unknown block type ");
            }
            if (blocks.back().Integrity != Block::EIntegrityType::Type0)
                errBlocks.emplace_back(blocks.back());
            if (errBlocks.size() == 4)
                return vector<Block>();
        }
        //通过完整型Lv.1的块，筛选完整型Lv.2发现有一块不完整，则为不完整型加半不完整型，多于一块则无听---//
        for (auto& block : blocks)
            if (block.Integrity == Block::EIntegrityType::Type0 && !block.IntegrityJudge(Hands, -1))
            {
                if (errBlocks.size() != 4)
                {
                    block.Integrity = Block::EIntegrityType::TypeEx;
                    errBlocks.emplace_back(block);
                    //特殊标记---//
                    errBlocks.emplace_back(Block(0));
                    errBlocks.emplace_back(Block(0));
                    errBlocks.emplace_back(Block(0));
                }
                else return vector<Block>();
            }
        return errBlocks;
    }

    /// <summary>
    /// 连接两块
    /// </summary>
    /// <param name="frontBlock">前块</param>
    /// <param name="followBlock">后块</param>
    /// <returns>连接后的牌、连接后的块、用来连接的牌</returns>
    inline tuple<bool, list<shared_ptr<Tile>>, Block, Tile> Opponent::JointBlocks(Block frontBlock, Block followBlock)
    {
        //临时记录中间隔的牌（可能是铳牌）---//
        auto itFrontBlockTile = Hands.begin();
        advance(itFrontBlockTile, frontBlock.LastLoc());
        if (itFrontBlockTile == Hands.end())
        {
            return { false, list<shared_ptr<Tile>>(), Block(0), Tile(0) };
        }
        shared_ptr<Tile > tempReadyHands = make_shared<Tile>((*itFrontBlockTile)->Val + 1);
        //如果原来这两张牌中间不是隔一张，则无听---//
        if (GetRelation(Hands, frontBlock.LastLoc()) != 2)
            return { false, list<shared_ptr<Tile>>(), Block(0), Tile(0) };
        //临时用来判断的牌组---//
        list<shared_ptr<Tile>> jointedHands;
        //这两块不完整型总张数---//
        Block jointedBlock(0);
        jointedBlock.Len = frontBlock.Len + 1 + followBlock.Len;
        //复制该不完整型所有牌---//
        auto itBlockBegin = Hands.begin();
        advance(itBlockBegin, frontBlock.FirstLoc);
        auto jointedEnd = itBlockBegin;
        advance(jointedEnd, jointedBlock.Len - 1);

        jointedHands.insert(jointedHands.end(), itBlockBegin, jointedEnd);
        //插入一张中间隔的牌---//
        auto itTemp = jointedHands.begin();
        advance(itTemp, frontBlock.Len);
        jointedHands.insert(itTemp, tempReadyHands);
        return { true, jointedHands, jointedBlock, *tempReadyHands };
    }
}