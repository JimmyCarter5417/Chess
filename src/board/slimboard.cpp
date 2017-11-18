#include "slimboard.h"

#include <assert.h>
#include <memory>

using namespace std;

static const int8_t g_kingDelta[4]    = {-16,  -1,  1, 16};
static const int8_t g_advisorDelta[4] = {-17, -15, 15, 17};
static const int8_t g_knightDelta[4][2] = {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}};// 马的正常delta


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
    /*static const uint8_t initBoard[256] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0, 17, 18, 19,  0,  0,  0,  0,  0,  0,// 起点车51
      0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0, 13,  0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,
      0,  0,  0, 15,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0, 15,  0,  0, 22,  0,  0, 15,  0,  0,  0,  0,
      0,  0,  0, 12,  0,  0,  0, 11,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0, 11, 10,  9, 10,  0,  0, 12,  0,  0,  0,  0,// 终点车203
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };*/
    
    memcpy(board_, initBoard, 256);

    redKingIdx_ = 199;
    blackKingIdx_ = 55;
    
    redScore_ = 0;
    blackScore_ = 0;
    player_ = def::EP_red;
    //player_ = def::EP_black;
    
    stack<TRecord> tmp;
    history_.swap(tmp);   
}

bool SlimBoard::undoMakeMove()// 悔棋
{
    if (history_.empty())
        return false;

    SlimBoard::TRecord record = history_.top();
    history_.pop();    
    undoMovePiece(record.move, record.capture);
    
    def::switchPlayer(player_);

    return true;
}

uint8_t SlimBoard::autoMove()// 电脑计算走棋
{
    // todo
    return 0;
}

uint8_t SlimBoard::makeMove(def::TMove move)// 指定走法走棋
{
    uint8_t srcIndex = toIndex(move.src);
    uint8_t dstIndex = toIndex(move.dst);
    uint16_t innerMove = getMove(srcIndex, dstIndex);

    if (!isValidMove(innerMove))
        return board::EMR_invalid;
    else
        return makeMove(innerMove);
}

// 不检查走法是否合法
uint8_t SlimBoard::makeMove(uint16_t move)
{
    uint8_t ret = 0;    
    
    uint8_t capture = movePiece(move);
    if (isCheck())// 走棋是否导致自己被将军
    {
        undoMovePiece(move, capture);// 是的话即为自杀
        ret |= board::EMR_suicide;
        return ret;
    }
    
    def::switchPlayer(player_);// 切换玩家
    history_.push({capture, move});// 保持纪录
    
    ret |= board::EMR_ok;

    if (capture != 0)
        ret |= board::EMR_eat; 

    if (isCheck())
    {
        ret |= board::EMR_check;

        if (isCheckmate())
            ret |= board::EMR_dead;
    }

    return ret;
}

uint8_t SlimBoard::movePiece(uint16_t move)
{
    uint8_t srcIndex = getMoveSrc(move);
    uint8_t dstIndex = getMoveDst(move);
    
    def::EPiece srcPiece = getPiece(srcIndex);
    def::EPiece dstPiece = getPiece(dstIndex);
    
    if (dstPiece != 0)
        delPiece(dstIndex, dstPiece);
    
    delPiece(srcIndex, srcPiece);
    addPiece(dstIndex, srcPiece);

    // 更新将位置，注意srcPiece/dstPiece与srcIndex/dstIndex
    if ((srcPiece & def::g_pieceMask) == def::g_king)
        updateKingIdx(static_cast<def::EPlayer>(srcPiece & def::g_colorMask), dstIndex);
    
    return dstPiece;
}

void SlimBoard::undoMovePiece(uint16_t move, uint8_t capture)
{
    uint8_t srcIndex = getMoveSrc(move);
    uint8_t dstIndex = getMoveDst(move);
    
    def::EPiece srcPiece = getPiece(srcIndex);
    def::EPiece dstPiece = getPiece(dstIndex);
    
    delPiece(dstIndex, dstPiece);
    addPiece(srcIndex, dstPiece);
    
    if (capture != 0)
        addPiece(dstIndex, static_cast<def::EPiece>(capture));

    // 更新将位置，注意srcPiece/dstPiece与srcIndex/dstIndex
    if ((dstPiece & def::g_pieceMask) == def::g_king)
        updateKingIdx(static_cast<def::EPlayer>(dstPiece & def::g_colorMask), srcIndex);
}

void SlimBoard::generateAllMoves(vector<uint16_t>& moves)// 生成当前局面所有合法走法
{
    moves.clear();

    for (int src = 0; src < 256; src++)
    {
        if (getPieceOwner(src) == player_)
        {
            switch (getPiece(src) & def::g_pieceMask)
            {
            case def::g_king:
            {
                for (int i = 0; i < 4; i++)
                {
                    uint8_t dst = src + g_kingDelta[i];// 将加上偏移量
                    if (isInSquare(dst) && getPieceOwner(dst) != player_)// 在九宫格内，且dst不是己方棋子
                        moves.push_back(getMove(src, dst));
                }
            }
                break;
            case def::g_advisor:
            {
                for (int i = 0; i < 4; i++)
                {
                    uint8_t dst = src + g_advisorDelta[i];// 将加上偏移量
                    if (isInSquare(dst) && getPieceOwner(dst) != player_)// 在九宫格内，且dst不是己方棋子
                        moves.push_back(getMove(src, dst));
                }
            }
                break;
            case def::g_bishop:
            {
                for (int i = 0; i < 4; i++)
                {
                    uint8_t dst = src + g_advisorDelta[i];// 将加上偏移量
                    if (isInBoard(dst) && isHomeHalf(dst, player_) && board_[dst] == 0)// 象眼位置为空
                    {
                        dst += g_advisorDelta[i];// 得到象位置
                        if (getPieceOwner(dst) != player_)// 非己方棋子
                            moves.push_back(getMove(src, dst));
                    }
                }
            }
                break;
            case def::g_knight:
            {
                for (int i = 0; i < 4; i++)
                {
                    uint8_t dst = src + g_kingDelta[i];// 马腿
                    if (board_[dst] == 0)// 马腿为空
                    {
                        for (int j = 0; j < 2; j++)
                        {
                            dst = src + g_knightDelta[i][j];// 得到马位置
                            if (isInBoard(dst) && getPieceOwner(dst) != player_)// 非己方棋子
                                moves.push_back(getMove(src, dst));
                        }
                    }
                }
            }
                break;
            case def::g_rook:
            {
                for (int i = 0; i < 4; i++)
                {
                    int8_t delta = g_kingDelta[i];
                    uint8_t dst = src + delta;

                    while (isInBoard(dst))
                    {
                        if (board_[dst] == 0)// 空
                        {
                            moves.push_back(getMove(src, dst));
                        }
                        else
                        {
                            if (getPieceOwner(dst) != player_)// 非空则停止当前循环
                            {
                                moves.push_back(getMove(src, dst));
                            }

                            break;
                        }

                        dst += delta;
                    }
                }
            }
                break;
            case def::g_cannon:
            {
                for (int i = 0; i < 4; i++)
                {
                    int8_t delta = g_kingDelta[i];
                    uint8_t dst = src + delta;

                    while (isInBoard(dst))
                    {
                        if (board_[dst] == 0)
                        {
                            moves.push_back(getMove(src, dst));// 无炮架可直接移动到空位置
                            dst += delta;
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
                        else
                        {
                            if (getPieceOwner(dst) != player_)// 非己方棋子
                                moves.push_back(getMove(src, dst));

                            break;// 一旦搜索到对方棋子即可停止搜索
                        }
                    }
                }
            }
                break;
            case def::g_pawn:
            {
                uint8_t dst = getPawnForwardIndex(src, player_);
                if (isInBoard(dst) && getPieceOwner(dst) != player_)// 先向前移动
                    moves.push_back(getMove(src, dst));

                if (isAnotherHalf(src, player_))// 过河后才可左右移动
                {
                    for (int i = -1; i <= 1; i += 2)
                    {
                        dst = src + i;
                        if (isInBoard(dst) && getPieceOwner(dst) != player_)
                            moves.push_back(getMove(src, dst));
                    }
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

int SlimBoard::getScore(def::EPlayer player) const// 获取当前局面下的玩家分数
{
    if (player == def::EP_black)
        return blackScore_;
    else if (player == def::EP_red)
        return redScore_;
    else
        return 0;
}

def::EPiece SlimBoard::getPiece(def::TPos pos) const// 获取某一位置的棋子
{
    return getPiece(toIndex(pos));
}

def::EPlayer SlimBoard::getNextPlayer() const// 获取下一走棋玩家
{   
    return player_;
}

def::EPlayer SlimBoard::getPieceOwner(def::TPos pos) const// 获取pos棋子所属玩家
{    
    return getPieceOwner(toIndex(pos));
}

def::TMove SlimBoard::getTrigger() const// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标
{
    def::TMove move;

    if (!history_.empty())
    {
        SlimBoard::TRecord record = history_.top();// 上一步走法
        move.src = toPos(getMoveSrc(record.move));
        move.dst = toPos(getMoveDst(record.move));
    }

    return move;
}

bool SlimBoard::isCheck()
{
    uint8_t kingIdx = getNextPlayer() == def::EP_black ? blackKingIdx_ : redKingIdx_;
    def::EPlayer otherPlayer = def::getOtherPlayer(player_);

    // 把将当作卒，如果能吃到对方的卒，即被对方的卒将军
    def::EPiece otherPawn = static_cast<def::EPiece>(otherPlayer | def::g_pawn);
    if (board_[getPawnForwardIndex(kingIdx, player_)] == otherPawn ||
        board_[kingIdx - 1] == otherPawn ||
        board_[kingIdx + 1] == otherPawn)
        return true;

    // 把将当作马，如果能吃到对方的马，即被对方的马将军
    def::EPiece otherKnight = static_cast<def::EPiece>(otherPlayer | def::g_knight);
    static const int8_t knightCheckDelta[4][2] = {{-33, -18}, {-31, -14}, {14, 31}, {18, 33}};// 此处的马腿不使用正常的位置，以士的偏移位置作为马腿
    for (int i = 0; i < 4; i++)
    {
        if (board_[kingIdx + g_advisorDelta[i]] == 0)// 马腿位置为空才继续判断
        {
            for (int j = 0; j < 2; j++)
            {
                if (getPiece(kingIdx + knightCheckDelta[i][j]) == otherKnight)// 有对方马，即被将军
                    return true;
            }
        }
    }

    // 向将的四个方向延伸，判断是否被车/炮将军，或者将帅对脸
    def::EPiece otherKing = static_cast<def::EPiece>(otherPlayer | def::g_king);
    def::EPiece otherRook = static_cast<def::EPiece>(otherPlayer | def::g_rook);
    def::EPiece otherCannon = static_cast<def::EPiece>(otherPlayer | def::g_cannon);
    for (int i = 0; i < 4; i++)
    {
        int8_t delta = g_kingDelta[i];
        uint8_t cur = kingIdx + delta;

        while (isInBoard(cur))// 先找到第一个非空棋子
        {
            if (board_[cur] != 0)
            {
                if (board_[cur] == otherRook || board_[cur] == otherKing)
                    return true;
                else
                    break;
            }

            cur += delta;
        }

        cur += delta;
        while (isInBoard(cur))// 若能继续往前找到对方的炮，即被对方的炮将死
        {
            if (board_[cur] != 0)
            {
                if (board_[cur] == otherCannon)
                    return true;
                else
                    break;
            }

            cur += delta;
        }
    }

    return false;
}

bool SlimBoard::isCheckmate()
{
    vector<uint16_t> moves;
    generateAllMoves(moves);

    for (uint16_t move: moves)
    {
        uint8_t capture = movePiece(move);

        if (!isCheck())
        {
            undoMovePiece(move, capture);
            return false;
        }
        else
        {
            undoMovePiece(move, capture);
        }
    }

    return true;
}


uint8_t SlimBoard::getRotateIndex(uint8_t idx) const
{
    return 254 - idx;
}

bool SlimBoard::isInSquare(uint8_t idx) const
{
    static const char square[256] =
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

uint8_t SlimBoard::getPieceValue(def::EPiece piece, uint8_t idx) const
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

    idx = ((piece & def::g_colorMask) == def::g_redFlag) ? idx : getRotateIndex(idx);// 黑方要翻转pos坐标
    return redValue[(piece & def::g_pieceMask) - 1][idx];// 棋子值要减一
}

TPos SlimBoard::toPos(uint8_t idx) const
{
    return {(idx >> 4) - 3, (idx & 15) - 3};// 减去边缘的3
}

uint8_t SlimBoard::toIndex(def::TPos pos) const
{
    return ((pos.row + 3 ) << 4) + (pos.col + 3);// 加上边缘的3
}

bool SlimBoard::isInBoard(uint8_t idx) const
{
    static const char boardFlag[256] =
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

    return boardFlag[idx] == 1;
}

uint8_t SlimBoard::getMoveSrc(uint16_t move) const
{
    return move & 255;// 低8位为起点
}

uint8_t SlimBoard::getMoveDst(uint16_t move) const
{
    return move >> 8;// 高8位为终点
}

uint16_t SlimBoard::getMove(uint8_t src, uint8_t dst) const
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
    case def::g_king:
        return span[index] == 1;
    case def::g_advisor:
        return span[index] == 2;
    case def::g_bishop:
        return span[index] == 3;
    default:
        return false;
    }
}

def::EPiece SlimBoard::getPiece(uint8_t idx) const
{
    return static_cast<def::EPiece>(board_[idx]);
}

def::EPlayer SlimBoard::getPieceOwner(uint8_t idx) const// 用于内部使用一维坐标
{
    uint8_t color = (board_[idx] & def::g_colorMask);
    
    if (color == def::g_redFlag)
        return def::EP_red;
    else if (color == def::g_blackFlag)
        return def::EP_black;
    else
        return def::EP_none;
}

bool SlimBoard::isValidMove(uint16_t move)
{
    uint8_t src = getMoveSrc(move);
    uint8_t dst = getMoveDst(move);
    def::EPlayer player = getNextPlayer();

    // 判断src是否是当前玩家棋子
    if (getPieceOwner(src) != player)
        return false;

    // 判断dst是否是当前玩家棋子
    if (getPieceOwner(dst) == player)
        return false;

    switch (getPiece(src) & def::g_pieceMask)
    {
    case def::g_king:
    {
        return isInSquare(dst) && isValidSpan(def::g_king, src, dst);// dst在九宫格内
    }
        break;
    case def::g_advisor:
    {
        return isInSquare(dst) && isValidSpan(def::g_advisor, src, dst);// dst在九宫格内
    }
        break;
    case def::g_bishop:
    {
        return isInBoard(dst) && isValidSpan(def::g_bishop, src, dst) &&// dst在棋盘内
               isSameHalf(src, dst) && board_[getBishopEye(src, dst)] == 0;// src/dst位于同一侧,象眼为空
    }
        break;
    case def::g_knight:
    {
        uint8_t leg = getKnightLeg(src, dst);// 非法位置得到的马腿位置为src
        return leg != src && board_[leg] == 0;// 马腿为空
    }
        break;
    case def::g_rook:
    case def::g_cannon:
    {
        int delta = 0;
        if (isSameRow(src, dst))
            delta = src < dst ? 1 : -1;
        else if (isSameCol(src, dst))
            delta = src < dst ? 16 : -16;
        else
            return false;

        uint8_t next = src + delta;
        while (next != dst && board_[next] == 0)
            next += delta;

        if (next == dst)// 中间无棋子，则dst必须为空，或src为车
        {
            return board_[dst] == 0 || (getPiece(src) & def::g_pieceMask) == def::g_rook;
        }
        else// 中间有棋子，则dst必有对方棋子，src必为炮
        {
            if (board_[dst] != 0 && (getPiece(src) & def::g_pieceMask) == def::g_cannon)
            {
                next += delta;
                while (next != dst && board_[next] == 0)
                    next += delta;

                return next == dst;
            }
            else
            {
                return false;
            }
        }
    }
        break;
    case def::g_pawn:
    {
        if (isAnotherHalf(src, player))// 过河后可往左/右/前走
        {
            if (dst - src == 1 || dst - src == -1 || dst == getPawnForwardIndex(src, player))
                return true;
            else
                return false;
        }
        else// 未过河只能往前走
        {
            return dst == getPawnForwardIndex(src, player);
        }
    }
        break;
    default:
        return false;
    }

    return false;
}

bool SlimBoard::isSameHalf(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0x80) == 0;// 0 - 127为上方（黑），128 - 255为下方（红）
}

bool SlimBoard::isAnotherHalf(uint8_t src, def::EPlayer player) const
{
    return (src & 0x80) == ((player >> 4) << 7);// player >> 4  ->  black: 1 red: 0
}

bool SlimBoard::isHomeHalf(uint8_t src, def::EPlayer player) const
{
    return (src & 0x80) != ((player >> 4) << 7);// player >> 4  ->  black: 1 red: 0
}

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

uint8_t SlimBoard::getBishopEye(uint8_t src, uint8_t dst) const
{
    return (src + dst) >> 1;
}

bool SlimBoard::isSameRow(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0xf0) == 0;// 高四位相同
}

bool SlimBoard::isSameCol(uint8_t src, uint8_t dst) const
{
    return ((src ^ dst) & 0x0f) == 0;// 低四位相同
}

uint8_t SlimBoard::getPawnForwardIndex(uint8_t idx, def::EPlayer player) const// 卒向前走一步后的坐标
{
    return idx - 16 + ((player >> 4) << 5);// player >> 4  ->  black: 1 red: 0
}

void SlimBoard::addPiece(uint8_t idx, def::EPiece piece)
{
    board_[idx] = piece;// 添加棋子
    
    int val = getPieceValue(piece, idx);
    int color = (piece & def::g_colorMask);

    if (color == def::g_redFlag)// 增加对应玩家分数
        redScore_ += val;
    else if (color == def::g_blackFlag)
        blackScore_ += val;
}

void SlimBoard::delPiece(uint8_t idx, def::EPiece piece)
{
    board_[idx] = 0;// 删除棋子
    
    int val = getPieceValue(piece, idx);
    int color = (piece & def::g_colorMask);

    if (color == def::g_redFlag)// 减少对应玩家分数
        redScore_ -= val;
    else if (color == def::g_blackFlag)
        blackScore_ -= val;
}

void SlimBoard::updateKingIdx(def::EPlayer player, uint8_t idx)
{
    if (player == def::EP_black)
        blackKingIdx_ = idx;
    else if (player == def::EP_red)
        redKingIdx_ = idx;
}
