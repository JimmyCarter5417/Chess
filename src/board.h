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

    enum EMoveRet
    {
        EMR_null = 0,
        EMR_ok = 0x10,
        EMR_suicide = 0x01,
        EMR_check = 0x02,
        EMR_dead = 0x03,
    };

    byte movePiece(TPos prevPos, TPos currPos);
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
    // 对于player来说，从prevPos到currPos是否合法
    bool isValidMove(TPos prevPos, TPos currPos, Board::EPlayer player);

    typedef bool (Board::*PosFunc)(TPos pos, EPlayer player);// 判断位置是否合法
    typedef bool (Board::*DeltaFunc)(TDelta delta, EPlayer player);// 判断偏移量是否合法
    typedef bool (Board::*RuleFunc)(TPos prevPos, TPos currPos);// 棋子特定的规则
    bool isValidMove(TPos prevPos, TPos currPos, Board::EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule);

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

    bool isValidKingMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidAdvisorMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidBishopMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidKnightMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidRookMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidCannonMove(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool isValidPawnMove(TPos prevPos, TPos currPos, Board::EPlayer player);

    const unordered_set<TDelta>& getValidKingDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidAdvisorDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidBishopDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidKnightDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidRookDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidCannonDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidPawnDelta(EPlayer player) const;

    bool check(Board::EPlayer player);
    bool checkmate(Board::EPlayer player);

    bool isSuicide(TPos prevPos, TPos currPos, Board::EPlayer player);
    bool updatePieceSet(TPos prevPos, TPos currPos, Board::EPlayer player);

    inline bool isPiece(TPos pos, int piece);// 判断pos位置是否为特定棋子（不分颜色）    
    inline bool isKing(TPos pos);
    inline bool isDefensivePiece(TPos pos);
    inline bool isAttactivePiece(TPos pos);

private:
    // 玩家棋子位置集合
    struct TPieceSet
    {
        TPos king;
        unordered_set<TPos> defenders;
        unordered_set<TPos> attackers;

        void swap(TPieceSet& another)
        {
            std::swap(king, another.king);
            defenders.swap(another.defenders);
            attackers.swap(another.attackers);
        }
    };

    vector<vector<byte>> board_;// 棋盘
    Board::EPlayer player_;// who's turn for the next move

    TPieceSet upPieceSet_;// 上方玩家棋子集合
    TPieceSet downPieceSet_;// 下方玩家棋子集合
};

#endif // BOARD_H
