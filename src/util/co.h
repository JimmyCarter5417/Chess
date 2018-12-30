#ifndef CO_H
#define CO_H

#include "def.h"

namespace co
{
    // 二维数组尺寸
    const int g_rowNum = 10;
    const int g_colNum = 9;

    const def::TClientCo g_boardLeftTopCo = {36, 59};
    const def::TSize g_boardSize = {521, 577};
    const def::TSize g_iconSize = {56, 56};

    inline bool isValidPos(def::TPos pos)
    {
        return pos.row >= 0 && pos.row < g_rowNum &&
               pos.col >= 0 && pos.col < g_colNum;
    }  

    inline def::TPos getRotatePos(def::TPos pos)
    {
        return {g_rowNum - 1 - pos.row, g_colNum - 1 - pos.col};
    }

    inline def::TPos clientCo2Pos(def::TClientCo clientCo)
    {
        return {(clientCo.y - g_boardLeftTopCo.y + g_iconSize.height / 2) / g_iconSize.height,
                (clientCo.x - g_boardLeftTopCo.x + g_iconSize.width / 2) / g_iconSize.width};
    }

    inline def::TClientCo pos2ClientCo(def::TPos pos)
    {
        return {g_boardLeftTopCo.x + g_iconSize.width * pos.col - g_iconSize.width / 2,
                g_boardLeftTopCo.y + g_iconSize.height * pos.row - g_iconSize.height / 2};
    }    
}

#endif // CO_H
