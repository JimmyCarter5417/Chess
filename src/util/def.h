#ifndef DEF_H
#define DEF_H

#include <stdint.h>

namespace def
{
    // 两个位置的偏移量
    struct TDelta
    {
        int8_t deltaRow;
        int8_t deltaCol;

        TDelta(int8_t row = 0, int8_t col = 0);
        bool operator==(const TDelta& rhs) const;
    };

    // 逻辑位置
    struct TPos
    {
        int8_t row;
        int8_t col;

        TPos(int8_t r, int8_t c);
        TPos(const TPos& other);
        TPos& operator=(const TPos& rhs);
        bool operator==(const TPos& rhs) const;
        bool operator!=(const TPos& rhs) const;
        bool operator()() const;
        TDelta operator-(const TPos& rhs) const;
        TPos operator+(const TDelta& delta) const;
    };

    const TPos INVALID_POS = {-1, -1};  // 无效位置

    // 走法，src -> dst
    struct TMove
    {
        TPos src;
        TPos dst;

        TMove(TPos s, TPos d);
        TMove& operator=(const TMove& rhs);
        TMove(const TMove& other);
    };

    // 屏幕坐标
    struct TClientCo
    {
        int x;
        int y;
    };

    // 控件的屏幕尺寸
    struct TSize
    {
        int width;
        int height;
    };   

    // 各类图标编号
    enum ICON_E
    {
        ICON_empty = 0,         //  00000
        ICON_redKing = 9,       //  01001
        ICON_redAdvisor,        //  01010
        ICON_redBishop,         //  01011
        ICON_redKnight,         //  01100
        ICON_redRook,           //  01101
        ICON_redCannon,         //  01110
        ICON_redPawn = 15,      //  01111
        ICON_blackKing = 17,    //  10001
        ICON_blackAdvisor,      //  10010
        ICON_blackBishop,       //  10011
        ICON_blackKnight,       //  10100
        ICON_blackRook,         //  10101
        ICON_blackCannon,       //  10110
        ICON_blackPawn = 23,    //  10111
        ICON_select = 32        // 100000
    };

    // 棋子类型
    enum PIECE_E
    {
        PIECE_empty   = 0x0,
        PIECE_king    = 0x1,
        PIECE_advisor = 0x2,
        PIECE_bishop  = 0x3,
        PIECE_knight  = 0x4,
        PIECE_rook    = 0x5,
        PIECE_cannon  = 0x6,
        PIECE_pawn    = 0x7,
    };

    // 玩家类型，默认下方为红，上方为黑
    enum PLAYER_E
    {
        PLAYER_none  = 0x00,    // 00000
        PLAYER_red   = 0x08,    // 01000
        PLAYER_black = 0x10,    // 10000
    };

    const uint8_t PIECE_MASK  = 0x07;      // 1、2、3位为棋子掩码，00111
    const uint8_t PLAYER_MASK = 0x18;      // 4、5位为玩家掩码，11000

    void switchPlayer(PLAYER_E& player);       // 切换玩家
    PLAYER_E getEnemyPlayer(PLAYER_E player);  // 对位玩家

    PLAYER_E getOwner(ICON_E icon);
    PIECE_E getPiece(ICON_E icon);
}


#endif // DEF_H
