#pragma once
#include "define.h"

namespace MahjongHelper
{
    /// <summary>
    /// 块（其中牌关系只有相同或连续）---//
    /// </summary>
    class Block
    {
    public:
        /// <summary>
        /// 类型（真（3n）为完整型（由整数个面子组成），假为不完整型（含有雀头、不完整的面子））---//
        /// </summary>
        enum class EIntegrityType
        {
            /// <summary>
            /// 完整型（3n）
            /// </summary>
            Type0,
            /// <summary>
            /// 雀面不完整型或半不完整型（3n+1）
            /// </summary>
            Type1,
            /// <summary>
            /// 雀头不完整型或面子不完整型（3n+2）
            /// </summary>
            Type2,
            /// <summary>
            /// 雀半不完整型（3n）
            /// </summary>
            TypeEx
        } Integrity = EIntegrityType::Type0;

        /// <summary>
        /// 块内牌数（至少一张）---//
        /// </summary>
        int Len = 1;

        /// <summary>
        /// 块内首张牌的序号---//
        /// </summary>
        int const FirstLoc;

        int LastLoc();

        /// <summary>
        /// 类型（真（3n）为完整型（由整数个面子组成），假为不完整型（含有雀头、不完整的面子））---//
        /// </summary>
        bool Type = true;

        Block(const int loc) : FirstLoc{ loc } {  }

        vector<Group> GetGroups(HandTileList& hands);

        /// <summary>
        /// 每张牌的类型（顺或刻（雀头属于刻））---//
        /// </summary>
        enum class ETileType { Sequence, Triplet };
        /// <summary>
        /// 筛选完整型Lv.2
        /// </summary>
        /// <param name="hands">判断的牌组</param>
        /// <param name="eyesLoc">雀头的序号（-1为没有雀头）</param>
        bool IntegrityJudge(HandTileList& hands, int eyesLoc);

        /// <summary>
        /// 去对后完整（雀头完整型）
        /// </summary>
        /// <param name="hands">判断的牌组</param>
        /// <returns>是否完整</returns>
        bool IgnoreEyesJudge(HandTileList& hands);
        /// <summary>
        /// 遍历
        /// </summary>
        /// <param name="hands">判断的牌组</param>
        /// <param name="mode">是否要去对（真为雀面不完整型，假为面子不完整型）</param>
        /// <returns>听的牌，可能本来它就不为空，不过在这里不影响（将来算符时可能改动）</returns>
        list<Tile> Traversal(HandTileList& hands, const bool mode);
    };
}
