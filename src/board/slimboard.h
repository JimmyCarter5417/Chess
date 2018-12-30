#ifndef SLIMBOARD_H
#define SLIMBOARD_H

#include "board/board.h"
#include "util/zobrist.h"
#include "util/mystack.h"

#include <vector>
#include <stack>

using def::TPos;
using std::vector;
using std::stack;

class SlimBoard : public board::IBoard
{
public:
    SlimBoard();

    virtual void init();                                    // 开局
    virtual uint8_t autoMove();                             // 电脑走棋,返回EMoveRet的组合
    virtual uint8_t makeMove(def::TMove move);              // 指定走法走棋,返回EMoveRet的组合
    virtual bool undoMakeMove();                            // 悔棋

    virtual int getScore(def::PLAYER_E player) const;        // 获取当前局面下的玩家分数
    virtual def::ICON_E getIcon(def::TPos pos) const;      // 获取某一位置的棋子
    virtual def::PLAYER_E getNextPlayer() const;             // 获取下一走棋玩家
    virtual def::PLAYER_E getOwner(def::TPos pos) const;// 获取pos棋子所属玩家
    virtual def::TMove getTrigger() const;                  // 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

protected:
    // 内部使用一维坐标更为高效
    inline def::ICON_E getIcon(uint8_t idx) const;
    inline def::PLAYER_E getOwner(uint8_t idx) const;
    inline uint8_t getValue(def::ICON_E icon, uint8_t idx) const;

    uint8_t makeMove(uint16_t move);// 内部使用
    
    uint8_t movePiece(uint16_t move);
    void undoMovePiece(uint16_t move, uint8_t capture);

    void generateAllMoves(vector<uint16_t>& moves, bool capture = false) const;// 生成当前局面所有合法走法
    void initScore();

    // 相关算法
    int evaluate(def::PLAYER_E player) const;// 评价函数，相当重要
    // 基本递归算法测试
    int minimax(int depth, def::PLAYER_E maxPlayer, uint16_t* pNextMove);
    int negamax(int depth, uint16_t* pNextMove);
    int alphabeta(int depth, def::PLAYER_E maxPlayer, int alpha, int beta, uint16_t* pNextMove);
    int alphabetaWithNega(int depth, int alpha, int beta, uint16_t* pNextMove);
    // 下面是真正使用的算法
    uint16_t fullSearch();// 迭代加深的alpha-beta完全搜索
    int quiescentSearch(int alpha, int beta);// 静态搜索
    int alphabetaWithNegaSearch(int depth, int alpha, int beta, uint16_t* pNextMove);

    // 基础函数
    bool isValidMove(uint16_t move);
    bool isCheck();// 当前玩家是否被将军
    bool isCheckmate();// 当前玩家是否被将死
    
    void addIcon(uint8_t idx, def::ICON_E icon);
    void delIcon(uint8_t idx, def::ICON_E icon);
    void updateKingIdx(def::PLAYER_E player, uint8_t idx);

    inline bool isInSquare(uint8_t idx) const;
    inline bool isInBoard(uint8_t idx) const;
    inline bool isValidSpan(uint8_t piece, uint8_t src, uint8_t dst) const;

    inline bool isSameHalf(uint8_t src, uint8_t dst) const;
    inline bool isAnotherHalf(uint8_t src, def::PLAYER_E player) const;
    inline bool isHomeHalf(uint8_t src, def::PLAYER_E player) const;

    inline bool isSameRow(uint8_t src, uint8_t dst) const;
    inline bool isSameCol(uint8_t src, uint8_t dst) const;

    inline uint8_t getRotateIndex(uint8_t idx) const;
    inline uint8_t getPawnForwardIndex(uint8_t idx, def::PLAYER_E player) const;// 卒向前走一步后的坐标

    inline uint8_t getKnightLeg(uint8_t src, uint8_t dst) const;
    inline uint8_t getBishopEye(uint8_t src, uint8_t dst) const;

    inline TPos toPos(uint8_t idx) const;
    inline uint8_t toIndex(TPos pos) const;

    inline uint8_t getMoveSrc(uint16_t move) const;
    inline uint8_t getMoveDst(uint16_t move) const;
    inline uint16_t getMove(uint8_t src, uint8_t dst) const;

    inline uint8_t findKing(def::PLAYER_E player) const;

    int detectRepeat(int count);
    int getRepeatScore(int status);

private:
    struct TRecord
    {
        uint16_t move;     // 当前走法
        uint8_t  capture;  // 走棋后dst坐标被捕获的棋子
        bool     check;    // 走棋后是否能将军
        uint16_t key;      // 走棋前局面的校验码

        TRecord(uint16_t Move, uint8_t Capture, bool Check, uint16_t Key)
        {
            move = Move;
            capture = Capture;
            check = Check;
            key = Key;
        }
    };

private:
    uint8_t board_[256];
    uint16_t cache_[65536];

    int distance_;

    uint8_t redKingIdx_;
    uint8_t blackKingIdx_;

    int redScore_;
    int blackScore_;

    def::PLAYER_E winner_;
    def::PLAYER_E player_;

    MyStack<TRecord> records_;

    Zobrist zoCurr_;    
};

#endif // SLIMBOARD_H
