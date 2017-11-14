#include "def.h"

using namespace def;

TDelta::TDelta(int row/* = 0*/, int col/* = 0*/)
    : deltaRow(row)
    , deltaCol(col)
{

}

bool TDelta::operator==(const TDelta& rhs) const
{
    return deltaRow == rhs.deltaRow && deltaCol == rhs.deltaCol;
}

TPos::TPos(int row1/* = -1*/, int col1/* = -1*/)
    : row(row1)
    , col(col1)
{

}

bool TPos::operator==(const TPos& rhs) const
{
    return row == rhs.row && col == rhs.col;
}

bool TPos::operator!=(const TPos& rhs) const
{
    return row != rhs.row || col != rhs.col;
}

bool TPos::operator()() const
{
    return *this != g_nullPos;
}

TDelta TPos::operator-(const TPos& rhs) const
{
    return {row - rhs.row, col - rhs.col};
}

TPos TPos::operator+(const TDelta& delta) const
{
    return {row + delta.deltaRow, col + delta.deltaCol};
}

// 切换玩家
void def::switchPlayer(EPlayer& player)
{
    if (player == EP_up)
        player = EP_down;
    else if (player == EP_down)
        player = EP_up;
    else
        player = EP_none;
}

// 对位玩家
EPlayer def::getOtherPlayer(EPlayer player)
{
    if (player == EP_up)
        return EP_down;
    else if (player == EP_down)
        return EP_up;
    else
        return EP_none;
}
