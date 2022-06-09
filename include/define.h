#pragma once
#include <list>
#include <memory>
#include <vector>

using namespace std;

#include "Tile.h"
#include "Group.h"

namespace MahjongHelper
{
    using HandTileList = list<shared_ptr<Tile>>;
}
