#include "board.h"
#include "../co.h"
#include "../debug.h"

#include <assert.h>
#include <math.h>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include <unordered_map>


using namespace def;
using namespace co;
using namespace std;

Board::Board()
{
    snapshotMemo_ = std::make_shared<Memo<Snapshot>>();// 创建快照记录
    init();
}

void Board::init()
{
    snapshot_ = std::make_shared<Snapshot>();// 新分配快照
    snapshotMemo_->clear();// 清空备忘录
    snapshotMemo_->push(std::make_shared<Snapshot>());// 插入起始快照，不能是snapshot_！
}

ResMgr::EPiece Board::getPiece(TPos pos) const
{
    if (!co::isValidPos(pos))
        return ResMgr::EP_empty;

    return static_cast<ResMgr::EPiece>(snapshot_->board_[pos.row][pos.col]);
}

def::EPlayer Board::getNextPlayer() const
{
    return snapshot_->player_;
}

def::EPlayer Board::getPieceOwner(TPos pos) const
{
    byte scope = (getPiece(pos) & def::g_scopeMask);
    if (scope == snapshot_->upFlag_)
        return def::EP_up;
    else if (scope == snapshot_->downFlag_)
        return def::EP_down;
    else
        return def::EP_none;
}

bool Board::isValidKingPos(TPos pos, def::EPlayer player) const
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

    if (player == def::EP_down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == def::EP_up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidAdvisorPos(TPos pos, def::EPlayer player) const
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

    if (player == def::EP_down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == def::EP_up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidBishopPos(TPos pos, def::EPlayer player) const
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
        {4, 2}, {4, 6}
    };

    if (player == def::EP_down)
    {
        return downPos.find(pos) != downPos.end();
    }
    else if (player == def::EP_up)
    {
        return upPos.find(pos) != upPos.end();
    }
    else
    {
        return false;
    }
}

bool Board::isValidKnightPos(TPos pos, def::EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidRookPos(TPos pos, def::EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidCannonPos(TPos pos, def::EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidPawnPos(TPos pos, def::EPlayer player) const
{
    if (player == def::EP_down)
    {
        return ((pos.row == 5 || pos.row == 6) && (pos.col % 2 == 0)) ||
               (pos.row >= 0 && pos.row <= 4 && pos.col >= 0 && pos.col <= 8);
    }
    else if (player == def::EP_up)
    {
        return ((pos.row == 3 || pos.row == 4) && (pos.col % 2 == 0)) ||
               (pos.row >= 5 && pos.row <= 9 && pos.col >= 0 && pos.col <= 8);
    }
    else
    {
        return false;
    }
}

const unordered_set<TDelta>& Board::getValidKingDelta(def::EPlayer player) const
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

const unordered_set<TDelta>& Board::getValidAdvisorDelta(def::EPlayer player) const
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

const unordered_set<TDelta>& Board::getValidBishopDelta(def::EPlayer player) const
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
const unordered_set<TDelta>& Board::getValidKnightDelta(def::EPlayer player) const
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
const unordered_set<TDelta>& Board::getValidRookDelta(def::EPlayer player) const
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
const unordered_set<TDelta>& Board::getValidCannonDelta(def::EPlayer player) const
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
const unordered_set<TDelta>& Board::getValidPawnDelta(def::EPlayer player) const
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

    return player == def::EP_down ? downDeltas : upDeltas;
}

// 单格
bool Board::isValidKingDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidKingDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 单格对角线
bool Board::isValidAdvisorDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidAdvisorDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 田字
bool Board::isValidBishopDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidBishopDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 日字
bool Board::isValidKnightDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidKnightDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidRookDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidRookDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidCannonDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidCannonDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 前、左、右
bool Board::isValidPawnDelta(TDelta delta, def::EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidPawnDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidKingRule(TPos prevPos, TPos currPos) const
{    
    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidAdvisorRule(TPos prevPos, TPos currPos) const
{ 
    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidBishopRule(TPos prevPos, TPos currPos) const
{
    // 计算象眼位置
    TPos buddyPos = {(prevPos.row + currPos.row) / 2, (prevPos.col + currPos.col) / 2};

    // 不能塞象眼
    return getPiece(buddyPos) == ResMgr::EP_empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidKnightRule(TPos prevPos, TPos currPos) const
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
    return getPiece(buddyPos) == ResMgr::EP_empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidRookRule(TPos prevPos, TPos currPos) const
{
    // 中间不能有棋子
    if (prevPos.row == currPos.row)// 同一行
    {
        int cur = min(prevPos.col, currPos.col) + 1;
        int end = max(prevPos.col, currPos.col) - 1;
        while (cur <= end)
        {
            if (ResMgr::EP_empty != getPiece({prevPos.row, cur}))
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
            if (ResMgr::EP_empty != getPiece({cur, prevPos.col}))
                return false;

            cur++;
        }
    }

    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidCannonRule(TPos prevPos, TPos currPos) const
{ 
    if (getPiece(currPos) == ResMgr::EP_empty)// 目的位置为空，则中间不能有棋子
    {
        if (prevPos.row == currPos.row)// 同一行
        {
            int cur = min(prevPos.col, currPos.col) + 1;
            int end = max(prevPos.col, currPos.col) - 1;
            while (cur <= end)
            {
                if (ResMgr::EP_empty != getPiece({prevPos.row, cur}))
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
                if (ResMgr::EP_empty != getPiece({cur, prevPos.col}))
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
                if (ResMgr::EP_empty != getPiece({prevPos.row, cur}))
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
                if (ResMgr::EP_empty != getPiece({cur, prevPos.col}))
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
bool Board::isValidPawnRule(TPos prevPos, TPos currPos) const
{
    byte scope = (getPiece(prevPos) & def::g_scopeMask);
    if (scope == snapshot_->upFlag_)
    {
        if (prevPos.row <= 4)// 未过河，只能向前走一步，row加一
            return currPos.row - prevPos.row == 1;
    }
    else if (scope == snapshot_->downFlag_)
    {
        if (prevPos.row >= 5)// 未过河，只能向前走一步，row减一
            return currPos.row - prevPos.row == -1;
    }

    return true;
}

bool Board::isValidKingMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{
   //检查输入
    if (!isPiece(prevPos, g_king))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKingPos, &Board::isValidKingDelta, &Board::isValidKingRule);
}

bool Board::isValidKnightMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{    
    //检查输入
    if (!isPiece(prevPos, g_knight))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKnightPos, &Board::isValidKnightDelta, &Board::isValidKnightRule);
}

bool Board::isValidBishopMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{   
    //检查输入
    if (!isPiece(prevPos, g_bishop))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidBishopPos, &Board::isValidBishopDelta, &Board::isValidBishopRule);
}

bool Board::isValidAdvisorMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{
    //检查输入
    if (!isPiece(prevPos, g_advisor))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidAdvisorPos, &Board::isValidAdvisorDelta, &Board::isValidAdvisorRule);
}

bool Board::isValidRookMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{ 
    //检查输入
    if (!isPiece(prevPos, g_rook))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidRookPos, &Board::isValidRookDelta, &Board::isValidRookRule);
}

bool Board::isValidCannonMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{   
    //检查输入
    if (!isPiece(prevPos, g_cannon))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidCannonPos, &Board::isValidCannonDelta, &Board::isValidCannonRule);
}

bool Board::isValidPawnMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{
    //检查输入
    if (!isPiece(prevPos, g_pawn))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidPawnPos, &Board::isValidPawnDelta, &Board::isValidPawnRule);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, def::EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule) const
{
    def::EPlayer prevOwner = getPieceOwner(prevPos);
    def::EPlayer currOwner = getPieceOwner(currPos);
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
        if (!(this->*isValidRule)(prevPos, currPos))
            return false;
    }

    return true;
}

bool Board::isPiece(TPos pos, int piece) const
{
    return piece == (getPiece(pos) & g_pieceMask);
}

bool Board::isKing(TPos pos) const
{
    return isPiece(pos, g_king);
}

bool Board::isDefensivePiece(TPos pos) const
{
    return isPiece(pos, g_advisor) || isPiece(pos, g_bishop);
}

bool Board::isAttactivePiece(TPos pos) const
{
    return isPiece(pos, g_knight) || isPiece(pos, g_rook) || isPiece(pos, g_cannon) || isPiece(pos, g_pawn);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, def::EPlayer player) const
{
    typedef bool (Board::*MoveFunc)(TPos prevPos, TPos currPos, def::EPlayer player) const;
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
    if (!isValidMove(prevPos, currPos, snapshot_->player_))
    {
        ret = EMR_null;
        return ret;
    }

    // 自杀？
    if (isSuicide(prevPos, currPos, snapshot_->player_))
    {
        ret = EMR_suicide;// 设置SUICIDE位
        return ret;
    }    

    updateSnapshot(prevPos, currPos, snapshot_->player_);// 更新快照
    saveSnapshot();// 保存快照

    // 将军？
    def::EPlayer player = def::getOtherPlayer(snapshot_->player_);// player已切换，需要切回来
    if (check(player))
    {
        ret |= EMR_check;// 设置OK位/CHECK位

        // 将死？
        if (checkmate(player))
        {
            ret |= EMR_dead;// 设置DEAD位
        }
    }

    debug::printBoard(snapshot_->board_);
    return ret;
}

// 前提是isValidMove(prevPos, currPos, player) && !isSuicide(prevPos, currPos, player)
bool Board::updateSnapshot(TPos prevPos, TPos currPos, def::EPlayer player)
{
    if (player == def::EP_none)
        return false;

    Board::TPieceSet& activePieceSet = (player == def::EP_up ? snapshot_->upPieceSet_ : snapshot_->downPieceSet_);// 走棋一方
    Board::TPieceSet& passivePieceSet = (player == def::EP_up ? snapshot_->downPieceSet_ : snapshot_->upPieceSet_);// 对方

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

    snapshot_->board_[currPos.row][currPos.col] = snapshot_->board_[prevPos.row][prevPos.col];// 移动原棋子至当前位置
    snapshot_->board_[prevPos.row][prevPos.col] = ResMgr::EP_empty;// 删除原位置棋子

    def::switchPlayer(snapshot_->player_);// 切换玩家

    return true;
}

bool Board::rotate()
{
    snapshot_->rotate();
    return true;
}

bool Board::undo()
{
    if (snapshotMemo_->empty())
        return false;

    loadSnapshot(snapshotMemo_->top());
    snapshotMemo_->pop();
    debug::printBoard(snapshot_->board_);
    return true;
}

bool Board::isSuicide(TPos prevPos, TPos currPos, def::EPlayer player)
{
    // 不能越界
    if (!co::isValidPos(prevPos) || !co::isValidPos(currPos))
        return true;

    saveSnapshot();// 先保存快照
    updateSnapshot(prevPos, currPos, snapshot_->player_);// 不管结果，先让走棋，更新快照
    bool ret = check(snapshot_->player_) || isKingMeeting();// 如果上步走棋导致自己被将军，则为自杀；主动双王碰面也算自杀；此时player已经切换
    loadSnapshot(snapshotMemo_->top());// 将原先快照还原回来
    snapshotMemo_->pop();// 弹出memo最后的快照

    return ret;
}

// 检查player是否将对方
bool Board::check(def::EPlayer player)
{
    if (player == def::EP_none)
        return false;

    Board::TPieceSet& activePieceSet = (player == def::EP_up ? snapshot_->upPieceSet_ : snapshot_->downPieceSet_);// 将军一方
    Board::TPieceSet& passivePieceSet = (player == def::EP_up ? snapshot_->downPieceSet_ : snapshot_->upPieceSet_);// 对方

    TPos kingPos = passivePieceSet.king;
    unordered_set<TPos>& attackers = activePieceSet.attackers;

    for (TPos pos: attackers)
    {
        if (isValidMove(pos, kingPos, player)/* && !isSuicide(pos, kingPos, player)*/)// 这里不需要判断自杀！否则会递归出错
            return true;
    }

    return false;
}

// 检查player是否把对方将死
bool Board::checkmate(def::EPlayer player)
{
    // 默认对方已处于被将军状态，不再检查
    if (player == def::EP_none)
        return false;

    def::EPlayer otherPlayer = def::getOtherPlayer(player);
    Board::TPieceSet& activePieceSet = (player == def::EP_up ? snapshot_->upPieceSet_ : snapshot_->downPieceSet_);// 将军一方
    Board::TPieceSet& passivePieceSet = (player == def::EP_up ? snapshot_->downPieceSet_ : snapshot_->upPieceSet_);// 对方

    // 检查
    auto handleCheckmate = [this](TPos pos, const unordered_set<TDelta>& deltas, def::EPlayer player)
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
    const unordered_set<TDelta>* pDeltas = &getValidKingDelta(otherPlayer);
    if (handleCheckmate(passivePieceSet.king, *pDeltas, otherPlayer))
        return false;

    // 尝试移动防御棋子
    for (TPos pos: passivePieceSet.defenders)
    {
        switch (getPiece(pos) & g_pieceMask)
        {
        case g_advisor:
            pDeltas = &getValidAdvisorDelta(otherPlayer);
            break;
        case g_bishop:
            pDeltas = &getValidBishopDelta(otherPlayer);
            break;
        default:
            break;
        }

        if (handleCheckmate(pos, *pDeltas, otherPlayer))
            return false;
    }

    // 尝试移动进攻棋子
    for (TPos pos: passivePieceSet.attackers)
    {
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

        if (handleCheckmate(pos, *pDeltas, otherPlayer))
            return false;
    }

    return true;
}

bool Board::isKingMeeting() const
{
    if (snapshot_->upPieceSet_.king.col != snapshot_->downPieceSet_.king.col)
        return false;

    int start = min(snapshot_->upPieceSet_.king.row, snapshot_->downPieceSet_.king.row) + 1;
    int end = max(snapshot_->upPieceSet_.king.row, snapshot_->downPieceSet_.king.row) - 1;
    while (start <= end)
    {
        if (snapshot_->board_[start][snapshot_->upPieceSet_.king.col] != ResMgr::EP_empty)
            return false;

        start++;
    }

    return true;
}

// 创建与现有快照相同的另一快照
shared_ptr<Board::Snapshot> Board::createSnapshot()
{
    shared_ptr<Snapshot> newSnapshot = make_shared<Snapshot>();
    *newSnapshot.get() = *snapshot_.get();
    return newSnapshot;
}

bool Board::saveSnapshot()
{
    snapshotMemo_->push(snapshot_);// 保存原快照
    snapshot_ = createSnapshot();// 使用新快照
    //return snapshot_;
    return snapshot_.get() != nullptr;
}

bool Board::loadSnapshot(shared_ptr<Snapshot> snapshot)
{
    snapshot_ = snapshot;
    //return snapshot_;
    return snapshot_.get() != nullptr;
}
