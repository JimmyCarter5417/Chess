#ifndef SLIMBOARD_H
#define SLIMBOARD_H

#include "board/board.h"
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

    virtual int getScore(def::EPlayer player) const;        // 获取当前局面下的玩家分数
    virtual def::EPiece getPiece(def::TPos pos) const;      // 获取某一位置的棋子
    virtual def::EPlayer getNextPlayer() const;             // 获取下一走棋玩家
    virtual def::EPlayer getPieceOwner(def::TPos pos) const;// 获取pos棋子所属玩家
    virtual def::TMove getTrigger() const;                  // 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

protected:
    // 内部使用一维坐标更为高效
    inline def::EPlayer getPieceOwner(uint8_t idx) const;
    inline def::EPiece getPiece(uint8_t idx) const;
    uint8_t makeMove(uint16_t move);// 内部使用
    
    uint8_t movePiece(uint16_t move);
    void undoMovePiece(uint16_t move, uint8_t capture);

    void generateAllMoves(vector<uint16_t>& moves) const;// 生成当前局面所有合法走法
    void initScore();

    int evaluate();
    int minimax(int depth, def::EPlayer maxPlayer, uint16_t& nextMove);
    int negamax(int depth, uint16_t& nextMove);
    int alphadeta(int depth, def::EPlayer maxPlayer, int alpha, int beta, uint16_t& nextMove);

    bool isValidMove(uint16_t move);
    bool isCheck();// 当前玩家是否被将军
    bool isCheckmate();// 当前玩家是否被将死
    
    void addPiece(uint8_t idx, def::EPiece piece);
    void delPiece(uint8_t idx, def::EPiece piece);
    void updateKingIdx(def::EPlayer player, uint8_t idx);

    inline bool isInSquare(uint8_t idx) const;
    inline bool isInBoard(uint8_t idx) const;
    inline bool isValidSpan(uint8_t piece, uint8_t src, uint8_t dst) const;

    inline bool isSameHalf(uint8_t src, uint8_t dst) const;
    inline bool isAnotherHalf(uint8_t src, def::EPlayer player) const;
    inline bool isHomeHalf(uint8_t src, def::EPlayer player) const;

    inline bool isSameRow(uint8_t src, uint8_t dst) const;
    inline bool isSameCol(uint8_t src, uint8_t dst) const;

    inline uint8_t getRotateIndex(uint8_t idx) const;
    inline uint8_t getPieceValue(def::EPiece piece, uint8_t idx) const;
    inline uint8_t getPawnForwardIndex(uint8_t idx, def::EPlayer player) const;// 卒向前走一步后的坐标

    inline uint8_t getKnightLeg(uint8_t src, uint8_t dst) const;
    inline uint8_t getBishopEye(uint8_t src, uint8_t dst) const;

    inline TPos toPos(uint8_t idx) const;
    inline uint8_t toIndex(TPos pos) const;

    inline uint8_t getMoveSrc(uint16_t move) const;
    inline uint8_t getMoveDst(uint16_t move) const;
    inline uint16_t getMove(uint8_t src, uint8_t dst) const;



private:
    struct TRecord
    {
        uint8_t capture;  // dst坐标被捕获的棋子
        uint16_t move;    // 当前走法

        TRecord(uint8_t c = 0, uint16_t m = 0)
        {
            capture = c;
            move = m;
        }
    };

private:
    uint8_t board_[256];

    uint8_t redKingIdx_;
    uint8_t blackKingIdx_;

    int redScore_;
    int blackScore_;

    def::EPlayer player_;
    stack<TRecord> history_;
};

#endif // SLIMBOARD_H
