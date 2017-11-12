#ifndef RESMGR_H
#define RESMGR_H

#include <unordered_map>

class QPixmap;

class ResMgr
{
public:
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

    enum EPieceSkin
    {
        EPS_comic,
        EPS_delicate,
        EPS_polish,
        EPS_wood,

        EPS_end
    };

    enum EBgSkin
    {
        EBS_canvas,
        EBS_drops,
        EBS_green,
        EBS_sheet,
        EBS_skeleton,
        EBS_stone,
        EBS_wood,

        EBS_end
    };

    static ResMgr* getInstance();
    ~ResMgr();

    bool loadPieceSkin(EPieceSkin skin);
    bool loadBgSkin(EBgSkin skin);

    QPixmap* getBg();
    QPixmap* getPiece(EPiece piece);

private:
    ResMgr();

private:
    std::unordered_map<int, QPixmap*> pieces_;
    QPixmap* bg_;

    bool initBg_;
    bool initPieces_;
};

#endif // RESMGR_H
