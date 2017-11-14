#ifndef DEF_H
#define DEF_H

namespace def
{
    typedef unsigned char byte;
    typedef unsigned int uint;

    struct TDelta
    {
        int deltaRow;
        int deltaCol;

        TDelta(int row = 0, int col = 0);
        bool operator==(const TDelta& rhs) const;
    };

    struct TPos
    {
        int row;
        int col;

        TPos(int row1 = -1, int col1 = -1);
        bool operator==(const TPos& rhs) const;
        bool operator!=(const TPos& rhs) const;
        bool operator()() const;
        TDelta operator-(const TPos& rhs) const;
        TPos operator+(const TDelta& delta) const;
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

    enum EPlayer
    {
        EP_up,
        EP_down,
        EP_none,
    };


    void switchPlayer(EPlayer& player);// 切换玩家
    EPlayer getOtherPlayer(EPlayer player);// 对位玩家

    // 红黑双方棋子编号
    enum EPiece
    {
        EP_empty = 0,

        EP_redKing = 9,
        EP_redAdvisor,
        EP_redBishop,
        EP_redKnight,
        EP_redRook,
        EP_redCannon,
        EP_redPawn = 15,

        EP_blackKing = 17,
        EP_blackAdvisor,
        EP_blackBishop,
        EP_blackKnight,
        EP_blackRook,
        EP_blackCannon,
        EP_blackPawn = 23,

        EP_select = 32
    };

    const TPos g_nullPos = {-1, -1};

    //掩码
    const byte g_scopeMask = 0x18;
    const byte g_pieceMask = 0x7;

    // 颜色标识
    const byte g_redFlag   = 0x8;// 默认下面为红色
    const byte g_blackFlag = 0x10;// 默认上面为黑色
    
    //七种棋子用低三位表示
    const byte g_empty   = 0x0;
    const byte g_king    = 0x1;
    const byte g_advisor = 0x2;
    const byte g_bishop  = 0x3;
    const byte g_knight  = 0x4;
    const byte g_rook    = 0x5;
    const byte g_cannon  = 0x6;
    const byte g_pawn    = 0x7;
}


#endif // DEF_H
