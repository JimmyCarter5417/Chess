#ifndef CO_H
#define CO_H

#include "def.h"
#include "size.h"

using def::TPos;
using def::TClientCo;

namespace co
{
    // 二维数组尺寸
    const int g_rowNum = 10;
    const int g_colNum = 9;

    inline bool isValidPos(TPos pos)
    {
        return pos.row >= 0 && pos.row < g_rowNum &&
               pos.col >= 0 && pos.col < g_colNum;
    }  

    inline void clientCo2Pos(TClientCo clientCo, TPos& pos)
    {
        pos.row = (clientCo.y - size::g_boardLeftTopCo.y + size::g_pieceSize.height / 2) / size::g_pieceSize.height;
        pos.col = (clientCo.x - size::g_boardLeftTopCo.x + size::g_pieceSize.width / 2) / size::g_pieceSize.width;
    }

    inline void pos2ClientCo(TPos pos, TClientCo& clientCo)
    {
        clientCo.x = size::g_boardLeftTopCo.x + size::g_pieceSize.height * pos.col - size::g_pieceSize.height / 2;
        clientCo.y = size::g_boardLeftTopCo.y + size::g_pieceSize.width * pos.row - size::g_pieceSize.width / 2;
    }

    inline TPos getRotatePos(TPos pos)
    {
        return {co::g_rowNum - 1 - pos.row, co::g_colNum - 1 - pos.col};
    }
}

#endif // CO_H
