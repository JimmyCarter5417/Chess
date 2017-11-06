#ifndef CO_H
#define CO_H

#include "def.h"
#include "size.h"

using def::TPos;
using def::TClientCo;

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
        return pos.row >= 0 && pos.row < g_row && pos.col >= 0 && pos.col < g_col;
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

        x = 16 * (pos.row + g_topEdge) + pos.col + g_leftEdge;
        return true;
    }

    inline bool x2xy(int x, TPos& pos)
    {
        if (!isValid1d(x))
            return false;

        pos.row = x / 16 - g_topEdge;
        pos.col = x % 16 - g_leftEdge;
        return true;
    }

    inline void clientCo2xy(TClientCo clientCo, TPos& pos)
    {
        pos.row = clientCo.y / size::g_pieceSize.height;
        pos.col = clientCo.x / size::g_pieceSize.width;
    }

    inline void xy2ClientCo(TPos pos, TClientCo& clientCo)
    {
        clientCo.x = size::g_boardLeftTopPos.col + size::g_pieceSize.height * pos.col - size::g_pieceSize.height / 2;
        clientCo.y = size::g_boardLeftTopPos.row + size::g_pieceSize.width * pos.row - size::g_pieceSize.width / 2;
    }
}

#endif // CO_H
