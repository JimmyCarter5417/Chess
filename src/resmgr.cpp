#include "resmgr.h"
#include <QString>
#include <QPixmap>
#include <memory>
#include <string>

using namespace std;

ResMgr* ResMgr::getInstance()
{
    static ResMgr mgr;
    return &mgr;
}

ResMgr::ResMgr()
    : bg_(nullptr)
    , initBg_(false)
    , initPieces_(false)
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
    static unordered_map<int, string> allPieceSkinInfo =
    {
        {EPS_Delicate, ":/piece/delicate/res/piece/deicate/"},
        {EPS_Polish,   ":/piece/polish/res/piece/polish/"},
        {EPS_Wood,     ":/piece/wood/res/piece/wood/"},
    };

    static unordered_map<int, string> allPieceInfo =
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

    unordered_map<int, string>::iterator itr = allPieceSkinInfo.find(skin);
    if (itr == allPieceSkinInfo.end())
        return false;

    //初始化棋子
    for (pair<const int, string>& pieceInfo: allPieceInfo)
    {
        EPiece pieceType = static_cast<EPiece>(pieceInfo.first);

        if (pieces_[pieceType] == nullptr)
            pieces_[pieceType] = new QPixmap;

        string path = allPieceSkinInfo[skin] + pieceInfo.second;
        if (!pieces_[pieceType]->load(QString(path.c_str())))
            return false;
    }

    initBg_ = true;
    return true;
}

bool ResMgr::loadBg(EBgSkin skin)
{
    static unordered_map<int, string> allBgSkinInfo =
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

    unordered_map<int, string>::iterator itr = allBgSkinInfo.find(skin);
    if (itr == allBgSkinInfo.end())
        return false;

    if (bg_ == nullptr)
        bg_ = new QPixmap;

    if (!bg_->load(itr->second.c_str()))
        return false;

    initBg_ = true;
    return true;
}

QPixmap* ResMgr::getBg()
{
    if (!initBg_)
        loadBg(ResMgr::EBS_Wood);

    return bg_;
}

QPixmap* ResMgr::getPiece(EPiece piece)
{
    if (!initPieces_)
        loadPieces(ResMgr::EPS_Wood);

    return pieces_[piece];
}
