#ifndef CO_H
#define CO_H

#include "def.h"

using def::TPos;

namespace co
{
    //二维数组尺寸
    const int g_row = 10;
    const int g_col = 9;
    //二维数组在一维数组内的偏移量
    const int g_leftEdge   = 3;
    const int g_topEdge    = 3;
    const int g_rightEdge  = 11;
    const int g_bottomEdge = 12;

    inline bool isValid2d(TPos pos)
    {
        return pos.x >= 0 && pos.x < g_row && pos.y >= 0 && pos.y < g_col;
    }

    inline bool isValid1d(int x)
    {
        int row = x / 16;
        int col = x % 16;
        return row >= g_topEdge && row <= g_bottomEdge &&
               col >= g_leftEdge && col <= g_rightEdge;
    }

    inline bool xy2x(TPos pos, int& x)
    {
        if (!isValid2d(pos))
            return false;

        x = 16 * (pos.x + g_topEdge) + pos.y + g_leftEdge;
        return true;
    }

    inline bool x2xy(int x, TPos& pos)
    {
        if (!isValid1d(x))
            return false;

        pos.x = x / 16 - g_topEdge;
        pos.y = x % 16 - g_leftEdge;
        return true;
    }
}

#endif // CO_H
