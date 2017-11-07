#include "board.h"
#include "co.h"

#include <assert.h>
#include <math.h>
#include <memory.h>

using namespace def;
using namespace co;
using namespace std;

Board::Board()
{
    init();
}

void Board::init()
{
    static const std::vector<std::vector<byte>> initialBoard = {
             /* edge            mid            edge */
             /*  0   1   2   3   4   5   6   7   8  */
      /* 0 */ { 21, 20, 19, 18, 17, 18, 19, 20, 21 }, /*  edge */
      /* 1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 },
      /* 2 */ {  0, 22,  0,  0,  0,  0,  0, 22,  0 }, /* black */
      /* 3 */ { 23,  0, 23,  0, 23,  0, 23,  0, 23 },
      /* 4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /*  mid  */
      /* 5 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /*  mid  */
      /* 6 */ { 15,  0, 15,  0, 15,  0, 15,  0, 15 },
      /* 7 */ {  0, 14,  0,  0,  0,  0,  0, 14,  0 }, /*  red  */
      /* 8 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 },
      /* 9 */ { 13, 12, 11, 10,  9, 10, 11, 12, 13 }, /*  edge */
    };

    board_ = initialBoard;
}

byte Board::operator[](TPos pos)
{
    if (!co::isValidPos(pos))
        return 0;

    return board_[pos.row][pos.col];
}

ResMgr::EPiece Board::getPiece(TPos pos)
{
    if (!co::isValidPos(pos))
        return ResMgr::EP_Empty;

    return static_cast<ResMgr::EPiece>(board_[pos.row][pos.col]);
}

bool Board::isValidKingPos(TPos pos, bool red)
{ 
    static TPos redPos[] =
    {
        {7, 3}, {7, 4}, {7, 5},
        {8, 3}, {8, 4}, {8, 5},
        {9, 3}, {9, 4}, {9, 5}
    };

    static TPos blackPos[] =
    {
        {0, 3}, {0, 4}, {0, 5},
        {1, 3}, {1, 4}, {1, 5},
        {2, 3}, {2, 4}, {2, 5}
    };

    return findPos(pos, red ? redPos : blackPos, sizeof (redPos) / sizeof (TPos));
}

bool Board::isValidAdvisorPos(TPos pos, bool red)
{
    static TPos redPos[] =
    {
        {7, 3}, {7, 5},
        {8, 4},
        {9, 3}, {9, 5}
    };

    static TPos blackPos[] =
    {
        {0, 3}, {0, 5},
        {1, 4},
        {2, 3}, {2, 5}
    };

    return findPos(pos, red ? redPos : blackPos, sizeof (redPos) / sizeof (TPos));
}

bool Board::isValidBishopPos(TPos pos, bool red)
{
    static TPos redPos[] =
    {
        {5, 2}, {5, 6},
        {7, 0}, {7, 4}, {7, 8},
        {9, 2}, {9, 6}
    };

    static TPos blackPos[] =
    {
        {0, 2}, {0, 6},
        {2, 0}, {2, 4}, {2, 8},
        {3, 2}, {3, 6}
    };

    return findPos(pos, red ? redPos : blackPos, sizeof (redPos) / sizeof (TPos));
}

bool Board::isValidKnightPos(TPos pos, bool red)
{
    return co::isValidPos(pos);
}

bool Board::isValidRookPos(TPos pos, bool red)
{
    return co::isValidPos(pos);
}

bool Board::isValidCannonPos(TPos pos, bool red)
{
    return co::isValidPos(pos);
}

bool Board::isValidPawnPos(TPos pos, bool red)
{
    if (red)
    {
        return ((pos.row == 5 || pos.row == 6) && (pos.col % 2 == 0)) ||
               (pos.row >= 0 && pos.row <= 4 && pos.col >= 0 && pos.col <= 8);
    }
    else
    {
        return ((pos.row == 3 || pos.row == 4) && (pos.col % 2 == 0)) ||
               (pos.row >= 5 && pos.row <= 9 && pos.col >= 0 && pos.col <= 8);
    }
}

// 单格
bool Board::isValidKingDelta(TDelta delta, bool red)
{
    static TDelta deltas[] =
    {
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1}
    };

    return findDelta(delta, deltas, sizeof (deltas) / sizeof (TDelta));
}

// 单格对角线
bool Board::isValidAdvisorDelta(TDelta delta, bool red)
{
    static TDelta deltas[] =
    {
        {-1, -1},
        {-1,  1},
        { 1, -1},
        { 1,  1}
    };

    return findDelta(delta, deltas, sizeof (deltas) / sizeof (TDelta));
}

// 田字
bool Board::isValidBishopDelta(TDelta delta, bool red)
{
    static TDelta deltas[] =
    {
        {-2, -2},
        {-2,  2},
        { 2, -2},
        { 2,  2}
    };

    return findDelta(delta, deltas, sizeof (deltas) / sizeof (TDelta));
}

// 日字
bool Board::isValidKnightDelta(TDelta delta, bool red)
{
    static TDelta deltas[] =
    {
        {-2, -1},
        {-2,  1},
        {-1, -2},
        {-1,  2},
        { 1, -2},
        { 1,  2},
        { 2, -1},
        { 2,  1}
    };

    return findDelta(delta, deltas, sizeof (deltas) / sizeof (TDelta));
}

// 共线
bool Board::isValidRookDelta(TDelta delta, bool red)
{
    return (delta.deltaRow == 0 && delta.deltaCol != 0) ||
           (delta.deltaRow != 0 && delta.deltaCol == 0);
}

// 共线
bool Board::isValidCannonDelta(TDelta delta, bool red)
{
    return (delta.deltaRow == 0 && delta.deltaCol != 0) ||
           (delta.deltaRow != 0 && delta.deltaCol == 0);
}

// 前、左、右
bool Board::isValidPawnDelta(TDelta delta, bool red)
{
    if (red)
    {
        return delta == TDelta(-1,  0) ||
               delta == TDelta( 0, -1) ||
               delta == TDelta( 0,  1);
    }
    else
    {
        return delta == TDelta( 1,  0) ||
               delta == TDelta( 0, -1) ||
               delta == TDelta( 0,  1);
    }
}

bool Board::isValidKingMove(TPos prevPos, TPos nextPos)
{
    auto verify = [](TPos prevPos, TPos nextPos)->bool
    {
        return true;
    };

   //检查输入
    if (!isPiece(prevPos, g_king))
        return false;

    return isValidMove(prevPos, nextPos, isValidKingPos, isValidKingDelta, verify);
}

bool Board::isValidKnightMove(TPos prevPos, TPos nextPos)
{
    // 不能蹩马腿
    auto verify = [](TPos prevPos1, TPos nextPos1)->bool
    {
        TPos buddyPos = {(prevPos1.row + nextPos1.row) / 2, (prevPos1.col + nextPos1.col) / 2};
        return !isKnightFoot(prevPos1, buddyPos);
    };

    return isValidMove(prevPos, nextPos, isValidKnightPos, isValidKnightDelta, verify);
}

bool Board::isValidBishopMove(TPos prevPos, TPos nextPos)
{
    // 不能塞象眼
    auto verify = [](TPos prevPos, TPos nextPos)->bool
    {
        TPos buddyPos = {(prevPos.row + nextPos.row) / 2, (prevPos.col + nextPos.col) / 2};
        return !isBishopEye(prevPos, buddyPos);
    };

    //检查输入
    if (!isPiece(prevPos, g_bishop))
        return false;

    return isValidMove(prevPos, nextPos, isValidBishopPos, isValidBishopDelta, verify);
}

bool Board::isValidAdvisorMove(TPos prevPos, TPos nextPos)
{
    //检查输入
    if (!isPiece(prevPos, g_advisor))
        return false;

    return isValidMove(prevPos, nextPos, isValidAdvisorPos, isValidAdvisorDelta, nullptr);
}

bool Board::isValidRookMove(TPos prevPos, TPos nextPos)
{ 
    //检查输入
    if (!isPiece(prevPos, g_rook))
        return false;

    return isValidMove(prevPos, nextPos, isValidRookPos, isValidRookDelta, nullptr);
}

bool Board::isValidCannonMove(TPos prevPos, TPos nextPos)
{ 
    auto verify = [](TPos prevPos, TPos nextPos)
    {
        // todo
        return true;
    };

    //检查输入
    if (!isPiece(prevPos, g_cannon))
        return false;

    return isValidMove(prevPos, nextPos, isValidCannonPos, isValidCannonDelta, verify);
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

    return (buddyPos.row == knightPos.row && buddyPos.col == knightPos.col - 1) ||
           (buddyPos.row == knightPos.row && buddyPos.col == knightPos.col + 1) ||
           (buddyPos.row == knightPos.row - 1 && buddyPos.col == knightPos.col) ||
           (buddyPos.row == knightPos.row + 1 && buddyPos.col == knightPos.col);
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

    return (buddyPos.row == bishopPos.row - 1 && buddyPos.col == bishopPos.col - 1) ||
           (buddyPos.row == bishopPos.row - 1 && buddyPos.col == bishopPos.col + 1) ||
           (buddyPos.row == bishopPos.row + 1 && buddyPos.col == bishopPos.col - 1) ||
           (buddyPos.row == bishopPos.row + 1 && buddyPos.col == bishopPos.col + 1);
}

bool Board::isValidMove(TPos prevPos, TPos nextPos, PosFunc isValidPos, DeltaFunc isValidDelta, VerifyFunc verify)
{
    ResMgr::EPiece prevPiece = getPiece(prevPos);
    ResMgr::EPiece nextPiece = getPiece(nextPos);
    bool red = ((prevPiece & g_clrMask) == g_redFlag);

    // 目的位置与原位置棋子同色，则不能走
    if ((prevPiece & g_clrMask) == (nextPiece & g_clrMask))
        return false;

    // 走法前后的位置要有效
    if (isValidPos != nullptr)
    {
        if (!isValidPos(prevPos, red) || !isValidPos(nextPos, red))
            return false;
    }

    // delta是否符合规则
    if (isValidDelta != nullptr)
    {
        if (!isValidDelta(nextPos - prevPos, red))
            return false;
    }

    // 额外规则
    if (verify != nullptr)
    {
        if (!verify(prevPos, nextPos))
            return false;
    }

    return true;
}

bool Board::isPiece(TPos pos, int piece)
{
    return piece == (getPiece(pos) & g_pieceMask);
}

bool Board::findPos(TPos pos, TPos* poss, int count)
{
    assert(poss != nullptr && count > 0);

    for (int i = 0; i < count; ++i)
    {
        if (poss[i] == pos)
            return true;
    }

    return false;
}

bool Board::findDelta(TDelta delta, TDelta* deltas, int count)
{
    assert(deltas != nullptr && count > 0);

    for (int i = 0; i < count; i++)
    {
        if (deltas[i] == delta)
            return true;
    }

    return false;
}
