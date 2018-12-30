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

TPos::TPos(int8_t r, int8_t c)
    : row(r)
    , col(c)
{

}

TPos::TPos(const TPos& other)
{
    *this = other;
}

TPos& TPos::operator=(const TPos& rhs)
{
    row = rhs.row;
    col = rhs.col;
    return *this;
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
    return *this != INVALID_POS;
}

TDelta TPos::operator-(const TPos& rhs) const
{
    return {row - rhs.row, col - rhs.col};
}

TPos TPos::operator+(const TDelta& delta) const
{
    return {row + delta.deltaRow, col + delta.deltaCol};
}

TMove::TMove(TPos s, TPos d)
{
    src = s;
    dst = d;
}

TMove& TMove::operator=(const TMove& rhs)
{
    src = rhs.src;
    dst = rhs.dst;
    return *this;
}

TMove::TMove(const TMove& other)
{
    *this = other;
}

// 切换玩家
void def::switchPlayer(PLAYER_E& player)
{
    if (player == PLAYER_black)
        player = PLAYER_red;
    else if (player == PLAYER_red)
        player = PLAYER_black;
    else
        player = PLAYER_none;
}

// 对位玩家
PLAYER_E def::getEnemyPlayer(PLAYER_E player)
{
    if (player == PLAYER_black)
        return PLAYER_red;
    else if (player == PLAYER_red)
        return PLAYER_black;
    else
        return PLAYER_none;
}

PLAYER_E def::getOwner(ICON_E icon)
{
    return static_cast<PLAYER_E>(icon & def::PLAYER_MASK);
}

PIECE_E def::getPiece(ICON_E icon)
{
    return static_cast<PIECE_E>(icon & def::PIECE_MASK);
}
