#ifndef DEF_H
#define DEF_H

#include <stdint.h>

namespace def
{
    struct TDelta
    {
        int8_t deltaRow;
        int8_t deltaCol;

        TDelta(int8_t row = 0, int8_t col = 0);
        bool operator==(const TDelta& rhs) const;
    };

    struct TPos
    {
        int8_t row;
        int8_t col;

        TPos(int8_t row1 = -1, int8_t col1 = -1);
        bool operator==(const TPos& rhs) const;
        bool operator!=(const TPos& rhs) const;
        bool operator()() const;
        TDelta operator-(const TPos& rhs) const;
        TPos operator+(const TDelta& delta) const;
    };

    struct TMove
    {
        TPos src;
        TPos dst;
    };

    struct TClientCo
    {
        int x;
        int y;
    };

    struct TSize
    {
        int width;
        int height;
    };   

    // 红黑双方棋子编号
    enum EPiece
    {
        EP_empty = 0,     // 00000

        EP_redKing = 9,   // 01001
        EP_redAdvisor,    // 01010
        EP_redBishop,     // 01011
        EP_redKnight,     // 01100
        EP_redRook,       // 01101
        EP_redCannon,     // 01110
        EP_redPawn = 15,  // 01111

        EP_blackKing = 17,// 10001
        EP_blackAdvisor,  // 10010
        EP_blackBishop,   // 10011
        EP_blackKnight,   // 10100
        EP_blackRook,     // 10101
        EP_blackCannon,   // 10110
        EP_blackPawn = 23,// 10111

        EP_select = 32    //100000
    };    

    const TPos g_nullPos = {-1, -1};

    //掩码
    const uint8_t g_colorMask = 0x18;// 11000
    const uint8_t g_pieceMask = 0x7; // 00111

    // 颜色标识
    const uint8_t g_redFlag   = 0x8; // 默认下面为红色 01000
    const uint8_t g_blackFlag = 0x10;// 默认上面为黑色 10000
    
    //七种棋子用低三位表示
    const uint8_t g_empty   = 0x0;
    const uint8_t g_king    = 0x1;
    const uint8_t g_advisor = 0x2;
    const uint8_t g_bishop  = 0x3;
    const uint8_t g_knight  = 0x4;
    const uint8_t g_rook    = 0x5;
    const uint8_t g_cannon  = 0x6;
    const uint8_t g_pawn    = 0x7;

    enum EPlayer
    {
        EP_red   = g_redFlag,    // 01000
        EP_black = g_blackFlag,  // 10000
        EP_none  = 0,            // 00000
    };

    void switchPlayer(EPlayer& player);// 切换玩家
    EPlayer getOtherPlayer(EPlayer player);// 对位玩家
}


#endif // DEF_H
