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

//        TPos(int row1, int col1)
//            : row(row1)
//            , col(col1)
//        {}

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

        TPos& operator+(const TDelta& delta)
        {
            row += delta.deltaRow;
            col += delta.deltaCol;
            return *this;
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

    const TPos g_nullPos = {-1, -1};

    //掩码
    const byte g_scopeMask = 0x18;
    const byte g_pieceMask = 0x7;
    
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
