#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "resmgr.h"

#include <vector>

using def::TPos;
using def::TDelta;
using def::byte;
using std::vector;

class Board
{
public:
    Board();

    void init();

    ResMgr::EPiece getPiece(TPos pos) const;
    ResMgr::EPiece operator[](TPos Pos) const;

    bool movePiece(TPos prevPos, TPos currPos);

protected:
    bool isValidMove(TPos prevPos, TPos currPos);
    typedef bool (Board::*PosFunc)(TPos pos, bool red);// 判断位置是否合法
    typedef bool (Board::*DeltaFunc)(TDelta delta, bool red);// 判断偏移量是否合法
    typedef bool (Board::*RuleFunc)(TPos prevPos, TPos currPos);// 棋子特定的规则
    bool isValidMove(TPos prevPos, TPos currPos, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule);

    bool isValidKingPos(TPos pos, bool red);
    bool isValidAdvisorPos(TPos pos, bool red);
    bool isValidBishopPos(TPos pos, bool red);
    bool isValidKnightPos(TPos pos, bool red);
    bool isValidRookPos(TPos pos, bool red);
    bool isValidCannonPos(TPos pos, bool red);
    bool isValidPawnPos(TPos pos, bool red);

    bool isValidKingDelta(TDelta delta, bool red);
    bool isValidAdvisorDelta(TDelta delta, bool red);
    bool isValidBishopDelta(TDelta delta, bool red);
    bool isValidKnightDelta(TDelta delta, bool red);
    bool isValidRookDelta(TDelta delta, bool red);
    bool isValidCannonDelta(TDelta delta, bool red);
    bool isValidPawnDelta(TDelta delta, bool red);

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
    
private:
    vector<vector<byte>> board_;


};

#endif // BOARD_H
