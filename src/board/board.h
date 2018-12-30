#ifndef BOARD_H
#define BOARD_H

#include "util/def.h"

namespace board
{
    enum MOVE_RET_E
    {
        MOVE_RET_ok      = 0x01,// 可以走棋
        MOVE_RET_invalid = 0x02,// 无效走法
        MOVE_RET_suicide = 0x04,// 自杀(包括双王见面)
        MOVE_RET_check   = 0x08,// 将军
        MOVE_RET_dead    = 0x10,// 将死
        MOVE_RET_eat     = 0x20,// 吃子
    };

    struct IBoard
    {
        virtual ~IBoard(){}

        virtual void init() = 0;                                    // 开局
        virtual uint8_t autoMove() = 0;                             // 电脑走棋,返回EMoveRet的组合
        virtual uint8_t makeMove(def::TMove move) = 0;              // 指定走法走棋,返回EMoveRet的组合
        virtual bool undoMakeMove() = 0;                            // 悔棋

        virtual int getScore(def::PLAYER_E player) const = 0;       // 获取当前局面下的玩家分数
        virtual def::ICON_E getIcon(def::TPos pos) const = 0;       // 获取某一位置的棋子
        virtual def::PLAYER_E getOwner(def::TPos pos) const = 0;    // 获取pos棋子所属玩家
        virtual def::PLAYER_E getNextPlayer() const = 0;            // 获取下一走棋玩家
        virtual def::TMove getTrigger() const = 0;                  // 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
    };
}

#endif // BOARD_H
