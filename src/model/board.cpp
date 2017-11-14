#include "board.h"
#include "util/debug.h"

#include <assert.h>
#include <math.h>
#include <algorithm>
#include <unordered_map>

Board::Board()
{

}

void Board::init()
{
    snapshot_ = std::make_shared<Snapshot>();// 新分配快照

    if (!snapshotMemo_)
        snapshotMemo_ = std::make_shared<Memo<Snapshot>>();// 创建快照记录
    snapshotMemo_->clear();// 清空备忘录    
}

int Board::getScore(EPlayer player) const
{
    if (player == def::EP_up)
        return snapshot_->upPieceSet_.score;
    else if (player == def::EP_down)
        return snapshot_->downPieceSet_.score;
    else
        return 0;
}

def::EPiece Board::getPiece(TPos pos) const
{
    if (!co::isValidPos(pos))
        return def::EP_empty;

    return static_cast<def::EPiece>(snapshot_->board_[pos.row][pos.col]);
}

EPlayer Board::getNextPlayer() const
{
    return snapshot_->player_;
}

EPlayer Board::getPieceOwner(TPos pos) const
{
    byte scope = (getPiece(pos) & def::g_scopeMask);
    if (scope == snapshot_->upFlag_)
        return def::EP_up;
    else if (scope == snapshot_->downFlag_)
        return def::EP_down;   
    else
        return def::EP_none;
}

std::pair<TPos, TPos> Board::getTrigger() const
{
    return snapshot_->trigger_;
}

def::TPos Board::calcBestMove(int depth)
{
    vector<std::pair<TPos, TPos>> moves;
    if (!generateAllMoves(moves))
        return def::g_nullPos;

    TPos dstPos = def::g_nullPos;
    int bestScore = INT_MIN;
    for (const std::pair<TPos, TPos>& posPair: moves)
    {
        int score = calcBestScore(depth - 1);
        if (best > score)
        {
            bestScore = score;
            dstPos = posPair.second;
        }
    }
}

int Board::calcBestScore(int depth)
{
    if (depth == 0)
        return getScore(def::getOtherPlayer(getNextPlayer()));

    vector<std::pair<TPos, TPos>> moves;
    if (!generateAllMoves(moves))
        return -1;

    int res = INT_MIN;
    for (const std::pair<TPos, TPos>& posPair: moves)
    {
        int score = calcBestScore(depth - 1);
        if (score > res)
        {
            res = score;
        }
    }

    return res;
}

bool Board::generateAllMoves(vector<std::pair<TPos, TPos>&> moves)
{
    moves.clear();

    for (unsigned int i = 0; i < snapshot_->board_.size(); i++)
    {
        for (unsigned int j = 0; j < snapshot_->board_.front().size(); j++)
        {
            TPos pos = {i, j};
            EPlayer player = getNextPlayer();

            if (getPieceOwner(pos) != player)
                continue;

            const unordered_set<TDelta>* pDeltas = nullptr;

            switch (getPiece(pos) & def::g_pieceMask)
            {
            case def::g_king:
                pDeltas = &getValidKingDelta(player);
                break;
            case def::g_advisor:
                pDeltas = &getValidAdvisorDelta(player);
                break;
            case def::g_bishop:
                pDeltas = &getValidBishopDelta(player);
                break;
            case def::g_knight:
                pDeltas = &getValidKnightDelta(player);
                break;
            case def::g_rook:
                pDeltas = &getValidRookDelta(player);
                break;
            case def::g_cannon:
                pDeltas = &getValidCannonDelta(player);
                break;
            case def::g_pawn:
                pDeltas = &getValidPawnDelta(player);
                break;
            default:
                pDeltas = nullptr;
                break;
            }

            if (pDeltas == nullptr)
            {
                moves.clear();
                return false;
            }

            for (const TDelta& delta: *pDeltas)
            {
                if (co::isValidPos(pos + delta))
                    moves.push_back(pos, pos + delta);
            }
        }
    }

    return true;
}

bool Board::isValidKingPos(TPos pos, EPlayer player) const
{
    static const unordered_set<TPos> upPos =
    {
        {0, 3}, {0, 4}, {0, 5},
        {1, 3}, {1, 4}, {1, 5},
        {2, 3}, {2, 4}, {2, 5}
    };

    static const unordered_set<TPos> downPos =
    {
        {7, 3}, {7, 4}, {7, 5},
        {8, 3}, {8, 4}, {8, 5},
        {9, 3}, {9, 4}, {9, 5}
    };

    return player == def::EP_up ?
           upPos.find(pos) != upPos.end() :
           downPos.find(pos) != downPos.end();
}

bool Board::isValidAdvisorPos(TPos pos, EPlayer player) const
{
    static const unordered_set<TPos> upPos =
    {
        {0, 3}, {0, 5},
        {1, 4},
        {2, 3}, {2, 5}
    };

    static const unordered_set<TPos> downPos =
    {
        {7, 3}, {7, 5},
        {8, 4},
        {9, 3}, {9, 5}
    };

    return player == def::EP_up ?
           upPos.find(pos) != upPos.end() :
           downPos.find(pos) != downPos.end();
}

bool Board::isValidBishopPos(TPos pos, EPlayer player) const
{
    static const unordered_set<TPos> upPos =
    {
        {0, 2}, {0, 6},
        {2, 0}, {2, 4}, {2, 8},
        {4, 2}, {4, 6}
    };

    static const unordered_set<TPos> downPos =
    {
        {5, 2}, {5, 6},
        {7, 0}, {7, 4}, {7, 8},
        {9, 2}, {9, 6}
    };    

    return player == def::EP_up ?
           upPos.find(pos) != upPos.end() :
           downPos.find(pos) != downPos.end();
}

bool Board::isValidKnightPos(TPos pos, EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidRookPos(TPos pos, EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidCannonPos(TPos pos, EPlayer player) const
{
    return co::isValidPos(pos);
}

bool Board::isValidPawnPos(TPos pos, EPlayer player) const
{
    if (player == def::EP_up)
    {
        return ((pos.row == 3 || pos.row == 4) && (pos.col % 2 == 0)) ||
               (pos.row >= 5 && pos.row <= 9 && pos.col >= 0 && pos.col <= 8);
    }
    else
    {
        return ((pos.row == 5 || pos.row == 6) && (pos.col % 2 == 0)) ||
                (pos.row >= 0 && pos.row <= 4 && pos.col >= 0 && pos.col <= 8);
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
    static const unordered_set<TDelta> upDeltas =
    {
        { 1,  0},
        { 0, -1},
        { 0,  1},
    };

    static const unordered_set<TDelta> downDeltas =
    {
        {-1,  0},
        { 0, -1},
        { 0,  1},
    };    

    return player == def::EP_up ? upDeltas : downDeltas;
}

// 单格
bool Board::isValidKingDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidKingDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 单格对角线
bool Board::isValidAdvisorDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidAdvisorDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 田字
bool Board::isValidBishopDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidBishopDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 日字
bool Board::isValidKnightDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidKnightDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidRookDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidRookDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 共线
bool Board::isValidCannonDelta(TDelta delta, EPlayer player) const
{
    const unordered_set<TDelta>& deltas = getValidCannonDelta(player);
    return deltas.find(delta) != deltas.end();
}

// 前、左、右
bool Board::isValidPawnDelta(TDelta delta, EPlayer player) const
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
    return getPiece(buddyPos) == def::EP_empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidKnightRule(TPos prevPos, TPos currPos) const
{    
    // 计算马腿位置
    TPos buddyPos = def::g_nullPos;
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
    return getPiece(buddyPos) == def::EP_empty;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidRookRule(TPos prevPos, TPos currPos) const
{
    // 中间不能有棋子
    if (prevPos.row == currPos.row)// 同一行
    {
        int cur = std::min(prevPos.col, currPos.col) + 1;
        int end = std::max(prevPos.col, currPos.col) - 1;
        while (cur <= end)
        {
            if (def::EP_empty != getPiece({prevPos.row, cur}))
                return false;

            cur++;
        }
    }
    else// 同一列
    {
        int cur = std::min(prevPos.row, currPos.row) + 1;
        int end = std::max(prevPos.row, currPos.row) - 1;
        while (cur <= end)
        {
            if (def::EP_empty != getPiece({cur, prevPos.col}))
                return false;

            cur++;
        }
    }

    return true;
}

// 不再检查pos及delta，默认前面已检查
bool Board::isValidCannonRule(TPos prevPos, TPos currPos) const
{ 
    if (getPiece(currPos) == def::EP_empty)// 目的位置为空，则中间不能有棋子
    {
        if (prevPos.row == currPos.row)// 同一行
        {
            int cur = std::min(prevPos.col, currPos.col) + 1;
            int end = std::max(prevPos.col, currPos.col) - 1;
            while (cur <= end)
            {
                if (def::EP_empty != getPiece({prevPos.row, cur}))
                    return false;

                cur++;
            }

            return true;
        }
        else// 同一列
        {
            int cur = std::min(prevPos.row, currPos.row) + 1;
            int end = std::max(prevPos.row, currPos.row) - 1;
            while (cur <= end)
            {
                if (def::EP_empty != getPiece({cur, prevPos.col}))
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
            int cur = std::min(prevPos.col, currPos.col) + 1;
            int end = std::max(prevPos.col, currPos.col) - 1;
            while (cur <= end)
            {
                if (def::EP_empty != getPiece({prevPos.row, cur}))
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
            int cur = std::min(prevPos.row, currPos.row) + 1;
            int end = std::max(prevPos.row, currPos.row) - 1;
            while (cur <= end)
            {
                if (def::EP_empty != getPiece({cur, prevPos.col}))
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

bool Board::isValidKingMove(TPos prevPos, TPos currPos, EPlayer player) const
{
   //检查输入
    if (!isPiece(prevPos, def::g_king))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKingPos, &Board::isValidKingDelta, &Board::isValidKingRule);
}

bool Board::isValidKnightMove(TPos prevPos, TPos currPos, EPlayer player) const
{    
    //检查输入
    if (!isPiece(prevPos, def::g_knight))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidKnightPos, &Board::isValidKnightDelta, &Board::isValidKnightRule);
}

bool Board::isValidBishopMove(TPos prevPos, TPos currPos, EPlayer player) const
{   
    //检查输入
    if (!isPiece(prevPos, def::g_bishop))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidBishopPos, &Board::isValidBishopDelta, &Board::isValidBishopRule);
}

bool Board::isValidAdvisorMove(TPos prevPos, TPos currPos, EPlayer player) const
{
    //检查输入
    if (!isPiece(prevPos, def::g_advisor))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidAdvisorPos, &Board::isValidAdvisorDelta, &Board::isValidAdvisorRule);
}

bool Board::isValidRookMove(TPos prevPos, TPos currPos, EPlayer player) const
{ 
    //检查输入
    if (!isPiece(prevPos, def::g_rook))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidRookPos, &Board::isValidRookDelta, &Board::isValidRookRule);
}

bool Board::isValidCannonMove(TPos prevPos, TPos currPos, EPlayer player) const
{   
    //检查输入
    if (!isPiece(prevPos, def::g_cannon))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidCannonPos, &Board::isValidCannonDelta, &Board::isValidCannonRule);
}

bool Board::isValidPawnMove(TPos prevPos, TPos currPos, EPlayer player) const
{
    //检查输入
    if (!isPiece(prevPos, def::g_pawn))
        return false;

    return isValidMove(prevPos, currPos, player, &Board::isValidPawnPos, &Board::isValidPawnDelta, &Board::isValidPawnRule);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule) const
{
    EPlayer prevOwner = getPieceOwner(prevPos);
    EPlayer currOwner = getPieceOwner(currPos);
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
    return piece == (getPiece(pos) & def::g_pieceMask);
}

bool Board::isKing(TPos pos) const
{
    return isPiece(pos, def::g_king);
}

bool Board::isDefensivePiece(TPos pos) const
{
    return isPiece(pos, def::g_advisor) || isPiece(pos, def::g_bishop);
}

bool Board::isAttactivePiece(TPos pos) const
{
    return isPiece(pos, def::g_knight) || isPiece(pos, def::g_rook) || isPiece(pos, def::g_cannon) || isPiece(pos, def::g_pawn);
}

bool Board::isValidMove(TPos prevPos, TPos currPos, EPlayer player) const
{
    typedef bool (Board::*MoveFunc)(TPos prevPos, TPos currPos, EPlayer player) const;
    static std::unordered_map<int, MoveFunc> allMoveFunc =
    {
        {def::g_king,    &Board::isValidKingMove},
        {def::g_advisor, &Board::isValidAdvisorMove},
        {def::g_bishop,  &Board::isValidBishopMove},
        {def::g_knight,  &Board::isValidKnightMove},
        {def::g_rook,    &Board::isValidRookMove},
        {def::g_cannon,  &Board::isValidCannonMove},
        {def::g_pawn,    &Board::isValidPawnMove},
    };    

    std::unordered_map<int, MoveFunc>::iterator itr = allMoveFunc.find(getPiece(prevPos) & def::g_pieceMask);
    if (itr == allMoveFunc.end())
        return false;

    if (!(this->*(itr->second))(prevPos, currPos, player))
        return false;

    return true;
}

byte Board::movePiece(TPos prevPos, TPos currPos)
{
    byte ret = model::EMR_null;

    // pos/delta/rule是否合法
    if (!isValidMove(prevPos, currPos, snapshot_->player_))
    {
        return ret;
    }

    // 自杀？
    if (isSuicide(prevPos, currPos, snapshot_->player_))
    {
        ret |= model::EMR_suicide;// 设置SUICIDE位
        return ret;
    }

    ret = model::EMR_ok;// 可以走棋

    if (getPiece(currPos) != def::EP_empty)
        ret |= model::EMR_eat;// 吃子

    saveSnapshot();// 保存快照
    updateSnapshot(prevPos, currPos, snapshot_->player_);// 更新快照    

    // 将军？
    EPlayer player = def::getOtherPlayer(snapshot_->player_);// player已切换，需要切回来
    if (check(player))
    {
        ret |= model::EMR_check;// 设置OK位/CHECK位

        // 将死？
        if (checkmate(player))
        {
            ret |= model::EMR_dead;// 设置DEAD位
        }
    }

    debug::printBoard(snapshot_->board_);
    return ret;
}

// 前提是isValidMove(prevPos, currPos, player) && !isSuicide(prevPos, currPos, player)
bool Board::updateSnapshot(TPos prevPos, TPos currPos, EPlayer player)
{
    Board::TPieceSet& activePieceSet = (player == def::EP_up ? snapshot_->upPieceSet_ : snapshot_->downPieceSet_);// 走棋一方
    Board::TPieceSet& passivePieceSet = (player == def::EP_up ? snapshot_->downPieceSet_ : snapshot_->upPieceSet_);// 对方

    // 更新双方分数
    byte prevPiece = (getPiece(prevPos) & def::g_pieceMask);
    byte currPiece = (getPiece(currPos) & def::g_pieceMask);

    activePieceSet.score += getValue(prevPiece, currPos, player) - getValue(prevPiece, prevPos, player);
    passivePieceSet.score -= getValue(currPiece, currPos, def::getOtherPlayer(player));

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
    snapshot_->board_[prevPos.row][prevPos.col] = def::EP_empty;// 删除原位置棋子

    def::switchPlayer(snapshot_->player_);// 切换玩家

    snapshot_->trigger_.first = prevPos;// 设置当前快照的两个触发棋子位置，用于绘制select图标
    snapshot_->trigger_.second = currPos;

    return true;
}

bool Board::rotate()
{
    snapshot_->rotate();
    return true;
}

bool Board::undo()
{    
    if (snapshotMemo_->empty())// 已经是最初局面了
        return false;

    loadSnapshot(snapshotMemo_->top());// 读取上一个快照
    snapshotMemo_->pop();
    debug::printBoard(snapshot_->board_);
    return true;
}

bool Board::isSuicide(TPos prevPos, TPos currPos, EPlayer player)
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
bool Board::check(EPlayer player)
{
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
bool Board::checkmate(EPlayer player)
{
    // 默认对方已处于被将军状态，不再检查
    EPlayer otherPlayer = def::getOtherPlayer(player);
    Board::TPieceSet& activePieceSet = (player == def::EP_up ? snapshot_->upPieceSet_ : snapshot_->downPieceSet_);// 将军一方
    Board::TPieceSet& passivePieceSet = (player == def::EP_up ? snapshot_->downPieceSet_ : snapshot_->upPieceSet_);// 对方

    // 检查
    auto handleCheckmate = [this](TPos pos, const unordered_set<TDelta>& deltas, EPlayer player)
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
        switch (getPiece(pos) & def::g_pieceMask)
        {
        case def::g_advisor:
            pDeltas = &getValidAdvisorDelta(otherPlayer);
            break;
        case def::g_bishop:
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
        switch (getPiece(pos) & def::g_pieceMask)
        {
        case def::g_knight:
            pDeltas = &getValidKnightDelta(otherPlayer);
            break;
        case def::g_rook:
            pDeltas = &getValidRookDelta(otherPlayer);
            break;
        case def::g_cannon:
            pDeltas = &getValidCannonDelta(otherPlayer);
            break;
        case def::g_pawn:
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

    int start = std::min(snapshot_->upPieceSet_.king.row, snapshot_->downPieceSet_.king.row) + 1;
    int end = std::max(snapshot_->upPieceSet_.king.row, snapshot_->downPieceSet_.king.row) - 1;
    while (start <= end)
    {
        if (snapshot_->board_[start][snapshot_->upPieceSet_.king.col] != def::EP_empty)
            return false;

        start++;
    }

    return true;
}

// 创建与现有快照相同的另一快照
shared_ptr<Board::Snapshot> Board::createSnapshot()
{
    shared_ptr<Snapshot> newSnapshot = std::make_shared<Snapshot>();
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

int Board::getValue(byte piece, TPos pos, EPlayer player) const
{
    static const int downValueMap[7][10][9] =
    {
        {// king
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  1,  1,  1,  0,  0,  0},
            {0,  0,  0,  2,  2,  2,  0,  0,  0},
            {0,  0,  0, 11, 15, 11,  0,  0,  0},
        },
        {// advisor
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0,  0,  0,  0,  0,  0,  0},
            {0,  0,  0, 20,  0, 20,  0,  0,  0},
            {0,  0,  0,  0, 23,  0,  0,  0,  0},
            {0,  0,  0, 20,  0, 20,  0,  0,  0},
        },
        {// bishop
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0, 20,  0,  0,  0, 20,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            {18,  0,  0,  0, 23,  0,  0,  0, 18},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0, 20,  0,  0,  0, 20,  0,  0},
        },
        {// knight
            {90, 90, 90, 96, 90, 96, 90, 90, 90},
            {90, 96,103, 97, 94, 97,103, 96, 90},
            {92, 98, 99,103, 99,103, 99, 98, 92},
            {93,108,100,107,100,107,100,108, 93},
            {90,100, 99,103,104,103, 99,100, 90},
            {90, 98,101,102,103,102,101, 98, 90},
            {92, 94, 98, 95, 98, 95, 98, 94, 92},
            {93, 92, 94, 95, 92, 95, 94, 92, 93},
            {85, 90, 92, 93, 78, 93, 92, 90, 85},
            {88, 85, 90, 88, 90, 88, 90, 85, 88},
        },
        {// rook
            {206,208,207,213,214,213,207,208,206},
            {206,212,209,216,233,216,209,212,206},
            {206,208,207,214,216,214,207,208,206},
            {206,213,213,216,216,216,213,213,206},
            {208,211,211,214,215,214,211,211,208},
            {208,212,212,214,215,214,212,212,208},
            {204,209,204,212,214,212,204,209,204},
            {198,208,204,212,212,212,204,208,198},
            {200,208,206,212,200,212,206,208,200},
            {194,206,204,212,200,212,204,206,194},
        },
        {// cannon
            {100,100, 96, 91, 90, 91, 96,100,100},
            { 98, 98, 96, 92, 89, 92, 96, 98, 98},
            { 97, 97, 96, 91, 92, 91, 96, 97, 97},
            { 96, 99, 99, 98,100, 98, 99, 99, 96},
            { 96, 96, 96, 96,100, 96, 96, 96, 96},
            { 95, 96, 99, 96,100, 96, 99, 96, 95},
            { 96, 96, 96, 96, 96, 96, 96, 96, 96},
            { 97, 96,100, 99,101, 99,100, 96, 97},
            { 96, 97, 98, 98, 98, 98, 98, 97, 96},
            { 96, 96, 97, 99, 99, 99, 97, 96, 96},
        },
        {// pawn
            { 9,  9,  9, 11, 13, 11,  9,  9,  9},
            {19, 24, 34, 42, 44, 42, 34, 24, 19},
            {19, 24, 32, 37, 37, 37, 32, 24, 19},
            {19, 23, 27, 29, 30, 29, 27, 23, 19},
            {14, 18, 20, 27, 29, 27, 20, 18, 14},
            { 7,  0, 13,  0, 16,  0, 13,  0,  7},
            { 7,  0,  7,  0, 15,  0,  7,  0,  7},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
            { 0,  0,  0,  0,  0,  0,  0,  0,  0},
        }
    };

    if (piece == def::g_empty || player == def::EP_none || !co::isValidPos(pos))
        return 0;

    if (player == def::EP_up)// 转换坐标
        pos = co::getRotatePos(pos);

    return downValueMap[piece - 1][pos.row][pos.col];// piece需要减一
}

