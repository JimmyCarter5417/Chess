#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "resmgr.h"

#include <vector>

using def::TPos;
using def::TDelta;
using def::byte;

class Board
{
public:
    Board();

    void init();
    ResMgr::EPiece getPiece(TPos pos);
    
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

    bool isValidKingMove(TPos prevPos, TPos nextPos);
    bool isValidAdvisorMove(TPos prevPos, TPos nextPos);
    bool isValidBishopMove(TPos prevPos, TPos nextPos);
    bool isValidKnightMove(TPos prevPos, TPos nextPos);
    bool isValidRookMove(TPos prevPos, TPos nextPos);
    bool isValidCannonMove(TPos prevPos, TPos nextPos);
    bool isValidPawnMove(TPos prevPos, TPos nextPos);

    bool isKnightFoot(TPos prevPos, TPos buddyPos);// 马腿
    bool isBishopEye(TPos prevPos, TPos buddyPos);// 象眼

//protected:
    byte operator[](TPos boardPos);

protected:
    typedef bool (*PosFunc)(TPos pos, bool red);//判断位置是否合法
    typedef bool (*DeltaFunc)(TDelta delta, bool red);//判断偏移量是否合法
    typedef bool (*VerifyFunc)(TPos prevPos, TPos nextPos);//棋子特定的走法
    bool isValidMove(TPos prevPos, TPos nextPos,
                     PosFunc isValidPos, DeltaFunc isValidDelta, VerifyFunc verify);

    inline bool isPiece(TPos pos, int piece);//判断pos位置是否为特定棋子（不分颜色）
    inline bool findPos(TPos pos, TPos* poss, int count);
    inline bool findDelta(TDelta delta, TDelta* deltas, int count);
    
private:
    std::vector<std::vector<byte>> board_;
};

#endif // BOARD_H
