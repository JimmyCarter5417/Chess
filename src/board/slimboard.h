#ifndef SLIMBOARD_H
#define SLIMBOARD_H

#include "model/model.h"
#include <vector>
#include <stack>

using std::vector;
using std::stack;

class SlimBoard
{
public:
    SlimBoard();

    virtual void init();// 开局
    virtual bool undoMove();// 悔棋
    virtual def::int8 run();// 电脑走棋，返回EMoveRet的组合
    virtual def::int8 makeMove(def::TMove move);// 走棋，返回EMoveRet的组合
    virtual def::int8 makeMove(int index);// 走已生成走法中的某一个
    virtual int generateAllMoves();// 生成当前局面所有合法走法

    virtual int getScore(def::EPlayer player) const;// 获取当前局面下的玩家分数
    virtual def::EPiece getPiece(def::TPos pos) const;// 获取某一位置的棋子
    virtual def::EPlayer getNextPlayer() const;// 获取下一走棋玩家
    virtual def::EPlayer getPieceOwner(def::TPos pos) const;// 获取pos棋子所属玩家
    virtual def::TMove getTrigger() const;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
    virtual def::TMove getMove(int index) const;// 获取已生成走法中的某一个

protected:


private:
    struct TRecord
    {
        def::int8 capture;// dst坐标被捕获的棋子
        def::int8 src;    // src坐标
        def::int8 dst;    // dst坐标

        TRecord()
        {
            capture = 0;
            src = 0;
            dst = 0;
        }
    };

private:
    def::int8 board_[256];
    stack<TRecord> history_;
};

#endif // SLIMBOARD_H
