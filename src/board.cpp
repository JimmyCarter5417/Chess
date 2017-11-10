#include "board.h"
#include "co.h"

#include <assert.h>
#include <math.h>
#include <algorithm>
#include <memory.h>
#include <unordered_set>
#include <unordered_map>

using namespace def;
using namespace co;
using namespace std;


//第四位和第五位表示上下部分
static byte g_downFlag = 0x8;// 默认下面为红色
static byte g_upFlag   = 0x10;// 默认上面为黑色

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
      /* 2 */ {  0, 22,  0,  0,  0,  0,  0, 22,  0 }, /*  up   */
      /* 3 */ { 23,  0, 23,  0, 23,  0, 23,  0, 23 },
      /* 4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /*  mid  */
      /* 5 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 }, /*  mid  */
      /* 6 */ { 15,  0, 15,  0, 15,  0, 15,  0, 15 },
      /* 7 */ {  0, 14,  0,  0,  0,  0,  0, 14,  0 }, /*  down */
      /* 8 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0 },
      /* 9 */ { 13, 12, 11, 10,  9, 10, 11, 12, 13 }, /*  edge */
    };

    board_ = initialBoard;

    player_ = Board::EP_Down;// 下方先走

    upPieceSet_.king = {0, 4};
    upPieceSet_.defenders =
    {
        {0, 2},
        {0, 3},
        {0, 5},
        {0, 6},
    };
    upPieceSet_.attackers =
    {
        {0, 0},
        {0, 1},
        {0, 7},
        {0, 8},
        {2, 1},
        {2, 7},
        {3, 0},
        {3, 2},
        {3, 4},
        {3, 6},
        {3, 8},
    };

    downPieceSet_.king = {9, 4};
    downPieceSet_.defenders =
    {
        {9, 2},
        {9, 3},
        {9, 5},
        {9, 6},
    };
    downPieceSet_.attackers =
    {
        {9, 0},
        {9, 1},
        {9, 7},
        {9, 8},
        {7, 1},
        {7, 7},
        {6, 0},
        {6, 2},
        {6, 4},
        {6, 6},
        {6, 8},
    };
}

ResMgr::EPiece Board::operator[](TPos pos) const
{
    if (!co::isValidPos(pos))
        return ResMgr::EP_Empty;

    return static_cast<ResMgr::EPiece>(board_[pos.row][pos.col]);
}

ResMgr::EPiece Board::getPiece(TPos pos) const
{
    return (*this)[pos];
}

Board::EPlayer Board::getNextPlayer() const
{
    return player_;
}

Board::EPlayer Board::getPieceOwner(TPos pos) const
{
    byte piece = getPiece(pos);
    if ((piece & g_scopeMask) == g_upFlag)
        return Board::EP_Up;
    else if ((piece & g_scopeMask) == g_downFlag)
        return Board::EP_Down;
    else
        return Board::EP_None;
}

bool Board::isValidKingPos(TPos pos, EPlayer player)
{ 
    static unordered_set<TPos> downPos =
    {
        {7, 3}, {7, 4}, {7, 5},
        {8, 3}, {8, 4}, {8, 5},
        {9, 3}, {9, 4}, {9, 5}
    };

    static unordered_set<TPos> upPos =
    {
        {0, 3}, {0, 4}, {0, 5},
        {1, 3}, {1, 4}, {1, 5},
        {2, 3}, {2, 4}, {2, 5}
    };

    if (player == Board::EP_Down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == Board::EP_Up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidAdvisorPos(TPos pos, EPlayer player)
{
    static unordered_set<TPos> downPos =
    {
        {7, 3}, {7, 5},
        {8, 4},
        {9, 3}, {9, 5}
    };

    static unordered_set<TPos> upPos =
    {
        {0, 3}, {0, 5},
        {1, 4},
        {2, 3}, {2, 5}
    };

    if (player == Board::EP_Down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == Board::EP_Up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidBishopPos(TPos pos, EPlayer player)
{
    static unordered_set<TPos> downPos =
    {
        {5, 2}, {5, 6},
        {7, 0}, {7, 4}, {7, 8},
        {9, 2}, {9, 6}
    };

    static unordered_set<TPos> upPos =
    {
        {0, 2}, {0, 6},
        {2, 0}, {2, 4}, {2, 8},
        {3, 2}, {3, 6}
    };

    if (player == Board::EP_Down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == Board::EP_Up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidKnightPos(TPos pos, EPlayer player)
{
    return co::isValidPos(pos);
}

bool Board::isValidRookPos(TPos pos, EPlayer player)
{
    return co::isValidPos(pos);
}

bool Board::isValidCannonPos(TPos pos, EPlayer player)
{
    return co::isValidPos(pos);
}

bool Board::isValidPawnPos(TPos pos, EPlayer player)
{
    if (player == Board::EP_Down)
    {
        return ((pos.row == 5 || pos.row == 6) && (pos.col % 2 == 0)) ||
               (pos.row >= 0 && pos.row <= 4 && pos.col >= 0 && pos.col <= 8);
    }
    else if (player == Board::EP_Up)
    {
        return ((pos.row == 3 || pos.row == 4) && (pos.col % 2 == 0)) ||
               (pos.row >= 5 && pos.row <= 9 && pos.col >= 0 && pos.col <= 8);
    }
    else
    {
        return false;
    }
}

const unordered_set<TDelta>& Board::getValidKingDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
    {
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1}
    };

    return deltas;
}

const unordered_set<TDelta>& Board::getValidAdvisorDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
    {
        {-1, -1},
        {-1,  1},
        { 1, -1},
        { 1,  1}
    };

    return deltas;
}

const unordered_set<TDelta>& Board::getValidBishopDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
    {
        {-2, -2},
        {-2,  2},
        { 2, -2},
        { 2,  2}
    };

    return deltas;
}
const unordered_set<TDelta>& Board::getValidKnightDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
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

    return deltas;
}
const unordered_set<TDelta>& Board::getValidRookDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
    {
        {0, -8}, {0, -7}, {0, -6}, {0, -5}, {0, -4}, {0, -3}, {0, -2}, {0, -1},
        {0,  8}, {0,  7}, {0,  6}, {0,  5}, {0,  4}, {0,  3}, {0,  2}, {0,  1},

        {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
        { 9, 0}, { 8, 0}, { 7, 0}, { 6, 0}, { 5, 0}, { 4, 0}, { 3, 0}, { 2, 0}, { 1, 0},
    };

    return deltas;
}
const unordered_set<TDelta>& Board::getValidCannonDelta(EPlayer player) const
{
    static const unordered_set<TDelta> deltas =
    {
        {0, -8}, {0, -7}, {0, -6}, {0, -5}, {0, -4}, {0, -3}, {0, -2}, {0, -1},
        {0,  8}, {0,  7}, {0,  6}, {0,  5}, {0,  4}, {0,  3}, {0,  2}, {0,  1},

        {-9, 0}, {-8, 0}, {-7, 0}, {-6, 0}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0},
        { 9, 0}, { 8, 0}, { 7, 0}, { 6, 0}, { 5, 0}, { 4, 0}, { 3, 0}, { 2, 0}, { 1, 0},
    };

    return deltas;
}
const unordered_set<TDelta>& Board::getValidPawnDelta(EPlayer player) const
{
    static const unordered_set<TDelta> downDeltas =
    {
        {-1,  0},
        { 0, -1},
        { 0,  1},
    };

    static const unordered_set<TDelta> upDeltas =
    {
        { 1,  0},
        { 0, -1},
        { 0,  1},
    };

    return player == Board::EP_Down ? downDeltas : upDeltas;
}

// 单格
bool Board::isValidKingDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidKingDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 单格对角线
bool Board::isValidAdvisorDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidAdvisorDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 田字
bool Board::isValidBishopDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidBishopDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 日字
bool Board::isValidKnightDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidKnightDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidRookDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidRookDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidCannonDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidCannonDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 前、左、右
bool Board::isValidPawnDelta(TDelta delta, EPlayer player)
{
    const unordered_set<TDelta> deltas = getValidPawnDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidKingRule(TPos prevPos, TPos currPos)
{    
    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidAdvisorRule(TPos prevPos, TPos currPos)
{ 
    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidBishopRule(TPos prevPos, TPos currPos)
{
    // 计算象眼位置
    TPos buddyPos = {(prevPos.row + currPos.row) / 2, (prevPos.col + currPos.col) / 2};

    // 不能塞象眼
    return getPiece(buddyPos) == ResMgr::EP_Empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidKnightRule(TPos prevPos, TPos currPos)
{    
    // 计算马腿位置
    TPos buddyPos = g_nullPos;
    TDelta delta = currPos - prevPos;
    if (delta == TDelta(-1, 2) || delta == TDelta(1, 2))
        buddyPos = {prevPos.row, prevPos.col + 1};
    else if (delta == TDelta(-2, -1) || delta == TDelta(-2, 1))
        buddyPos = {prevPos.row - 1, prevPos.col};
    else if (delta == TDelta(-1, -2) || delta == TDelta(1, -2))
        buddyPos = {prevPos.row, prevPos.col - 1};
    else if (delta == TDelta(2, -1) || delta == TDelta(2, 1))
        buddyPos = {prevPos.row + 1, prevPos.col};

    // 不能蹩马腿
    return getPiece(buddyPos) == ResMgr::EP_Empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidRookRule(TPos prevPos, TPos currPos)
{
    // 中间不能有棋子
    if (prevPos.row == currPos.row)// 同一行
    {
        int cur = min(prevPos.col, currPos.col) + 1;
        int end = max(prevPos.col, currPos.col) - 1;
        while (cur <= end)
        {
            if (ResMgr::EP_Empty != getPiece({prevPos.row, cur}))
                return false;

            cur++;
        }
    }
    else// 同一列
    {
        int cur = min(prevPos.row, currPos.row) + 1;
        int end = max(prevPos.row, currPos.row) - 1;
        while (cur <= end)
        {
            if (ResMgr::EP_Empty != getPiece({cur, prevPos.col}))
                return false;

            cur++;
        }
    }

    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidCannonRule(TPos prevPos, TPos currPos)
{ 
    if (getPiece(currPos) == ResMgr::EP_Empty)// 目的位置为空，则中间不能有棋子
    {
        if (prevPos.row == currPos.row)// 同一行
        {
            int cur = min(prevPos.col, currPos.col) + 1;
            int end = max(prevPos.col, currPos.col) - 1;
            while (cur <= end)
            {
                if (ResMgr::EP_Empty != getPiece({prevPos.row, cur}))
                    return false;

                cur++;
            }

            return true;
        }
        else// 同一列
        {
            int cur = min(prevPos.row, currPos.row) + 1;
            int end = max(prevPos.row, currPos.row) - 1;
            while (cur <= end)
            {
                if (ResMgr::EP_Empty != getPiece({cur, prevPos.col}))
                    return false;

                cur++;
            }

            return true;
        }
    }
    else// 目的位置非空，则中间有且仅有一个棋子
    {
        if (prevPos.row == currPos.row)// 同一行
        {
            bool flag = false;
            int cur = min(prevPos.col, currPos.col) + 1;
            int end = max(prevPos.col, currPos.col) - 1;
            while (cur <= end)
            {
                if (ResMgr::EP_Empty != getPiece({prevPos.row, cur}))
                {
                    if (flag)
                        return false;
                    else
                        flag = true;
                }

                cur++;
            }

            return flag;
        }
        else// 同一列
        {
            bool flag = false;
            int cur = min(prevPos.row, currPos.row) + 1;
            int end = max(prevPos.row, currPos.row) - 1;
            while (cur <= end)
            {
                if (ResMgr::EP_Empty != getPiece({cur, prevPos.col}))
                {
                    if (flag)
                        return false;
                    else
                        flag = true;
                }

                cur++;
            }

            return flag;
        }
    }

    return false;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidPawnRule(TPos prevPos, TPos currPos)
{
    if ((getPiece(prevPos) & g_scopeMask) == g_downFlag)// 下方卒
    {
        if (prevPos.row >= 5)// 未过河，只能向前走一步，row减一
            return currPos.row - prevPos.row == -1;
    }
    else
    {
        if (prevPos.row <= 4)// 未过河，只能向前走一步，row加一
            return currPos.row - prevPos.row == 1;
    }

    return true;
}

bool Board::isValidKingMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{
   //检查输入
    if (!isPiece(prevPos, g_king))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKingPos, &Board::isValidKingDelta, &Board::isValidKingRule);
}

bool Board::isValidKnightMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{    
    //检查输入
    if (!isPiece(prevPos, g_knight))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKnightPos, &Board::isValidKnightDelta, &Board::isValidKnightRule);
}

bool Board::isValidBishopMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{   
    //检查输入
    if (!isPiece(prevPos, g_bishop))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidBishopPos, &Board::isValidBishopDelta, &Board::isValidBishopRule);
}

bool Board::isValidAdvisorMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{
    //检查输入
    if (!isPiece(prevPos, g_advisor))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidAdvisorPos, &Board::isValidAdvisorDelta, &Board::isValidAdvisorRule);
}

bool Board::isValidRookMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{ 
    //检查输入
    if (!isPiece(prevPos, g_rook))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidRookPos, &Board::isValidRookDelta, &Board::isValidRookRule);
}

bool Board::isValidCannonMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{   
    //检查输入
    if (!isPiece(prevPos, g_cannon))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidCannonPos, &Board::isValidCannonDelta, &Board::isValidCannonRule);
}

bool Board::isValidPawnMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{
    //检查输入
    if (!isPiece(prevPos, g_pawn))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidPawnPos, &Board::isValidPawnDelta, &Board::isValidPawnRule);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, Board::EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule)
{
    Board::EPlayer prevOwner = getPieceOwner(prevPos);
    Board::EPlayer currOwner = getPieceOwner(currPos);
    // 确认原位置棋子属于应走棋的玩家
    if (player != prevOwner)
        return false;

    // 目的位置与原位置棋子同色，则不能走
    if (currOwner == prevOwner)
        return false;

    // 走法前后的位置要有效
    if (isValidPos != nullptr)
    {
        if (!(this->*isValidPos)(prevPos, prevOwner) || !(this->*isValidPos)(currPos, prevOwner))
            return false;
    }

    // delta是否符合规则
    if (isValidDelta != nullptr)
    {
        if (!(this->*isValidDelta)(currPos - prevPos, prevOwner))
            return false;
    }

    // 额外规则
    if (isValidRule != nullptr)
    {
        //todo
        if (!(this->*isValidRule)(prevPos, currPos))
            return false;
    }

    return true;
}

bool Board::isPiece(TPos pos, int piece)
{
    return piece == (getPiece(pos) & g_pieceMask);
}

bool Board::isKing(TPos pos)
{
    return isPiece(pos, g_king);
}

bool Board::isDefensivePiece(TPos pos)
{
    return isPiece(pos, g_advisor) || isPiece(pos, g_bishop);
}

bool Board::isAttactivePiece(TPos pos)
{
    return isPiece(pos, g_knight) || isPiece(pos, g_rook) || isPiece(pos, g_cannon) || isPiece(pos, g_pawn);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, Board::EPlayer player)
{
    typedef bool (Board::*MoveFunc)(TPos prevPos, TPos currPos, Board::EPlayer player);
    static unordered_map<int, MoveFunc> allMoveFunc =
    {
        {g_king,    &Board::isValidKingMove},
        {g_advisor, &Board::isValidAdvisorMove},
        {g_bishop,  &Board::isValidBishopMove},
        {g_knight,  &Board::isValidKnightMove},
        {g_rook,    &Board::isValidRookMove},
        {g_cannon,  &Board::isValidCannonMove},
        {g_pawn,    &Board::isValidPawnMove},
    };    

    unordered_map<int, MoveFunc>::iterator itr = allMoveFunc.find(getPiece(prevPos) & g_pieceMask);
    if (itr == allMoveFunc.end())
        return false;

    if (!(this->*(itr->second))(prevPos, currPos, player))
        return false;

    return true;
}

byte Board::movePiece(TPos prevPos, TPos currPos)
{
    byte ret = EMR_ok;

    // pos/delta/rule是否合法
    if (!isValidMove(prevPos, currPos, player_))
    {
        ret = EMR_null;
        return ret;
    }

    // 会自杀？
    if (isSuicide(prevPos, currPos, player_))
    {
        ret = EMR_suicide;// 设置SUICIDE位
        return ret;
    }

    updatePieceSet(prevPos, currPos, player_);// 更新玩家棋子位置集合

    changePlayer();//另一玩家走棋

    board_[currPos.row][currPos.col] = board_[prevPos.row][prevPos.col];// 移动原棋子至当前位置
    board_[prevPos.row][prevPos.col] = ResMgr::EP_Empty;// 删除原位置棋子

    // 将军？
    if (check(player_))
    {
        ret = EMR_ok & EMR_check;// 设置OK位/CHECK位

        // 将死？
        if (checkmate(player_))
        {
            ret &= EMR_dead;// 设置DEAD位
        }
    }

    return ret;
}

// 前提是isValidMove(prevPos, currPos, player) && !isSuicide(prevPos, currPos, player)
bool Board::updatePieceSet(TPos prevPos, TPos currPos, Board::EPlayer player)
{
    if (player == Board::EP_None)
        return false;

    Board::TPieceSet& activePieceSet = (player == Board::EP_Up ? upPieceSet_ : downPieceSet_);// 走棋一方
    Board::TPieceSet& passivePieceSet = (player == Board::EP_Up ? downPieceSet_ : upPieceSet_);// 对方

    // 由prevPos更新activePieceSet
    if (isKing(prevPos))
    {
        activePieceSet.king = currPos;// 更新将位置
    }
    else if (isDefensivePiece(prevPos))
    {
        activePieceSet.defenders.erase(prevPos);// 移除原位置
        activePieceSet.defenders.insert(currPos);// 插入新位置
    }
    else if (isAttactivePiece(prevPos))
    {
        activePieceSet.attackers.erase(prevPos);// 移除原位置
        activePieceSet.attackers.insert(currPos);// 插入新位置
    }
    else
    {
        return false;
    }

    // 由currPos更新passivePieceSet
    if (isKing(currPos))
    {
        return false;// 不允许吃将
    }
    else if (isDefensivePiece(currPos))
    {
        passivePieceSet.defenders.erase(currPos);// 如果被吃掉，从列表删除
    }
    else if (isAttactivePiece(currPos))
    {
        passivePieceSet.attackers.erase(currPos);// 如果被吃掉，从列表删除
    }
    else
    {
        return false;
    }

    return true;
}

void Board::rotate()
{
    swap(g_downFlag, g_upFlag);

    //todo: optimize
    vector<vector<byte>> tmp(g_rowNum, vector<byte>(g_colNum, 0));
    for (int i = 0; i < g_rowNum; i++)
    {
        for (int j = 0; j < g_colNum; j++)
        {
            tmp[i][j] = board_[g_rowNum - 1 - i][g_colNum - 1 - j];
        }
    }

    board_.swap(tmp);
    upPieceSet_.swap(downPieceSet_);

    changePlayer();//改变玩家标志
}

//另一玩家走棋
bool Board::changePlayer()
{
    if (player_ == Board::EP_Up)
        player_ = Board::EP_Down;
    else if (player_ == Board::EP_Down)
        player_ = Board::EP_Up;
    else
        return false;

    return true;
}

bool Board::isSuicide(TPos prevPos, TPos currPos, Board::EPlayer player)
{
    struct TAutoSwapPiece
    {
        TAutoSwapPiece(byte& n1, byte& n2)
            : num1(n1)
            , num2(n2)
        {
            tmp = num2;
            num2 = num1;
            num1 = 0;
        }

        ~TAutoSwapPiece()
        {
            num1 = num2;
            num2 = tmp;
        }

    private:
        byte& num1, num2;
        byte tmp;
    };

    TAutoSwapPiece obj(board_[prevPos.row][prevPos.col], board_[currPos.row][currPos.col]);

    return check(player == Board::EP_Up ? Board::EP_Down : Board::EP_Up);//导致自己被将军，则为自杀
}

// 检查player是否将对方
bool Board::check(Board::EPlayer player)
{
    if (player == Board::EP_None)
        return false;

    Board::TPieceSet& activePieceSet = (player == Board::EP_Up ? upPieceSet_ : downPieceSet_);// 将军一方
    Board::TPieceSet& passivePieceSet = (player == Board::EP_Up ? downPieceSet_ : upPieceSet_);// 对方

    TPos kingPos = passivePieceSet.king;
    unordered_set<TPos>& attackers = activePieceSet.attackers;

    for (TPos pos: attackers)
    {
        if (isValidMove(pos, kingPos, player) && !isSuicide(pos, kingPos, player))// 不能自杀
            return true;
    }

    return false;
}

// 检查player是否把对方将死
bool Board::checkmate(Board::EPlayer player)
{
    // 默认对方已处于被将军状态，不再检查
    if (player == Board::EP_None)
        return false;

    Board::EPlayer otherPlayer = (player == Board::EP_Up ? Board::EP_Down : Board::EP_Up);
    Board::TPieceSet& activePieceSet = (player == Board::EP_Up ? upPieceSet_ : downPieceSet_);// 将军一方
    Board::TPieceSet& passivePieceSet = (player == Board::EP_Up ? downPieceSet_ : upPieceSet_);// 对方

    // 检查
    auto handleCheckmate = [this](TPos pos, const unordered_set<TDelta>& deltas, Board::EPlayer player)
    {
        for (TDelta delta: deltas)
        {
            if (this->isValidMove(pos, pos + delta, player) &&
                !this->isSuicide(pos, pos + delta, player))
                return true;
        }

        return false;
    };

    // 尝试被将军方的所有走法，一旦发现应将招法即结束搜索
    // 尝试移动将
    const unordered_set<TDelta>& deltas = getValidKingDelta(otherPlayer);
    if (handleCheckmate(passivePieceSet.king, deltas, otherPlayer))
        return false;

    // 尝试移动防御棋子
    for (TPos pos: passivePieceSet.defenders)
    {
        const unordered_set<TDelta>* pDeltas;
        switch (getPiece(pos) & g_pieceMask)
        {
        case g_advisor:
            pDeltas = &getValidKingDelta(otherPlayer);
            break;
        case g_bishop:
            pDeltas = &getValidBishopDelta(otherPlayer);
            break;
        default:
            break;
        }

        if (handleCheckmate(pos, deltas, otherPlayer))
            return false;
    }

    // 尝试移动进攻棋子
    for (TPos pos: passivePieceSet.attackers)
    {
        const unordered_set<TDelta>* pDeltas;
        switch (getPiece(pos) & g_pieceMask)
        {
        case g_knight:
            pDeltas = &getValidKnightDelta(otherPlayer);
            break;
        case g_rook:
            pDeltas = &getValidRookDelta(otherPlayer);
            break;
        case g_cannon:
            pDeltas = &getValidCannonDelta(otherPlayer);
            break;
        case g_pawn:
            pDeltas = &getValidPawnDelta(otherPlayer);
            break;
        default:
            break;
        }

        if (handleCheckmate(pos, deltas, otherPlayer))
            return false;
    }

    return true;
}
