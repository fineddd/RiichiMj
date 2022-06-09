#pragma once

namespace MahjongHelper
{
    /// <summary>
    /// 组（其中牌关系只有相同）
    /// </summary>
    class Group
    {
    public:
        /// <summary>
        /// 组内首张牌的序号
        /// </summary>
        const int Loc = 0;
        /// <summary>
        /// 组内牌数
        /// </summary>
        int Len = 1;
        /// <summary>
        /// 已确认（顺/刻）的牌数量
        /// </summary>
        int Confirmed = 0;
        Group(const int loc) : Loc{ loc } { }
    };
}