#pragma once
#include <string>

namespace MahjongHelper
{
    class Tile
    {
    public:
        /// <summary>
        /// 存放牌信息
        /// </summary>
        const int Val;
        Tile(const int val) : Val{ val } { }

        /// <summary>
        /// 获取
        /// </summary>
        std::string GetTileName()
        {
            switch (Val)
            {
            case 0:
                return "1M";
            case 1:
                return "2M";
            case 2:
                return "3M";
            case 3:
                return "4M";
            case 4:
                return "5M";
            case 5:
                return "6M";
            case 6:
                return "7M";
            case 7:
                return "8M";
            case 8:
                return "9M";

            case 16:
                return "1P";
            case 17:
                return "2P";
            case 18:
                return "3P";
            case 19:
                return "4P";
            case 20:
                return "5P";
            case 21:
                return "6P";
            case 22:
                return "7P";
            case 23:
                return "8P";
            case 24:
                return "9P";

            case 32:
                return "1S";
            case 33:
                return "2S";
            case 34:
                return "3S";
            case 35:
                return "4S";
            case 36:
                return "5S";
            case 37:
                return "6S";
            case 38:
                return "7S";
            case 39:
                return "8S";
            case 40:
                return "9S";

            case 48:
                return "东";
            case 56:
                return "南";
            case 64:
                return "西";
            case 72:
                return "北";
            case 80:
                return "白";
            case 88:
                return "发";
            case 96:
                return "中";
            default:
                return "!";
            }
        }
        /*std::string GetTileName()
        {
            switch (Val)
            {
            case 0:
                return "🀇";
            case 1:
                return "🀈";
            case 2:
                return "🀉";
            case 3:
                return "🀊";
            case 4:
                return "🀋";
            case 5:
                return "🀌";
            case 6:
                return "🀍";
            case 7:
                return "🀎";
            case 8:
                return "🀏";

            case 16:
                return "🀙";
            case 17:
                return "🀚";
            case 18:
                return "🀛";
            case 19:
                return "🀜";
            case 20:
                return "🀝";
            case 21:
                return "🀞";
            case 22:
                return "🀟";
            case 23:
                return "🀠";
            case 24:
                return "🀡";

            case 32:
                return "🀐";
            case 33:
                return "🀑";
            case 34:
                return "🀒";
            case 35:
                return "🀓";
            case 36:
                return "🀔";
            case 37:
                return "🀕";
            case 38:
                return "🀖";
            case 39:
                return "🀗";
            case 40:
                return "🀘";

            case 48:
                return "🀀";
            case 56:
                return "🀁";
            case 64:
                return "🀂";
            case 72:
                return "🀃";
            case 80:
                return "🀆";
            case 88:
                return "🀅";
            case 96:
                return "🀄";
            default:
                return "!";
            }
        }*/
    };
}