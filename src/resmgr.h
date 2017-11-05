#ifndef RESMGR_H
#define RESMGR_H

#include <unordered_map>

class QPixmap;

class ResMgr
{
public:
    enum EPiece
    {
        EP_Empty = 0,

        EP_RedKing = 8,
        EP_RedAdvisor,
        EP_RedBishop,
        EP_RedKnight,
        EP_RedRook,
        EP_RedCannon,
        EP_RedPawn = 14,

        EP_BlackKing = 16,
        EP_BlackAdvisor,
        EP_BlackBishop,
        EP_BlackKnight,
        EP_BlackRook,
        EP_BlackCannon,
        EP_BlackPawn = 22,

        EP_Select = 32
    };

    enum EPieceSkin
    {
        EPS_Delicate,
        EPS_Polish,
        EPS_Wood,

        EPS_End
    };

    enum EBoardSkin
    {
        EBS_Canvas,
        EBS_Drops,
        EBS_Green,
        EBS_Qianhong,
        EBS_Sheet,
        EBS_Skeleton,
        EBS_White,
        EBS_Wood,

        EBS_End
    };

    static ResMgr* getInstance();

    ~ResMgr();

    bool loadPieces(EPieceSkin skin);
    bool loadBoard(EBoardSkin skin);

    QPixmap* getBoard();
    QPixmap* getPiece(EPiece piece);

private:
    ResMgr();

    std::unordered_map<int, QPixmap*> pieces_;
    QPixmap* board_;
};

#endif // RESMGR_H
