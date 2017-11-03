#ifndef CO_H
#define CO_H

#include "def.h"

using def::TPos;

namespace co
{
    //二维数组尺寸
    const int row_ = 10;
    const int col_ = 9;
    //二维数组在一维数组内的偏移量
    const int leftEdge_   = 3;
    const int topEdge_    = 3;
    const int rightEdge_  = 11;
    const int bottomEdge_ = 12;

    inline bool isValid2d(TPos pos)
    {
        return pos.x >= 0 && pos.x < row_ && pos.y >= 0 && pos.y < col_;
    }

    inline bool isValid1d(int x)
    {
        int row = x / 16;
        int col = x % 16;
        return row >= topEdge_ && row <= bottomEdge_ &&
               col >= leftEdge_ && col <= rightEdge_;
    }

    inline bool xy2x(TPos pos, int& x)
    {
        if (!isValid2d(pos))
            return false;

        x = 16 * (pos.x + topEdge_) + pos.y + leftEdge_;
        return true;
    }

    inline bool x2xy(int x, TPos& pos)
    {
        if (!isValid1d(x))
            return false;

        pos.x = x / 16 - topEdge_;
        pos.y = x % 16 - leftEdge_;
        return true;
    }
}

#endif // CO_H
