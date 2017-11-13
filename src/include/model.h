#ifndef MODEL_H
#define MODEL_H

#include <def.h>

namespace model
{
    enum EMoveRet
    {
        EMR_null = 0,
        EMR_ok = 0x10,
        EMR_suicide = 0x01,
        EMR_check = 0x02,
        EMR_dead = 0x04,
        EMR_eat = 0x08,
    };

    struct IModel
    {
        virtual void init() = 0;// 开局
        virtual bool rotate() = 0;// 翻转棋盘
        virtual bool undo() = 0;// 悔棋

        virtual int getScore(def::EPlayer player) const = 0;// 获取当前局面下的玩家分数
        virtual def::EPiece getPiece(def::TPos pos) const = 0;// 获取某一位置的棋子
        virtual def::EPlayer getNextPlayer() const = 0;// 获取下一走棋玩家
        virtual def::EPlayer getPieceOwner(def::TPos pos) const = 0;// 获取pos棋子所属玩家
        virtual std::pair<def::TPos, def::TPos> getTrigger() const = 0;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

        virtual def::byte movePiece(def::TPos prevPos, def::TPos currPos) = 0;// 尝试走棋，返回EMoveRet的组合
    };
}


#endif // MODEL_H
