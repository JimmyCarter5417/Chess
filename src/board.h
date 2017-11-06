#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "resmgr.h"

using def::TPos;

class Board
{
public:
    Board();

    void init();
    ResMgr::EPiece getPiece(TPos pos);
    
    //二维数组判断较为简便
    bool isValidKingPos(TPos pos, bool red);
    bool isValidAdvisorPos(TPos pos, bool red);
    bool isValidRookPos(TPos pos, bool red);

    // 用一维数组可忽略边界条件
    bool isKnightFoot(TPos knightPos, TPos buddyPos);//马腿
    bool isBishopEye(TPos bishopPos, TPos buddyPos);//象眼
    
    bool isValidKnightMove(TPos knightPos, TPos nextPos);//马
    bool isValidBishopMove(TPos bishopPos, TPos nextPos);//象
    
    bool isValidKingMove(TPos kingPos, TPos nextPos);//将
    bool isValidAdvisorMove(TPos advisorPos, TPos nextPos);//士  
    
    bool isValidRookMove(TPos rookPos, TPos nextPos);//车
    bool isValidCannonMove(TPos cannonPos, TPos nextPos);//炮
    
private:
    def::byte board_[256];
};

#endif // BOARD_H
