#include "def.h"

using namespace def;

TDelta::TDelta(int8_t row/* = 0*/, int8_t col/* = 0*/)
    : deltaRow(row)
    , deltaCol(col)
{

}

bool TDelta::operator==(const TDelta& rhs) const
{
    return deltaRow == rhs.deltaRow && deltaCol == rhs.deltaCol;
}

TPos::TPos(int8_t row1/* = -1*/, int8_t col1/* = -1*/)
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
    if (player == EP_black)
        player = EP_red;
    else if (player == EP_red)
        player = EP_black;
    else
        player = EP_none;
}

// 对位玩家
EPlayer def::getOtherPlayer(EPlayer player)
{
    if (player == EP_black)
        return EP_red;
    else if (player == EP_red)
        return EP_black;
    else
        return EP_none;
}
