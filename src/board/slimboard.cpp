#include "slimboard.h"

#include <memory>

using namespace std;

SlimBoard::SlimBoard()
{

}

void SlimBoard::init()// 开局
{
    static const def::int8 initBoard[256] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 21, 20, 19, 18, 17, 18, 19, 20, 21,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0, 22,  0,  0,  0,  0,  0, 22,  0,  0,  0,  0,  0,
      0,  0,  0, 23,  0, 23,  0, 23,  0, 23,  0, 23,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0,  0,  0,  0,
      0,  0,  0,  0, 14,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 13, 12, 11, 10,  9, 10, 11, 12, 13,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };

    memcpy(board_, initBoard_, 256);
    stack<TRecord> tmp;
    history_.swap(tmp);
}

bool SlimBoard::undoMove()// 悔棋
{
    if (history_.empty())
        return false;

    TRecord record = history_.top();
    history_.pop();
    board_[record.src] = board_[record.dst];
    board_[record.dst] = record.capture;

    return true;
}

def::int8 SlimBoard::run()// 电脑走棋，返回EMoveRet的组合
{
    // todo
    return 1;
}

def::int8 SlimBoard::makeMove(def::TMove move)// 走棋，返回EMoveRet的组合
{
    // todo
    return 1;
}

def::int8 SlimBoard::makeMove(int index)// 走已生成走法中的某一个
{
    // todo
    return 1;
}

int SlimBoard::generateAllMoves()// 生成当前局面所有合法走法
{
    // todo
    return 1;
}

int SlimBoard::getScore(def::EPlayer player) const// 获取当前局面下的玩家分数
{
    // todo
    return 1;
}

def::EPiece SlimBoard::getPiece(def::TPos pos) cons;// 获取某一位置的棋子
{
    // todo
    return def::EP_blackAdvisor;
}

def::EPlayer SlimBoard::getNextPlayer() const;// 获取下一走棋玩家
{
    // todo
    return def::EP_black;
}

def::EPlayer SlimBoard::getPieceOwner(def::TPos pos) const// 获取pos棋子所属玩家
{
    // todo
    return {1, 1};
}

def::TMove SlimBoard::getTrigger() const// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
{
    // todo
    return true;
}

def::TMove SlimBoard::getMove(int index) const// 获取已生成走法中的某一个
{
    // todo
    return {1, 1};
}
