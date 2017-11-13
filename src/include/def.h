#ifndef DEF_H
#define DEF_H

#include <functional>

namespace def
{
    typedef unsigned char byte;
    typedef unsigned int uint;

    struct TDelta
    {
        int deltaRow;
        int deltaCol;

        TDelta(int row, int col)
            : deltaRow(row)
            , deltaCol(col)
        {}

        bool operator==(const TDelta& rhs) const
        {
            return deltaRow == rhs.deltaRow && deltaCol == rhs.deltaCol;
        }
    };

    struct TPos
    {
        int row;
        int col;

        TPos()
            : row(-1)
            , col(-1)
        {

        }

        TPos(int row1, int col1)
            : row(row1)
            , col(col1)
        {}

        bool operator==(const TPos& rhs) const
        {
            return row == rhs.row && col == rhs.col;
        }

        bool operator!=(const TPos& rhs) const
        {
            return row != rhs.row || col != rhs.col;
        }

        TDelta operator-(const TPos& rhs) const
        {
            return {row - rhs.row, col - rhs.col};
        }

        TPos operator+(const TDelta& delta)
        {
            return {row + delta.deltaRow, col + delta.deltaCol};
        }

        bool operator()()
        {
            return row != -1 && col != -1;// g_nullPos
        }
    };

    struct TClientCo
    {
        int x;
        int y;
    };

    struct TSize
    {
        uint width;
        uint height;
    };

    enum EPlayer
    {
        EP_up,
        EP_down,
        EP_none,
    };

    // 切换玩家
    inline void switchPlayer(EPlayer& player)
    {
        if (player == EP_up)
            player = EP_down;
        else if (player == EP_down)
            player = EP_up;
        else
            player = EP_none;
    }

    inline EPlayer getOtherPlayer(EPlayer player)
    {
        if (player == EP_up)
            return EP_down;
        else if (player == EP_down)
            return EP_up;
        else
            return EP_none;
    }

    enum EPiece
    {
        EP_empty = 0,

        EP_redKing = 9,
        EP_redAdvisor,
        EP_redBishop,
        EP_redKnight,
        EP_redRook,
        EP_redCannon,
        EP_redPawn = 15,

        EP_blackKing = 17,
        EP_blackAdvisor,
        EP_blackBishop,
        EP_blackKnight,
        EP_blackRook,
        EP_blackCannon,
        EP_blackPawn = 23,

        EP_select = 32
    };

    const TPos g_nullPos = {-1, -1};

    //掩码
    const byte g_scopeMask = 0x18;
    const byte g_pieceMask = 0x7;

    // 颜色标识
    const byte g_redFlag   = 0x8;// 默认下面为红色
    const byte g_blackFlag = 0x10;// 默认上面为黑色
    
    //七种棋子用低三位表示
    const byte g_empty   = 0x0;
    const byte g_king    = 0x1;
    const byte g_advisor = 0x2;
    const byte g_bishop  = 0x3;
    const byte g_knight  = 0x4;
    const byte g_rook    = 0x5;
    const byte g_cannon  = 0x6;
    const byte g_pawn    = 0x7;
}

namespace std
{
    // TPos散列函数
    template <>
    struct hash<def::TPos>
    {
        size_t operator()(const def::TPos& k) const
        {
            return ((hash<int>()(k.row) ^ (hash<int>()(k.col) << 1)) >> 1);
        }
    };

    // TDelta散列函数
    template <>
    struct hash<def::TDelta>
    {
        size_t operator()(const def::TDelta& k) const
        {
            return ((hash<int>()(k.deltaRow) ^ (hash<int>()(k.deltaCol) << 1)) >> 1);
        }
    };
}

#endif // DEF_H
