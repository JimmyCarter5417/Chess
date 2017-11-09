#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "resmgr.h"

#include <vector>
#include <unordered_set>

using def::TPos;
using def::TDelta;
using def::byte;
using std::vector;
using std::unordered_set;

class Board
{
public:
    Board();

    void init();

    ResMgr::EPiece getPiece(TPos pos) const;
    ResMgr::EPiece operator[](TPos Pos) const;

    bool movePiece(TPos prevPos, TPos currPos);
    void rotate();

    enum EPlayer
    {
        EP_Up,
        EP_Down,
        EP_None
    };

    EPlayer getNextPlayer() const;
    EPlayer getPieceOwner(TPos pos) const;

    bool changePlayer();

protected:
    bool isValidMove(TPos prevPos, TPos currPos);
    typedef bool (Board::*PosFunc)(TPos pos, EPlayer player);// 判断位置是否合法
    typedef bool (Board::*DeltaFunc)(TDelta delta, EPlayer player);// 判断偏移量是否合法
    typedef bool (Board::*RuleFunc)(TPos prevPos, TPos currPos);// 棋子特定的规则
    bool isValidMove(TPos prevPos, TPos currPos, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule);

    bool isValidKingPos(TPos pos, EPlayer player);
    bool isValidAdvisorPos(TPos pos, EPlayer player);
    bool isValidBishopPos(TPos pos, EPlayer player);
    bool isValidKnightPos(TPos pos, EPlayer player);
    bool isValidRookPos(TPos pos, EPlayer player);
    bool isValidCannonPos(TPos pos, EPlayer player);
    bool isValidPawnPos(TPos pos, EPlayer player);

    bool isValidKingDelta(TDelta delta, EPlayer player);
    bool isValidAdvisorDelta(TDelta delta, EPlayer player);
    bool isValidBishopDelta(TDelta delta, EPlayer player);
    bool isValidKnightDelta(TDelta delta, EPlayer player);
    bool isValidRookDelta(TDelta delta, EPlayer player);
    bool isValidCannonDelta(TDelta delta, EPlayer player);
    bool isValidPawnDelta(TDelta delta, EPlayer player);

    // 不再检查pos及delta，默认前面已检查
    bool isValidKingRule(TPos prevPos, TPos currPos);
    bool isValidAdvisorRule(TPos prevPos, TPos currPos);
    bool isValidBishopRule(TPos prevPos, TPos currPos);
    bool isValidKnightRule(TPos prevPos, TPos currPos);
    bool isValidRookRule(TPos prevPos, TPos currPos);
    bool isValidCannonRule(TPos prevPos, TPos currPos);
    bool isValidPawnRule(TPos prevPos, TPos currPos);

    bool isValidKingMove(TPos prevPos, TPos currPos);
    bool isValidAdvisorMove(TPos prevPos, TPos currPos);
    bool isValidBishopMove(TPos prevPos, TPos currPos);
    bool isValidKnightMove(TPos prevPos, TPos currPos);
    bool isValidRookMove(TPos prevPos, TPos currPos);
    bool isValidCannonMove(TPos prevPos, TPos currPos);
    bool isValidPawnMove(TPos prevPos, TPos currPos); 

    inline bool isPiece(TPos pos, int piece);// 判断pos位置是否为特定棋子（不分颜色）
    bool isAttactivePiece(TPos pos);

    bool isChecked(Board::EPlayer player);
    bool isSuicide(TPos prevPos, TPos currPos);

private:
    struct TPieceSet
    {
        TPos king;
        unordered_set<TPos> attackers;

        void swap(TPieceSet& another)
        {
            std::swap(king, another.king);
            attackers.swap(another.attackers);
        }
    };

    vector<vector<byte>> board_;
    EPlayer player_;// who's turn for next move

    TPieceSet upPieceSet_;
    TPieceSet downPieceSet_;
};

#endif // BOARD_H
