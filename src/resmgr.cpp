#include "resmgr.h"
#include <QString>
#include <QPixmap>

ResMgr* ResMgr::getInstance()
{
    static ResMgr mgr;
    return &mgr;
}

ResMgr::ResMgr()
    :bg_(nullptr)
{

}

ResMgr::~ResMgr()
{
    if (bg_)
    {
        delete bg_;
        bg_ = nullptr;
    }

    for (auto& elem: pieces_)
    {
        if (elem.second != nullptr)
        {
            delete elem.second;
            elem.second = nullptr;
        }
    }
}

bool ResMgr::loadPieces(EPieceSkin skin)
{
    struct TPieceSkinInfo
    {
        EPieceSkin skin;
        const char* path;
    };

    struct TPieceInfo
    {
        EPiece piece;
        const char* name;
    };

    static TPieceSkinInfo allPieceSkinInfo[] =
    {
        {EPS_Delicate, ":/piece/delicate/res/piece/deicate/"},
        {EPS_Polish,   ":/piece/polish/res/piece/polish/"},
        {EPS_Wood,     ":/piece/wood/res/piece/wood/"},
    };

    static TPieceInfo allPieceInfo[] =
    {
        {EP_Empty,        "OO.GIF"},

        {EP_RedKing,      "RK.GIF"},
        {EP_RedAdvisor,   "RA.GIF"},
        {EP_RedBishop,    "RB.GIF"},
        {EP_RedKnight,    "RN.GIF"},
        {EP_RedRook,      "RR.GIF"},
        {EP_RedCannon,    "RC.GIF"},
        {EP_RedPawn,      "RP.GIF"},

        {EP_BlackKing,    "BK.GIF"},
        {EP_BlackAdvisor, "BA.GIF"},
        {EP_BlackBishop,  "BB.GIF"},
        {EP_BlackKnight,  "BN.GIF"},
        {EP_BlackRook,    "BR.GIF"},
        {EP_BlackCannon,  "BC.GIF"},
        {EP_BlackPawn,    "BP.GIF"},

        {EP_Select,       "OOS.GIF"},
    };

    QString strPrefix;//get prefix first
    for (int i = 0; i < sizeof(allPieceSkinInfo) / sizeof(TPieceSkinInfo); i++)
    {
        if (allPieceSkinInfo[i].skin == i)
        {
            strPrefix = allPieceSkinInfo[skin].path;
            break;
        }
    }

    if (strPrefix.isEmpty())
    {
        return false;
    }

    //初始化棋子
    for (int i = 0; i < sizeof(allPieceInfo) / sizeof(TPieceInfo); i++)
    {
        EPiece piece = allPieceInfo[i].piece;

        if (pieces_[piece] == nullptr)
        {
            pieces_[piece] = new QPixmap;
        }

        if (!pieces_[piece]->load(strPrefix + allPieceInfo[i].name))
        {
            return false;
        }
    }

    return true;
}

bool ResMgr::loadBg(EBgSkin skin)
{
    struct TBgSkinInfo
    {
        EBgSkin skin;
        const char* path;
    };

    static TBgSkinInfo allBgSkinInfo[] =
    {
        {EBS_Canvas,   ":/bg/res/bg/CANVAS.GIF"},
        {EBS_Drops,    ":/bg/res/bg/DROPS.GIF"},
        {EBS_Green,    ":/bg/res/bg/GREEN.GIF"},
        {EBS_Qianhong, ":/bg/res/bg/QIANHONG.GIF"},
        {EBS_Sheet,    ":/bg/res/bg/SHEET.GIF"},
        {EBS_Skeleton, ":/bg/res/bg/SKELETON.GIF"},
        {EBS_White,    ":/bg/res/bg/WHITE.GIF"},
        {EBS_Wood,     ":/bg/res/bg/WOOD.GIF"}
    };

    bool ret = false;

    for (int i = 0; i < sizeof(allBgSkinInfo) / sizeof(TBgSkinInfo); i++)
    {
        if (allBgSkinInfo[i].skin == i)
        {
            if (bg_ == nullptr)
            {
                bg_ = new QPixmap;
            }

            ret = bg_->load(allBgSkinInfo[skin].path);
            break;
        }
    }

    return ret;
}

QPixmap* ResMgr::getBg()
{
    return bg_;
}

QPixmap* ResMgr::getPiece(EPiece piece)
{
    return pieces_[piece];
}
