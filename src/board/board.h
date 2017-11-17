#ifndef BOARD_H
#define BOARD_H

#include "util/def.h"

namespace board
{
    enum EMoveRet
    {
        EMR_ok = 0x01,
        EMR_suicide = 0x02,
        EMR_check = 0x04,
        EMR_dead = 0x08,
        EMR_eat = 0x10,
    };

    struct IBoard
    {
        virtual ~IBoard(){}

        virtual void init() = 0;// 开局
        virtual bool undo() = 0;// 悔棋
        virtual def::int8 run() = 0;// 电脑走棋，返回EMoveRet的组合
        virtual def::int8 movePiece(def::TMove move) = 0;// 走棋，返回EMoveRet的组合
        virtual def::int8 movePiece(int index) = 0;// 走已生成走法中的某一个
        virtual int generateAllMoves() = 0;// 生成当前局面所有合法走法

        virtual int getScore(def::EPlayer player) const = 0;// 获取当前局面下的玩家分数
        virtual def::EPiece getPiece(def::TPos pos) const = 0;// 获取某一位置的棋子
        virtual def::EPlayer getNextPlayer() const = 0;// 获取下一走棋玩家
        virtual def::EPlayer getPieceOwner(def::TPos pos) const = 0;// 获取pos棋子所属玩家
        virtual def::TMove getTrigger() const = 0;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
        virtual def::TMove getMove(int index) const = 0;// 获取已生成走法中的某一个
    };
}

#endif // BOARD_H
