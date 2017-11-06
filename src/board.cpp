#include "board.h"
#include "co.h"

#include <math.h>
#include <memory.h>

using namespace def;
using namespace co;
using namespace std;

Board::Board()
{
    memset(board_, 0, sizeof(board_));
    init();
}

void Board::init()
{
    // up: black, down: red
    // 0: empty, !0: piece
    static const byte initialBoard[256] = {
              //  0               4               8
              // end             mid             end
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 21, 20, 19, 18, 17, 18, 19, 20, 21,  0,  0,  0,  0,// end 0
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0, 22,  0,  0,  0,  0,  0, 22,  0,  0,  0,  0,  0,// black
      0,  0,  0, 23,  0, 23,  0, 23,  0, 23,  0, 23,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,// mid 4
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,// mid 5
      0,  0,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0,  0,  0,  0,
      0,  0,  0,  0, 14,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,// red
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 13, 12, 11, 10,  9, 10, 11, 12, 13,  0,  0,  0,  0,// end 9
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };        // end             mid             end
              //  0               4               8

    memcpy(board_, initialBoard, sizeof(board_));
}

ResMgr::EPiece Board::getPiece(TPos pos)
{
    int x = -1;

    if (!co::xy2x(pos, x))
        return ResMgr::EP_Empty;
    else
        return static_cast<ResMgr::EPiece>(board_[x]);
}

bool Board::isValidKingPos(TPos pos, bool red)
{ 
    if (red)
        return pos.row >= 7 && pos.row <= 9 &&
               pos.col >= 3 && pos.col <= 5;
    else
        return pos.row >= 0 && pos.row <= 2 &&
               pos.col >= 3 && pos.col <= 5;    
}

bool Board::isValidAdvisorPos(TPos pos, bool red)
{
    if (red)
        return (pos.row == 9 && pos.col == 3) ||
               (pos.row == 9 && pos.col == 5) ||
               (pos.row == 8 && pos.col == 4) ||
               (pos.row == 7 && pos.col == 3) ||
               (pos.row == 7 && pos.col == 5);
    else
        return (pos.row == 0 && pos.col == 3) ||
               (pos.row == 0 && pos.col == 5) ||
               (pos.row == 1 && pos.col == 4) ||
               (pos.row == 2 && pos.col == 3) ||
               (pos.row == 2 && pos.col == 5);
}

bool Board::isValidRookPos(TPos pos, bool red)
{
    if (red)
        return (pos.row == 9 && pos.col == 2) ||
               (pos.row == 9 && pos.col == 6) ||
               (pos.row == 7 && pos.col == 0) ||
               (pos.row == 7 && pos.col == 4) ||
               (pos.row == 7 && pos.col == 8) ||
               (pos.row == 5 && pos.col == 2) ||
               (pos.row == 5 && pos.col == 6);
    else
        return (pos.row == 0 && pos.col == 2) ||
               (pos.row == 0 && pos.col == 6) ||
               (pos.row == 2 && pos.col == 0) ||
               (pos.row == 2 && pos.col == 4) ||
               (pos.row == 2 && pos.col == 8) ||
               (pos.row == 3 && pos.col == 2) ||
               (pos.row == 3 && pos.col == 6);
}

bool Board::isKnightFoot(TPos knightPos, TPos buddyPos)
{
    //马位置非马，则为假
    ResMgr::EPiece piece = getPiece(knightPos);
    if (g_knight != (piece & g_pieceMask))
        return false;

    //马腿位置为空，则为假
    piece = getPiece(buddyPos);
    if (piece == ResMgr::EP_Empty)
        return false;

    //二维坐标转为一维坐标
    int knight, buddy;
    if (!co::xy2x(knightPos, knight) || !co::xy2x(buddyPos, buddy))
        return false;

     return buddy == knight - 16 ||
            buddy == knight - 1  ||
            buddy == knight + 1  ||
            buddy == knight + 16;
}

bool Board::isBishopEye(TPos bishopPos, TPos buddyPos)
{
    //马位置非马，则为假
    ResMgr::EPiece piece = getPiece(bishopPos);
    if (g_bishop != (piece & g_pieceMask))
        return false;

    //象眼位置为空，则为假
    piece = getPiece(buddyPos);
    if (piece == ResMgr::EP_Empty)
        return false;

    //二维坐标转为一维坐标
    int bishop, buddy;
    if (!co::xy2x(bishopPos, bishop) || !co::xy2x(buddyPos, buddy))
        return false;

     return buddy == bishop - 17 ||
            buddy == bishop - 15 ||
            buddy == bishop + 15 ||
            buddy == bishop + 17;
}

bool Board::isValidKnightMove(TPos knightPos, TPos nextPos)
{
    //马位置非马，则为假
    ResMgr::EPiece piece = getPiece(knightPos);
    if (g_knight != (piece & g_pieceMask))
        return false;

    //二维坐标转为一维坐标
    int knight, next;
    if (!co::xy2x(knightPos, knight) || !co::xy2x(nextPos, next))
        return false;

    //有效走法
    if (next != knight - 33 &&
        next != knight - 31 &&
        next != knight - 18 &&
        next != knight - 14 &&
        next != knight + 14 &&
        next != knight + 18 &&
        next != knight + 31 &&
        next != knight + 33)
        return false;

    //不能蹩马腿
    TPos buddyPos = {(knightPos.row + nextPos.row) / 2, (knightPos.col + nextPos.col) / 2};
    if (isKnightFoot(knightPos, buddyPos))
        return false;

    //目的位置为空或异色棋子才能走
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if ((nextPiece & g_clrMask) == (piece & g_clrMask))
        return false;

    return true;
}

bool Board::isValidBishopMove(TPos bishopPos, TPos nextPos)
{
    //象位置非象，则为假
    ResMgr::EPiece piece = getPiece(bishopPos);
    if (g_bishop != (piece & g_pieceMask))
        return false;

    //二维坐标转为一维坐标
    int bishop, next;
    if (!co::xy2x(bishopPos, bishop) || !co::xy2x(nextPos, next))
        return false;

    //有效走法
    if (next != bishop - 34 &&
        next != bishop - 30 &&
        next != bishop + 30 &&
        next != bishop + 34)
        return false;

    //不能塞象眼
    TPos buddyPos = {(bishopPos.row + nextPos.row) / 2, (bishopPos.col + nextPos.col) / 2};
    if (isBishopEye(bishopPos, buddyPos))
        return false;

    //目的位置为空或异色棋子才能走
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if ((nextPiece & g_clrMask) == (piece & g_clrMask))
        return false;

    return true;
}

bool Board::isValidKingMove(TPos kingPos, TPos nextPos)
{    
    //将位置非将，则为假
    ResMgr::EPiece piece = getPiece(kingPos);
    if (g_king != (piece & g_pieceMask))
        return false;

    //走法前后的位置要有效
    bool red = ((piece & g_clrMask) == g_redFlag);
    if (!isValidKingPos(kingPos, red) || !isValidKingPos(nextPos, red))
        return false;
    
    //二维坐标转为一维坐标
    int king, next;
    if (!co::xy2x(kingPos, king) || !co::xy2x(nextPos, next))
        return false;    

    //目的位置为空或异色棋子才能走
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if ((nextPiece & g_clrMask) == (piece & g_clrMask))
        return false;
    
    //保证相邻
    return next == king - 16 ||
           next == king - 1  ||
           next == king + 1  ||
           next == king + 16;
}

bool Board::isValidAdvisorMove(TPos advisorPos, TPos nextPos)
{
    //士位置非士，则为假
    ResMgr::EPiece piece = getPiece(advisorPos);
    if (g_advisor != (piece & g_pieceMask))
        return false;

    //走法前后的位置要有效
    bool red = ((piece & g_clrMask) == g_redFlag);
    if (!isValidAdvisorPos(advisorPos, red) || !isValidKingPos(nextPos, red))
        return false;
    
    //二维坐标转为一维坐标
    int advisor, next;
    if (!co::xy2x(advisorPos, advisor) || !co::xy2x(nextPos, next))
        return false;    

    //目的位置为空或异色棋子才能走
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if ((nextPiece & g_clrMask) == (piece & g_clrMask))
        return false;
    
    //保证对角线
    return next == advisor - 17 ||
           next == advisor - 15 ||
           next == advisor + 15 ||
           next == advisor + 17;
}

bool Board::isValidRookMove(TPos rookPos, TPos nextPos)
{ 
    //车位置非车，则为假
    ResMgr::EPiece piece = getPiece(rookPos);
    if (g_rook != (piece & g_pieceMask))
        return false;
    
    //二维坐标转为一维坐标
    int rook, next;
    if (!co::xy2x(rookPos, rook) || !co::xy2x(nextPos, next))
        return false;    

    int div = abs(rook - next) / 16;
    int mod = abs(rook - next) % 16;
    //保证共线    
    if ((div != 0 && mod != 0) || (div == 0 && mod == 0))
        return false;

    //中间不能有棋子
    int step = (div != 0) ? 16 : 1;
    int cur = min(rook, next) + step;
    int end = max(rook, next) - step;

    while (cur <= end)
    {
        if (board_[cur] != 0)
            return false;

        cur += step;
    }

    //目的位置为空或异色棋子才能走
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if ((nextPiece & g_clrMask) == (piece & g_clrMask))
        return false;

    return true;
}

bool Board::isValidCannonMove(TPos cannonPos, TPos nextPos)
{ 
    //车位置非车，则为假
    ResMgr::EPiece piece = getPiece(cannonPos);
    if (g_cannon != (piece & g_pieceMask))
        return false;
    
    //二维坐标转为一维坐标
    int cannon, next;
    if (!co::xy2x(cannonPos, cannon) || !co::xy2x(nextPos, next))
        return false;    
    
    int div = abs(cannon - next) / 16;
    int mod = abs(cannon - next) % 16;
    //保证共线
    if ((div != 0 && mod != 0) || (div == 0 && mod == 0))
        return false;

    //中间最多有一个棋子
    int step = (div != 0) ? 16 : 1;
    int cur = min(cannon, next) + step;
    int end = max(cannon, next) - step;
    bool flag = false;//中间是否有棋子
    while (cur <= end)
    {
        if (board_[cur] != 0)
        {
            if (flag)
                return false;
            else
                flag = true;
        }

        cur += step;
    }

    ResMgr::EPiece nextPiece = getPiece(nextPos);
    if (flag)
    {
        //不能打同色
        if ((nextPiece & g_clrMask) == (piece & g_clrMask) ||
            (nextPiece & g_pieceMask) == g_empty)
            return false;
    }
    else
    {
        //目的位置有棋子则不能走
        if ((nextPiece & g_pieceMask) != g_empty)
            return false;
    }

    return true;
}
