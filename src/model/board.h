#ifndef BOARD_H
#define BOARD_H

#include "../def.h"
#include "../co.h"
#include "../resmgr.h"
#include "memo.h"

#include <vector>
#include <unordered_set>
#include <memory>

using def::TPos;
using def::TDelta;
using def::byte;
using std::vector;
using std::unordered_set;
using std::shared_ptr;

class Board
{
public:
    // 玩家棋子位置集合
    struct TPieceSet
    {
        TPos king;
        unordered_set<TPos> defenders;
        unordered_set<TPos> attackers;

        void swap(TPieceSet& another)
        {
            std::swap(king, another.king);
            defenders.swap(another.defenders);
            attackers.swap(another.attackers);
        }
    };

    // 当前局面快照
    class Snapshot
    {
        friend class Board;

        vector<vector<byte>> board_;// 棋盘
        def::EPlayer player_;// 下一步要走棋的玩家

        TPieceSet upPieceSet_;// 上方玩家棋子集合
        TPieceSet downPieceSet_;// 下方玩家棋子集合

        // 第四位和第五位表示上下部分
        byte upFlag_;// 默认上面为黑色
        byte downFlag_;// 默认下面为红色

        std::pair<TPos, TPos> trigger_;// 表示该snapshot是由trigger的两个位置移动产生的，用于绘制select图标

    public:
        Snapshot()
        {
            static const vector<vector<byte>> initialBoard = {
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
            vector<vector<byte>> tmp(co::g_rowNum, vector<byte>(co::g_colNum, 0));
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

    enum EMoveRet
    {
        EMR_null = 0,
        EMR_ok = 0x10,
        EMR_suicide = 0x01,
        EMR_check = 0x02,
        EMR_dead = 0x04,
        EMR_eat = 0x08,
    };

    Board();

    void init();    

    byte movePiece(TPos prevPos, TPos currPos);
    bool rotate();
    bool undo();

    ResMgr::EPiece getPiece(TPos pos) const;
    def::EPlayer getNextPlayer() const;
    def::EPlayer getPieceOwner(TPos pos) const;

    std::pair<TPos, TPos> getTrigger() const;

protected:
    // 对于player来说，从prevPos到currPos是否合法
    bool isValidMove(TPos prevPos, TPos currPos, def::EPlayer player) const;

    typedef bool (Board::*PosFunc)(TPos pos, def::EPlayer player) const;// 判断位置是否合法
    typedef bool (Board::*DeltaFunc)(TDelta delta, def::EPlayer player) const;// 判断偏移量是否合法
    typedef bool (Board::*RuleFunc)(TPos prevPos, TPos currPos) const;// 棋子特定的规则
    bool isValidMove(TPos prevPos, TPos currPos, def::EPlayer player, PosFunc isValidPos, DeltaFunc isValidDelta, RuleFunc isValidRule) const;

    bool isValidKingPos(TPos pos, def::EPlayer player) const;
    bool isValidAdvisorPos(TPos pos, def::EPlayer player) const;
    bool isValidBishopPos(TPos pos,def:: EPlayer player) const;
    bool isValidKnightPos(TPos pos, def::EPlayer player) const;
    bool isValidRookPos(TPos pos, def::EPlayer player) const;
    bool isValidCannonPos(TPos pos, def::EPlayer player) const;
    bool isValidPawnPos(TPos pos, def::EPlayer player) const;

    bool isValidKingDelta(TDelta delta, def::EPlayer player) const;
    bool isValidAdvisorDelta(TDelta delta, def::EPlayer player) const;
    bool isValidBishopDelta(TDelta delta, def::EPlayer player) const;
    bool isValidKnightDelta(TDelta delta, def::EPlayer player) const;
    bool isValidRookDelta(TDelta delta, def::EPlayer player) const;
    bool isValidCannonDelta(TDelta delta, def::EPlayer player) const;
    bool isValidPawnDelta(TDelta delta, def::EPlayer player) const;

    // 不再检查pos及delta，默认前面已检查
    bool isValidKingRule(TPos prevPos, TPos currPos) const;
    bool isValidAdvisorRule(TPos prevPos, TPos currPos) const;
    bool isValidBishopRule(TPos prevPos, TPos currPos) const;
    bool isValidKnightRule(TPos prevPos, TPos currPos) const;
    bool isValidRookRule(TPos prevPos, TPos currPos) const;
    bool isValidCannonRule(TPos prevPos, TPos currPos) const;
    bool isValidPawnRule(TPos prevPos, TPos currPos) const;

    bool isValidKingMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidAdvisorMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidBishopMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidKnightMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidRookMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidCannonMove(TPos prevPos, TPos currPos, def::EPlayer player) const;
    bool isValidPawnMove(TPos prevPos, TPos currPos, def::EPlayer player) const;

    const unordered_set<TDelta>& getValidKingDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidAdvisorDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidBishopDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidKnightDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidRookDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidCannonDelta(def::EPlayer player) const;
    const unordered_set<TDelta>& getValidPawnDelta(def::EPlayer player) const;

    const unordered_map<TPos, int>& getKingValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getAdvisorValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getBishopValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getKnightValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getRookValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getCannonValue(def::EPlayer player) const;
    const unordered_map<TPos, int>& getPawnValue(def::EPlayer player) const;

    bool check(def::EPlayer player);
    bool checkmate(def::EPlayer player);

    bool isSuicide(TPos prevPos, TPos currPos, def::EPlayer player);
    bool updatePieceSet(TPos prevPos, TPos currPos, def::EPlayer player);

    inline bool isPiece(TPos pos, int piece) const;// 判断pos位置是否为特定棋子（不分颜色）
    inline bool isKing(TPos pos) const;
    inline bool isDefensivePiece(TPos pos) const;
    inline bool isAttactivePiece(TPos pos) const;

    bool isKingMeeting() const;

    shared_ptr<Snapshot> createSnapshot();
    bool saveSnapshot();
    bool loadSnapshot(shared_ptr<Snapshot> snapshot);
    bool updateSnapshot(TPos prevPos, TPos currPos, def::EPlayer player);

private:
    shared_ptr<Snapshot> snapshot_;
    shared_ptr<Memo<Snapshot>> snapshotMemo_;
};

#endif // BOARD_H
