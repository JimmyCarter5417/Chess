#ifndef BOARD_H
#define BOARD_H

#include "util/def.h"

namespace board
{
    enum EMoveRet
    {
        EMR_ok      = 0x01,// 可以走棋
        EMR_invalid = 0x02,// 无效走法
        EMR_suicide = 0x04,// 自杀(包括双王见面)
        EMR_check   = 0x08,// 将军
        EMR_dead    = 0x10,// 将死
        EMR_eat     = 0x20,// 吃子
    };

    struct IBoard
    {
        virtual ~IBoard(){}

        virtual void init() = 0;                                    // 开局
        virtual uint8_t autoMove() = 0;                             // 电脑走棋,返回EMoveRet的组合
        virtual uint8_t makeMove(def::TMove move) = 0;              // 指定走法走棋,返回EMoveRet的组合
        virtual bool undoMakeMove() = 0;                            // 悔棋

        virtual int getScore(def::EPlayer player) const = 0;        // 获取当前局面下的玩家分数
        virtual def::EPiece getPiece(def::TPos pos) const = 0;      // 获取某一位置的棋子
        virtual def::EPlayer getNextPlayer() const = 0;             // 获取下一走棋玩家
        virtual def::EPlayer getPieceOwner(def::TPos pos) const = 0;// 获取pos棋子所属玩家
        virtual def::TMove getTrigger() const = 0;                  // 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
    };
}

#endif // BOARD_H
