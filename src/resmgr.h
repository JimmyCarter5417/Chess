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

        EP_RedKing = 9,
        EP_RedAdvisor,
        EP_RedBishop,
        EP_RedKnight,
        EP_RedRook,
        EP_RedCannon,
        EP_RedPawn = 15,

        EP_BlackKing = 17,
        EP_BlackAdvisor,
        EP_BlackBishop,
        EP_BlackKnight,
        EP_BlackRook,
        EP_BlackCannon,
        EP_BlackPawn = 23,

        EP_Select = 32
    };

    enum EPieceSkin
    {
        EPS_Delicate,
        EPS_Polish,
        EPS_Wood,

        EPS_End
    };

    enum EBgSkin
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
    bool loadBg(EBgSkin skin);

    QPixmap* getBg();
    QPixmap* getPiece(EPiece piece);

private:
    ResMgr();

    std::unordered_map<int, QPixmap*> pieces_;
    QPixmap* bg_;
};

#endif // RESMGR_H
