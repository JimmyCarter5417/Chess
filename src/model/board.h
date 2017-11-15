#ifndef BOARD_H
#define BOARD_H

#include "memo.h"
#include "model/model.h"
#include "util/def.h"
#include "util/co.h"
#include "util/hash.h"

#include <vector>
#include <unordered_set>
#include <memory>

using def::EPlayer;
using def::TPos;
using def::TDelta;
using def::int8;
using std::vector;
using std::unordered_set;
using std::shared_ptr;

class Board : public model::IModel
{
public:    
    Board();

    virtual void init();// 开局
    virtual bool rotate();// 翻转棋盘
    virtual bool undo();// 悔棋

    virtual int getScore(def::EPlayer player) const;// 获取当前局面下的玩家分数
    virtual def::EPiece getPiece(def::TPos pos) const;// 获取某一位置的棋子
    virtual def::EPlayer getNextPlayer() const;// 获取下一走棋玩家
    virtual def::EPlayer getPieceOwner(def::TPos pos) const;// 获取pos棋子所属玩家
    virtual std::pair<def::TPos, def::TPos> getTrigger() const;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

    virtual def::int8 movePiece(def::TPos prevPos, def::TPos currPos);// 尝试走棋，返回EMoveRet的组合
    virtual std::pair<TPos, TPos> calcBestMove(def::int8 depth);// 遍历n层，计算下一步最佳走法

    virtual bool run();

    bool generateAllMoves(vector<std::pair<TPos, TPos>>& moves);
    int calcBestScore(def::int8 depth, std::pair<TPos, TPos>& move);

    int ab(int depth, int alpha, int beta, def::EPlayer player, std::pair<TPos, TPos>& move);
protected:
    // 玩家棋子位置集合
    struct TPieceSet
    {
        TPos king;
        int score;
        unordered_set<TPos> defenders;
        unordered_set<TPos> attackers;

        void swap(TPieceSet& other)
        {
            std::swap(king, other.king);
            std::swap(score, other.score);
            defenders.swap(other.defenders);
            attackers.swap(other.attackers);
        }
    };

    // 当前局面快照
    class Snapshot
    {
        friend class Board;

        vector<vector<int8>> board_;// 棋盘
        EPlayer player_;// 下一步要走棋的玩家

        TPieceSet upPieceSet_;// 上方玩家棋子集合
        TPieceSet downPieceSet_;// 下方玩家棋子集合

        // 第四位和第五位表示上下部分
        int8 upFlag_;// 默认上面为黑色
        int8 downFlag_;// 默认下面为红色

        std::pair<TPos, TPos> trigger_;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

    public:
        Snapshot()
        {
            static const vector<vector<int8>> initialBoard = {
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

            // 初始化snapshot
            board_ = initialBoard;
            player_ = def::EP_down;// 下方先走
            // 上方棋子集合
            upPieceSet_.king = {0, 4};
            upPieceSet_.score = 888;
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
            // 下方棋子集合
            downPieceSet_.king = {9, 4};
            downPieceSet_.score = 888;
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

            upFlag_   = def::g_blackFlag;// 默认上面为黑色
            downFlag_ = def::g_redFlag;// 默认下面为红色

            trigger_ = {def::g_nullPos, def::g_nullPos};// 起始局面无先前走棋


        }

        Snapshot(const Snapshot& other)
        {
            board_ = other.board_;
            player_ = other.player_;
            upPieceSet_ = other.upPieceSet_;
            downPieceSet_ = other.downPieceSet_;
            upFlag_ = other.upFlag_;
            downFlag_ = other.downFlag_;
            trigger_ = other.trigger_;
        }

        // 赋值运算符的现代写法
        Snapshot& operator=(Snapshot other)
        {
            swap(other);
            return *this;
        }

        void swap(Snapshot& other)
        {
            board_.swap(other.board_);
            std::swap(player_,other.player_);
            upPieceSet_.swap(other.upPieceSet_);
            downPieceSet_.swap(other.downPieceSet_);
            std::swap(upFlag_, other.upFlag_);
            std::swap(downFlag_, other.downFlag_);
            std::swap(trigger_, other.trigger_);
        }

        void rotate()
        {
            //todo: optimize
            vector<vector<int8>> tmp(co::g_rowNum, vector<int8>(co::g_colNum, 0));
            for (int i = 0; i < co::g_rowNum; i++)
            {
                for (int j = 0; j < co::g_colNum; j++)
                {
                    TPos rotatePos = co::getRotatePos({i, j});
                    tmp[i][j] = board_[rotatePos.row][rotatePos.col];
                }
            }

            board_.swap(tmp);
            def::switchPlayer(player_);
            upPieceSet_.swap(downPieceSet_);
            std::swap(upFlag_, downFlag_);// 交换上下方标志位
            trigger_.first = co::getRotatePos(trigger_.first);// 旋转trigger
            trigger_.second = co::getRotatePos(trigger_.second);
        }
    };

    // 对于player来说，从prevPos到currPos是否合法
    bool isValidMove(TPos prevPos, TPos currPos, EPlayer player) const;

    typedef bool (Board::*PosFunc)(TPos pos, EPlayer player) const;// 判断位置是否合法
    typedef bool (Board::*DeltaFunc)(TDelta delta, EPlayer player) const;// 判断偏移量是否合法
    typedef bool (Board::*RuleFunc)(TPos prevPos, TPos currPos) const;// 棋子特定的规则
    bool isValidMove(TPos prevPos, TPos currPos, EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule) const;

    bool isValidKingPos(TPos pos, EPlayer player) const;
    bool isValidAdvisorPos(TPos pos, EPlayer player) const;
    bool isValidBishopPos(TPos pos,def:: EPlayer player) const;
    bool isValidKnightPos(TPos pos, EPlayer player) const;
    bool isValidRookPos(TPos pos, EPlayer player) const;
    bool isValidCannonPos(TPos pos, EPlayer player) const;
    bool isValidPawnPos(TPos pos, EPlayer player) const;

    bool isValidKingDelta(TDelta delta, EPlayer player) const;
    bool isValidAdvisorDelta(TDelta delta, EPlayer player) const;
    bool isValidBishopDelta(TDelta delta, EPlayer player) const;
    bool isValidKnightDelta(TDelta delta, EPlayer player) const;
    bool isValidRookDelta(TDelta delta, EPlayer player) const;
    bool isValidCannonDelta(TDelta delta, EPlayer player) const;
    bool isValidPawnDelta(TDelta delta, EPlayer player) const;

    // 不再检查pos及delta，默认前面已检查
    bool isValidKingRule(TPos prevPos, TPos currPos) const;
    bool isValidAdvisorRule(TPos prevPos, TPos currPos) const;
    bool isValidBishopRule(TPos prevPos, TPos currPos) const;
    bool isValidKnightRule(TPos prevPos, TPos currPos) const;
    bool isValidRookRule(TPos prevPos, TPos currPos) const;
    bool isValidCannonRule(TPos prevPos, TPos currPos) const;
    bool isValidPawnRule(TPos prevPos, TPos currPos) const;

    bool isValidKingMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidAdvisorMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidBishopMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidKnightMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidRookMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidCannonMove(TPos prevPos, TPos currPos, EPlayer player) const;
    bool isValidPawnMove(TPos prevPos, TPos currPos, EPlayer player) const;

    const unordered_set<TDelta>& getValidKingDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidAdvisorDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidBishopDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidKnightDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidRookDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidCannonDelta(EPlayer player) const;
    const unordered_set<TDelta>& getValidPawnDelta(EPlayer player) const;

    int getValue(int8 piece, TPos pos, EPlayer player) const;

    bool check(EPlayer player);
    bool checkmate(EPlayer player);

    bool isSuicide(TPos prevPos, TPos currPos, EPlayer player);
    bool updatePieceSet(TPos prevPos, TPos currPos, EPlayer player);

    inline bool isPiece(TPos pos, int piece) const;// 判断pos位置是否为特定棋子（不分颜色）
    inline bool isKing(TPos pos) const;
    inline bool isDefensivePiece(TPos pos) const;
    inline bool isAttactivePiece(TPos pos) const;

    bool isKingMeeting() const;

    shared_ptr<Snapshot> createSnapshot();
    bool saveSnapshot();
    bool loadSnapshot(shared_ptr<Snapshot> snapshot);
    bool updateSnapshot(TPos prevPos, TPos currPos, EPlayer player);



private:
    shared_ptr<Snapshot> snapshot_;
    shared_ptr<Memo<Snapshot>> snapshotMemo_;
};

#endif // BOARD_H
