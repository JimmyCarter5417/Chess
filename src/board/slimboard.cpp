#include <algorithm>
#include <assert.h>
#include <memory.h>
#include <time.h>

#include "slimboard.h"

using namespace std;

static const int8_t g_deltaKing[4]      = {-16,  -1,  1, 16};
static const int8_t g_deltaAdvisor[4]   = {-17, -15, 15, 17};
static const int8_t g_deltaKnight[4][2] = {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}};// 马的正常delta

static const int g_scoreCheckmate  = 10000;// 将死对方的分数
static const int g_scoreWin        = 9900; // 分数大于此界限均为胜利
static const int g_scoreDraw       = 20;
static const int g_maxDepth        = 32;   // 最大递归深度

static const Zobrist g_zoPlayer;
static const Zobrist g_zoTable[14][256];


SlimBoard::SlimBoard()
{

}

void SlimBoard::init()// 开局
{
    static const uint8_t initBoard[256] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 21, 20, 19, 18, 17, 18, 19, 20, 21,  0,  0,  0,  0,// 起点车51
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0, 22,  0,  0,  0,  0,  0, 22,  0,  0,  0,  0,  0,
      0,  0,  0, 23,  0, 23,  0, 23,  0, 23,  0, 23,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 15,  0, 15,  0, 15,  0, 15,  0, 15,  0,  0,  0,  0,
      0,  0,  0,  0, 14,  0,  0,  0,  0,  0, 14,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 13, 12, 11, 10,  9, 10, 11, 12, 13,  0,  0,  0,  0,// 终点车203
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    
    // 初始化棋盘
    memcpy(board_, initBoard, sizeof(board_));
    memset(cache_, 0, sizeof(cache_));

    distance_ = 0;

    // 双王起始位置
    redKingIdx_ = 199;
    blackKingIdx_ = 55;
    // 计算双方起始分数
    initScore();

    winner_ = def::PLAYER_none;
    player_ = def::PLAYER_red;
    // 清空历史记录
    records_.clear();

    zoCurr_.clear();
}

// 计算双方起始分数
void SlimBoard::initScore()
{
    blackScore_ = 0;
    redScore_ = 0;

    for (int i = 0; i < 256; i++)
    {
        def::ICON_E   icon  = getIcon(i);
        def::PLAYER_E owner = getOwner(i);

        if (owner == def::PLAYER_black)
            blackScore_ += getValue(icon, i);
        else if (owner == def::PLAYER_red)
            redScore_ += getValue(icon, i);
    }
}

bool SlimBoard::undoMakeMove()// 悔棋
{
    if (records_.empty())
    {
        return false;
    }

    SlimBoard::TRecord record = records_.top();
    records_.pop();
    undoMovePiece(record.move, record.capture);
    
    def::switchPlayer(player_);

    distance_--;// 减少与根节点的距离

    return true;
}

uint8_t SlimBoard::autoMove()// 电脑计算走棋
{
    /*int depth = 3;

    uint16_t nextMove1;
    int a = minimax(depth, player_, &nextMove1);

    uint16_t nextMove2;
    int b = negamax(depth, &nextMove2);

    uint16_t nextMove3;
    int c = alphabeta(depth, player_, INT_MIN, INT_MAX, &nextMove3);

    uint16_t nextMove4;
    int d = alphabetaWithNega(depth, -g_scoreCheckmate, g_scoreCheckmate, &nextMove4);*/

    uint16_t move = fullSearch();

    return makeMove(move);
}

int SlimBoard::evaluate(def::PLAYER_E player) const
{
    return (player == def::PLAYER_red) ? (redScore_ - blackScore_) : (blackScore_ - redScore_);
}

int SlimBoard::minimax(int depth, def::PLAYER_E maxPlayer, uint16_t* pNextMove)
{
    if (depth == 0 || winner_ != def::PLAYER_none)      
        return evaluate(maxPlayer);// 评价函数是相对于极大方的

    vector<uint16_t> moves;
    generateAllMoves(moves);

    if (player_ == maxPlayer)// 极大节点
    {
        int maxScore = INT_MIN;

        for (uint16_t move: moves)
        {
            if (board::MOVE_RET_ok & makeMove(move))// move可能导致自杀
            {
                int val = minimax(depth - 1, maxPlayer, nullptr);
                undoMakeMove();

                if (val > maxScore)
                {
                    maxScore = val;

                    if (pNextMove != nullptr)
                        *pNextMove = move;
                }
            }
        }

        return maxScore;
    }
    else// 极小节点
    {
        int minScore = INT_MAX;

        for (uint16_t move: moves)
        {
            if (board::MOVE_RET_ok & makeMove(move))// move可能导致自杀
            {
                int val = minimax(depth - 1, maxPlayer, nullptr);
                undoMakeMove();

                if (val < minScore)
                {
                    minScore = val;

                    if (pNextMove != nullptr)
                        *pNextMove = move;
                }
            }
        }

        return minScore;
    }
}

int SlimBoard::negamax(int depth, uint16_t* pNextMove)
{
    if (depth == 0 || winner_ != def::PLAYER_none)
        return evaluate(player_);// 评价函数是相对于当前玩家的

    vector<uint16_t> moves;
    generateAllMoves(moves);

    int maxScore = INT_MIN;
    for (uint16_t move: moves)
    {
        if (board::MOVE_RET_ok & makeMove(move))// move可能导致自杀
        {
            int val = -negamax(depth - 1, nullptr);
            undoMakeMove();

            if (val > maxScore)
            {
                maxScore = val;

                if (pNextMove != nullptr)
                    *pNextMove = move;
            }
        }
    }

    return maxScore;
}

int SlimBoard::alphabeta(int depth, def::PLAYER_E maxPlayer, int alpha, int beta, uint16_t* pNextMove)
{
    if (depth == 0 || winner_ != def::PLAYER_none)
        return evaluate(maxPlayer);// 评价函数是相对于极大方的

    vector<uint16_t> moves;
    generateAllMoves(moves);

    if (player_ == maxPlayer)// 极大节点
    {
        int maxScore = alpha;

        for (uint16_t move: moves)
        {
            if (board::MOVE_RET_ok & makeMove(move))
            {
                int val = alphabeta(depth - 1, maxPlayer, maxScore, beta, nullptr);
                undoMakeMove();

                if (val > maxScore)// 本层为极大节点，取各子节点最大值
                {
                    maxScore = val;

                    if (pNextMove != nullptr)
                        *pNextMove = move;
                }

                if (maxScore >= beta)// beta剪枝：对于上层极小节点来说，希望寻找其子节点最小值，本层maxScore一旦大于beta，即可忽略
                {
                    break;
                }
            }
        }

        return maxScore;
    }
    else// 极小节点
    {
        int minScore = beta;

        for (uint16_t move: moves)
        {
            if (board::MOVE_RET_ok & makeMove(move))
            {
                int val = alphabeta(depth - 1, maxPlayer, alpha, minScore, nullptr);
                undoMakeMove();

                if (val < minScore)// 本层为极小节点，取各子节点最小值
                {
                    minScore = val;

                    if (pNextMove != nullptr)
                        *pNextMove = move;
                }

                if (alpha >= minScore)// alpha剪枝：对于上层极大节点来说，希望寻找其子节点最大值，本层alpha一旦大于minScore，即可忽略
                {
                    break;
                }
            }
        }

        return minScore;
    }

    return 0;
}

int SlimBoard::alphabetaWithNega(int depth, int alpha, int beta, uint16_t* pNextMove)
{
    if (depth == 0 || winner_ != def::PLAYER_none)       
        return evaluate(player_);// 评价函数是相对于当前玩家的

    vector<uint16_t> moves;
    generateAllMoves(moves);

    int maxScore = alpha;

    for (uint16_t move: moves)
    {
        if (board::MOVE_RET_ok & makeMove(move))
        {
            int val = -alphabetaWithNega(depth - 1, -beta, -maxScore, nullptr);
            undoMakeMove();

            if (val > maxScore)
            {
                maxScore = val;

                if (pNextMove != nullptr)
                    *pNextMove = move;
            }

            if (val >= beta)// beta剪枝
                break;
        }
    }

    return maxScore;
}

// 迭代加深的alpha-beta完全搜索
uint16_t SlimBoard::fullSearch()
{
    uint16_t move = 0;

    memset(cache_, 0, 65536 * sizeof (uint16_t));
    clock_t start = clock();

    for (int i = 1; i <= g_maxDepth; i++)
    {
        int score = alphabetaWithNegaSearch(i, -g_scoreCheckmate, g_scoreCheckmate, &move);

        if (score > g_scoreWin || score < -g_scoreWin)// 将死对方或被对方将死
            break;

        if (clock() - start > CLOCKS_PER_SEC)
            break;
    }

    return move;
}

int SlimBoard::quiescentSearch(int alpha, int beta)
{
    // 检查重复局面
    if (int status = detectRepeat(1))
    {
        return getRepeatScore(status);
    }

    // 到达极限递归深度
    if (distance_ == g_maxDepth)
    {
        return evaluate(player_);
    }

    int maxScore = -g_scoreCheckmate;
    vector<uint16_t> moves;

    if (isCheck())// 被将军，则生成所有走法
    {
        generateAllMoves(moves);
        std::sort(moves.begin(), moves.end(), // 将生成的走法按照历史走法的分值排序，得分高表示之前浅层递归已经记录过的走法，被排到最前
                  [this](uint16_t v1, uint16_t v2)// 因为相同局面浅一些的搜索可能会更适合剪枝
                    {
                        return this->cache_[v1] >= this->cache_[v2];
                    });
    }
    else// 否则先评估
    {
        int val = evaluate(player_);

        if (val > maxScore)// 更新alpha
            maxScore = val;

        if (val >= beta)// beta截断
            return val;

        static int MvvLva[8] = {0, 5, 1, 1, 3, 4, 3, 2};// 空 将 仕 象 马 车 炮 卒
        generateAllMoves(moves);
        std::sort(moves.begin(), moves.end(), // 将生成的走法按照MvvLva逆向排序，先搜索最优吃子方法
                  [this](uint16_t v1, uint16_t v2)
                    {
                        return MvvLva[def::extractPiece(static_cast<def::ICON_E>(extractDst(v1)))] >=
                               MvvLva[def::extractPiece(static_cast<def::ICON_E>(extractDst(v2)))];
                    });
    }

    // 同alpha-beta类似
    for (uint16_t move: moves)
    {
        if (board::MOVE_RET_ok & makeMove(move))
        {
            int val = -quiescentSearch(-beta, -maxScore);
            undoMakeMove();

            if (val > maxScore)
                maxScore = val;

            if (val >= beta)// beta剪枝
                break;
        }
    }

    if (maxScore == -g_scoreCheckmate)// 一步都走不了
        maxScore = -g_scoreCheckmate + distance_;

    return maxScore;
}

int SlimBoard::alphabetaWithNegaSearch(int depth, int alpha, int beta, uint16_t* pNextMove)
{
    if (depth == 0 || winner_ != def::PLAYER_none)
        return evaluate(player_);// 评价函数是相对于当前玩家的

    vector<uint16_t> moves;
    generateAllMoves(moves);
    std::sort(moves.begin(), moves.end(), // 将生成的走法按照历史走法的分值排序，得分高表示之前浅层递归已经记录过的走法，被排到最前
              [this](uint16_t v1, uint16_t v2)// 因为相同局面浅一些的搜索可能会更适合剪枝
                {
                    return this->cache_[v1] >= this->cache_[v2];
                });

    int maxScore = -g_scoreCheckmate;
    uint16_t maxMove = 0;

    for (uint16_t move: moves)
    {
        if (board::MOVE_RET_ok & makeMove(move))
        {
            int val = -alphabetaWithNegaSearch(depth - 1, -beta, -maxScore, nullptr);
            undoMakeMove();

            if (val > maxScore)// pv走法 beta走法
            {
                maxScore = val;
                maxMove = move;
            }

            if (val >= beta)// beta剪枝
            {
                break;
            }
        }
    }

    if (maxScore == -g_scoreCheckmate)// 此层无可走的棋，即被将死
    {
        maxScore = -g_scoreCheckmate + distance_;// 根据相对于根节点的步数给出评分
    }

    if (maxMove != 0)// 可以走棋的话，保存该最佳走法
    {
        cache_[maxMove] += depth * depth;// 层数越深，得分越低

        if (pNextMove != nullptr)
            *pNextMove = maxMove;
    }

    return maxScore;
}

// 指定走法走棋
uint8_t SlimBoard::makeMove(def::TMove move)
{
    uint8_t srcIdx = toIndex(move.src);
    uint8_t dstIdx = toIndex(move.dst);
    uint16_t innerMove = synthesisMove(srcIdx, dstIdx);

    if (!isValidMove(innerMove))
    {
        return board::MOVE_RET_invalid;
    }
    else
    {
        return makeMove(innerMove);
    }
}

// 不检查走法是否合法
uint8_t SlimBoard::makeMove(uint16_t move)
{
    uint8_t ret = 0;    
    
    uint8_t capture = movePiece(move); // 走棋
    if (isCheck()) // 走棋是否导致自己被将军
    {
        undoMovePiece(move, capture); // 是的话即为自杀
        ret |= board::MOVE_RET_suicide;
        return ret;
    }
    
    def::switchPlayer(player_); // 切换玩家
    // 注意：一定要切换玩家之后才能判断isCheck
    records_.push({move, capture, isCheck(), zoCurr_.getKey()}); // 保存历史走法

    ret |= board::MOVE_RET_ok;

    if (capture != 0)
    {
        ret |= board::MOVE_RET_eat;
    }

    if (isCheck())
    {
        ret |= board::MOVE_RET_check;

        if (isCheckmate())       
        {
            ret |= board::MOVE_RET_dead;
        }
    }

    distance_++;// 增加与根节点的距离

    return ret;
}

// 走棋，返回被吃的icon
uint8_t SlimBoard::movePiece(uint16_t move)
{
    uint8_t srcIdx = extractSrc(move);
    uint8_t dstIdx = extractDst(move);
    
    def::ICON_E srcIcon = getIcon(srcIdx);
    def::ICON_E dstIcon = getIcon(dstIdx);

    // 删除src、dst的icon
    delIcon(dstIdx, dstIcon);
    delIcon(srcIdx, srcIcon);
    // 在dst添加icon
    addIcon(dstIdx, srcIcon);
    
    return dstIcon;
}

// 悔棋，需要还原被吃掉的icon
void SlimBoard::undoMovePiece(uint16_t move, uint8_t capture)
{
    uint8_t srcIdx = extractSrc(move);
    uint8_t dstIdx = extractDst(move);
    
    def::ICON_E srcIcon = getIcon(srcIdx);
    def::ICON_E dstIcon = getIcon(dstIdx);

    // 删除dst的icon
    delIcon(dstIdx, dstIcon);
    // 还原src、dst的icon
    addIcon(srcIdx, dstIcon);
    addIcon(dstIdx, static_cast<def::ICON_E>(capture));
}

// 生成当前局面所有合法走法
void SlimBoard::generateAllMoves(vector<uint16_t>& moves, bool capture/* = false*/) const
{
    moves.clear();

    for (int src = 0; src < 256; src++)
    {
        if (getOwner(src) == player_)
        {
            switch (getIcon(src) & def::PIECE_MASK)
            {
                case def::PIECE_king:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        uint8_t dst = src + g_deltaKing[i];// 将加上偏移量
                        if (isInSquare(dst))// 在九宫格内
                        {
                            if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                                (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                                moves.push_back(synthesisMove(src, dst));
                        }
                    }

                    break;
                }
                case def::PIECE_advisor:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        uint8_t dst = src + g_deltaAdvisor[i];// 将加上偏移量
                        if (isInSquare(dst))// 在九宫格内
                        {
                            if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                                (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                                moves.push_back(synthesisMove(src, dst));
                        }
                    }

                    break;
                }
                case def::PIECE_bishop:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        uint8_t dst = src + g_deltaAdvisor[i];// 将加上偏移量
                        if (isInBoard(dst) && isHomeHalf(dst, player_) && board_[dst] == 0)// 象眼位置为空
                        {
                            dst += g_deltaAdvisor[i];// 得到象位置
                            if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                                (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                                moves.push_back(synthesisMove(src, dst));
                        }
                    }

                    break;
                }
                case def::PIECE_knight:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        uint8_t dst = src + g_deltaKing[i];// 马腿
                        if (board_[dst] == 0)// 马腿为空
                        {
                            for (int j = 0; j < 2; j++)
                            {
                                dst = src + g_deltaKnight[i][j];// 得到马位置
                                if (isInBoard(dst))
                                {
                                    if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                                        (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                                        moves.push_back(synthesisMove(src, dst));
                                }
                            }
                        }
                    }

                    break;
                }
                case def::PIECE_rook:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        int8_t delta = g_deltaKing[i];
                        uint8_t dst = src + delta;

                        while (isInBoard(dst))
                        {
                            if (board_[dst] == 0)// 空
                            {
                                if (!capture)// 不捕获棋子才能添加
                                    moves.push_back(synthesisMove(src, dst));
                            }
                            else// 非空的话，捕获或者不捕获均可添加
                            {
                                if (getOwner(dst) != player_)// 非空则停止当前循环
                                {
                                    moves.push_back(synthesisMove(src, dst));
                                }

                                break;
                            }

                            dst += delta;
                        }
                    }

                    break;
                }
                case def::PIECE_cannon:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        int8_t delta = g_deltaKing[i];
                        uint8_t dst = src + delta;

                        while (isInBoard(dst))
                        {
                            if (board_[dst] == 0)
                            {
                                if (!capture)// 不捕获棋子才能添加
                                {
                                    moves.push_back(synthesisMove(src, dst));// 无炮架可直接移动到空位置
                                    dst += delta;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }

                        dst += delta;// 跳过炮架
                        while (isInBoard(dst))
                        {
                            if (board_[dst] == 0)// 跳过空格
                            {
                                dst += delta;
                            }
                            else// 非空的话，捕获或者不捕获均可添加
                            {
                                if (getOwner(dst) != player_)// 非己方棋子
                                    moves.push_back(synthesisMove(src, dst));

                                break;// 一旦搜索到非空棋子即可停止搜索
                            }
                        }
                    }

                    break;
                }
                case def::PIECE_pawn:
                {
                    uint8_t dst = getPawnForwardIndex(src, player_);
                    if (isInBoard(dst))// 先向前移动
                    {
                        if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                            (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                            moves.push_back(synthesisMove(src, dst));
                    }

                    if (isAnotherHalf(src, player_))// 过河后才可左右移动
                    {
                        for (int i = -1; i <= 1; i += 2)
                        {
                            dst = src + i;
                            if (isInBoard(dst))// 左右移动
                            {
                                if ((capture && getOwner(dst) == def::getEnemyPlayer(player_)) ||// 捕获对方棋子
                                    (!capture && getOwner(dst) != player_))// 不捕获的话只要不是己方棋子即可
                                    moves.push_back(synthesisMove(src, dst));
                            }
                        }
                    }

                    break;
                }
                default:
                    break;
            }
        }
    }
}

// 获取当前局面下的玩家分数
int SlimBoard::getScore(def::PLAYER_E player) const
{
    if (player == def::PLAYER_black)
    {
        return blackScore_;
    }
    else if (player == def::PLAYER_red)
    {
        return redScore_;
    }
    else
    {
        return 0;
    }
}

// 获取某一位置的棋子
def::ICON_E SlimBoard::getIcon(def::TPos pos) const
{
    return getIcon(toIndex(pos));
}

// 获取pos棋子所属玩家
def::PLAYER_E SlimBoard::getOwner(def::TPos pos) const
{    
    return def::extractOwner(getIcon(pos));
}

// 获取下一走棋玩家
def::PLAYER_E SlimBoard::getNextPlayer() const
{   
    return player_;
}

// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
def::TMove SlimBoard::getTrigger() const
{
    def::TMove move = def::INVALID_MOVE;

    if (!records_.empty())
    {
        SlimBoard::TRecord record = records_.top();// 上一步走法
        move.src = toPos(extractSrc(record.move));
        move.dst = toPos(extractDst(record.move));
    }

    return move;
}

// 判断当前是否将军
// 注意：此函数之前只是更新了棋子，next_player尚未更新
bool SlimBoard::isCheck()
{
    // 此处的kingIdx是当前玩家的将的坐标
    uint8_t kingIdx = ((getNextPlayer() == def::PLAYER_black) ? blackKingIdx_ : redKingIdx_);
    // 此处的enemyPlayer是更新next_player之后的玩家
    def::PLAYER_E enemyPlayer = def::getEnemyPlayer(getNextPlayer());

    // 把将当作卒，如果能吃到对方的卒，即被对方的卒将军
    def::ICON_E enemyPawn = def::synthesisIcon(enemyPlayer, def::PIECE_pawn);
    if (board_[getPawnForwardIndex(kingIdx, player_)] == enemyPawn ||
        board_[kingIdx - 1] == enemyPawn ||
        board_[kingIdx + 1] == enemyPawn)
    {
        return true;
    }

    // 把将当作马，如果能吃到对方的马，即被对方的马将军
    def::ICON_E enemyKnight = def::synthesisIcon(enemyPlayer, def::PIECE_knight);
    static const int8_t knightCheckDelta[4][2] = {{-33, -18}, {-31, -14}, {14, 31}, {18, 33}};// 此处的马腿不使用正常的位置，以士的偏移位置作为马腿
    for (int i = 0; i < 4; i++)
    {
        if (board_[kingIdx + g_deltaAdvisor[i]] == 0)// 马腿位置为空才继续判断
        {
            for (int j = 0; j < 2; j++)
            {
                if (getIcon(kingIdx + knightCheckDelta[i][j]) == enemyKnight)// 有对方马，即被将军
                {
                    return true;
                }
            }
        }
    }

    // 向将的四个方向延伸，判断是否被车/炮将军，或者将帅对脸
    def::ICON_E enemyKing   = def::synthesisIcon(enemyPlayer, def::PIECE_king);
    def::ICON_E enemyRook   = def::synthesisIcon(enemyPlayer, def::PIECE_rook);
    def::ICON_E enemyCannon = def::synthesisIcon(enemyPlayer, def::PIECE_cannon);
    for (int i = 0; i < 4; i++)
    {
        int8_t delta = g_deltaKing[i];
        uint8_t cur = kingIdx + delta;

        while (isInBoard(cur)) // 先找到第一个非空棋子
        {
            if (board_[cur] != 0)
            {
                if (board_[cur] == enemyRook || board_[cur] == enemyKing)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }

            cur += delta;
        }

        cur += delta;
        while (isInBoard(cur)) // 若能继续往前找到对方的炮，即被对方的炮将死
        {
            if (board_[cur] != 0)
            {
                if (board_[cur] == enemyCannon)
                {
                    return true;
                }
                else
                {
                    break;
                }
            }

            cur += delta;
        }
    }

    return false;
}

// 判断当前是否将死
bool SlimBoard::isCheckmate()
{
    vector<uint16_t> moves;
    generateAllMoves(moves); // 生成当前所有合法走法

    for (uint16_t move: moves)
    {
        uint8_t capture = movePiece(move); // 尝试走棋

        if (!isCheck()) // 可应将，即可停止搜索
        {
            undoMovePiece(move, capture); // 还原
            return false;
        }
        else
        {
            undoMovePiece(move, capture); // 还原
        }
    }

    return true;
}

// 上下翻转后的一维坐标
uint8_t SlimBoard::getRotateIndex(uint8_t idx) const
{
    // 上方起点车坐标51，下方终点车坐标203
    // 51 + 203 == 254
    assert (idx <= 254);
    return 254 - idx;
}

// 是否在九宫格内
bool SlimBoard::isInSquare(uint8_t idx) const
{
    static const unsigned char square[256] =
    {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    assert (idx < 256);
    return square[idx] == 1;
}

// 获取idx位置处棋子的子力价值
uint8_t SlimBoard::getValue(def::ICON_E icon, uint8_t idx) const
{
    static const uint8_t redValue[7][256] =
    {
        { // king
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // advisor
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // bishop
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // knight
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
            0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
            0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
            0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
            0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
            0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
            0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
            0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
            0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
            0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // rook
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
            0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
            0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
            0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
            0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
            0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
            0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
            0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
            0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
            0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // cannon
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
            0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
            0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
            0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
            0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
            0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
            0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
            0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
            0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
            0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        },
        { // pawn
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
            0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
            0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
            0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
            0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
            0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
            0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        }
    };

    def::PLAYER_E owner = def::extractOwner(icon);
    def::PIECE_E  piece = def::extractPiece(icon);
    // 异常情况直接返回0
    if ((owner == def::PLAYER_none) || (piece == def::PIECE_empty))
    {
        return 0;
    }

    if (owner == def::PLAYER_black)
    {
        idx = getRotateIndex(idx); // 黑方要翻转pos坐标
    }

    return redValue[piece - 1][idx]; // 棋子值要减一才能作为下表使用
}

// 将一维坐标转换为二维坐标
def::TPos SlimBoard::toPos(uint8_t idx) const
{
    return {(idx >> 4) - 3, (idx & 15) - 3}; // 减去边缘的3
}

// 将二维坐标转换为一维坐标
uint8_t SlimBoard::toIndex(def::TPos pos) const
{
    return ((pos.row + 3 ) << 4) + (pos.col + 3); // 加上边缘的3
}

// 是否在棋盘内
bool SlimBoard::isInBoard(uint8_t idx) const
{
    static const unsigned char boardFlag[256] =
    {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    assert (idx < 256);
    return boardFlag[idx] == 1;
}

// 提取src
uint8_t SlimBoard::extractSrc(uint16_t move) const
{
    return move & 255; // 低8位为起点
}

// 提取dst
uint8_t SlimBoard::extractDst(uint16_t move) const
{
    return move >> 8; // 高8位为终点
}

// 将src和dst组合成一个走棋move
uint16_t SlimBoard::synthesisMove(uint8_t src, uint8_t dst) const
{
    return src + dst * 256;
}

bool SlimBoard::isValidSpan(uint8_t piece, uint8_t src, uint8_t dst) const
{
    static const uint8_t span[512] =
    {
                           0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0
    };

    int index = dst - src + 256;

    switch (piece)
    {
    case def::PIECE_king:
        return span[index] == 1;
    case def::PIECE_advisor:
        return span[index] == 2;
    case def::PIECE_bishop:
        return span[index] == 3;
    default:
        return false;
    }
}

// 查找一维坐标idx位置的icon
def::ICON_E SlimBoard::getIcon(uint8_t idx) const
{
    return static_cast<def::ICON_E>(board_[idx]);
}

// 查找一维坐标idx位置的icon的所属玩家
def::PLAYER_E SlimBoard::getOwner(uint8_t idx) const
{
    return def::extractOwner(getIcon(idx));
}

// 是否是合法走法
bool SlimBoard::isValidMove(uint16_t move)
{
    uint8_t src = extractSrc(move);
    uint8_t dst = extractDst(move);
    def::PLAYER_E player = getNextPlayer();

    // src不是当前玩家棋子，或dst是当前玩家棋子，即为非法
    if ((getOwner(src) != player) ||
        (getOwner(dst) == player))
    {
        return false;
    }

    def::PIECE_E piece = def::extractPiece(getIcon(src));
    switch (piece)
    {
        case def::PIECE_king:
        {
            return isInSquare(dst) && isValidSpan(def::PIECE_king, src, dst);// dst在九宫格内

            break;
        }
        case def::PIECE_advisor:
        {
            return isInSquare(dst) && isValidSpan(def::PIECE_advisor, src, dst);// dst在九宫格内

            break;
        }
        case def::PIECE_bishop:
        {
            return isInBoard(dst) && isValidSpan(def::PIECE_bishop, src, dst) &&// dst在棋盘内
                   isSameHalf(src, dst) && board_[getBishopEye(src, dst)] == 0;// src/dst位于同一侧,象眼为空

            break;
        }
        case def::PIECE_knight:
        {
            uint8_t leg = getKnightLeg(src, dst);// 非法位置得到的马腿位置为src
            return (leg != src) && (board_[leg] == 0);// 马腿为空
            
            break;
        }
        case def::PIECE_rook:
        case def::PIECE_cannon:
        {
            int delta = 0;
            if (isSameRow(src, dst))
            {
                delta = src < dst ? 1 : -1;
            }
            else if (isSameCol(src, dst))
            {
                delta = src < dst ? 16 : -16;
            }
            else
            {
                return false;
            }

            uint8_t next = src + delta;
            while (next != dst && board_[next] == 0)
            {
                next += delta;
            }

            if (next == dst) // 中间无棋子，则dst必须为空，或src为车
            {
                return (board_[dst] == 0) || (piece == def::PIECE_rook);
            }
            else // 中间有棋子，则dst必有对方棋子，src必为炮
            {
                if ((board_[dst] != 0) && (piece == def::PIECE_cannon))
                {
                    next += delta;
                    while (next != dst && board_[next] == 0)
                    {
                        next += delta;
                    }

                    return next == dst;
                }
                else
                {
                    return false;
                }
            }

            break;
        }
        case def::PIECE_pawn:
        {
            if (isAnotherHalf(src, player)) // 过河后可往左/右/前走
            {
                if ((dst - src == 1)  ||
                    (dst - src == -1) ||
                    (dst == getPawnForwardIndex(src, player)))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else // 未过河只能往前走
            {
                return dst == getPawnForwardIndex(src, player);
            }

            break;
        }
        default:
        {
            return false;
            
            break;
        }
    }

    return false;
}

// 是否在同一侧
bool SlimBoard::isSameHalf(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0x80) == 0; // 0 - 127为上方（黑），128 - 255为下方（红）
}

// 是否在对方区域
bool SlimBoard::isAnotherHalf(uint8_t src, def::PLAYER_E player) const
{
    return (src & 0x80) == ((player >> 4) << 7); // player >> 4  ->  black: 1 red: 0
}

// 是否在己方区域
bool SlimBoard::isHomeHalf(uint8_t src, def::PLAYER_E player) const
{
    return (src & 0x80) != ((player >> 4) << 7); // player >> 4  ->  black: 1 red: 0
}

// 计算马腿位置
uint8_t SlimBoard::getKnightLeg(uint8_t src, uint8_t dst) const// 非法位置得到的马腿位置为src
{
    static const int8_t knightLeg[512] =
    {
                                  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0
    };

    return src + knightLeg[dst - src + 256];
}

// 计算象眼位置
uint8_t SlimBoard::getBishopEye(uint8_t src, uint8_t dst) const
{
    return (src + dst) >> 1;
}

// 是否在同一行
bool SlimBoard::isSameRow(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0xf0) == 0; // 高四位相同
}

// 是否在同一列
bool SlimBoard::isSameCol(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0x0f) == 0; // 低四位相同
}

// 卒向前走一步后的坐标
uint8_t SlimBoard::getPawnForwardIndex(uint8_t idx, def::PLAYER_E player) const
{
    return idx - 16 + ((player >> 4) << 5); // player >> 4  ->  black: 1 red: 0
}

// 在idx位置放置icon
void SlimBoard::addIcon(uint8_t idx, def::ICON_E icon)
{
    board_[idx] = icon;// 添加棋子
    
    int value = getValue(icon, idx);
    int owner = def::extractOwner(icon);
    int piece = def::extractPiece(icon);

    if (owner == def::PLAYER_red)// 增加对应玩家分数
    {
        redScore_ += value;
        zoCurr_.Xor(g_zoTable[value - 9][idx]);// 更新zorbris
        // 更新将的坐标
        if (piece == def::PIECE_king)
        {
            redKingIdx_ = idx;
        }
    }
    else if (owner == def::PLAYER_black)
    {
        blackScore_ += value;
        zoCurr_.Xor(g_zoTable[value - 10][idx]);// 更新zorbris
        // 更新将的坐标
        if (piece == def::PIECE_king)
        {
            blackKingIdx_ = idx;
        }
    }
}

// 在idx位置删除icon
void SlimBoard::delIcon(uint8_t idx, def::ICON_E icon)
{
    board_[idx] = def::ICON_empty;// 删除棋子
    
    int value = getValue(icon, idx);
    int owner = getOwner(idx);
    int piece = def::extractPiece(icon);

    if (owner == def::PLAYER_red)// 减少对应玩家分数
    {
        redScore_ -= value; // 更新分数
        zoCurr_.Xor(g_zoTable[value - 9][idx]); // 更新zorbris
        // 更新将的坐标
        if (piece == def::PIECE_king)
        {
            redKingIdx_ = 0;
        }
    }
    else if (owner == def::PLAYER_black)
    {
        blackScore_ -= value; // 更新分数
        zoCurr_.Xor(g_zoTable[value - 10][idx]); // 更新zorbris
        // 更新将的坐标
        if (piece == def::PIECE_king)
        {
            blackKingIdx_ = 0;
        }
    }
}

// 查找player的将的坐标
uint8_t SlimBoard::findKing(def::PLAYER_E player) const
{
    def::ICON_E king = def::synthesisIcon(player, def::PIECE_king);

    for (int i = 0; i < 256; i++)
    {
        if (getIcon(i) == king)
        {
            return i;
        }
    }

    return 0;
}

/*
    A. 返回0，表示没有重复局面；
    B. 返回1，表示存在重复局面，但双方都无长将(判和)；
    C. 返回3(=1+2)，表示存在重复局面，本方单方面长将(判本方负)；
    D. 返回5(=1+4)，表示存在重复局面，对方单方面长将(判对方负)；
    E. 返回7(=1+2+4)，表示存在重复局面，双方长将(判和)。
*/
int SlimBoard::detectRepeat(int count)
{
    def::PLAYER_E player = def::getEnemyPlayer(player_); // 上一玩家
    bool selfPerpetualCheck = false;
    bool otherPerpetualCheck = false;

    for (unsigned int i = records_.size() - 1; i >= 0; ++i) // 由底向上搜索
    {
        const TRecord& record = records_.at(i);

        if (record.capture != 0) // 有吃子即可结束搜索
        {
            break;
        }

        if (player == player_)
        {
            selfPerpetualCheck = selfPerpetualCheck && record.check;

            if (record.key == zoCurr_.getKey())
            {
                if (--count == 0)
                {
                    return 1 + (selfPerpetualCheck ? 2 : 0) + (otherPerpetualCheck ? 4 : 0);
                }
            }
        }
        else
        {
            otherPerpetualCheck = otherPerpetualCheck && record.check;
        }

        def::switchPlayer(player);
    }

    return 0;
}

int SlimBoard::getRepeatScore(int status)
{
    assert (status != 0);

    int res = 0;

    if (status & 2)
    {
        res += distance_ - g_scoreCheckmate;
    }

    if (status & 4)
    {
        res += g_scoreCheckmate - distance_;
    }

    if (res == 0)
    {
        res = -g_scoreDraw;
    }

    return res;
}
