#include "resmgr.h"

#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QMediaPlaylist>
#include <QSound>
#include <QFile>

#include <memory>
#include <string>

ResMgr* ResMgr::getInstance()
{
    static ResMgr mgr;
    return &mgr;
}

ResMgr::ResMgr()
    : initBg_(false)
    , initPieces_(false)
{

}

ResMgr::~ResMgr()
{

}

bool ResMgr::loadPieceSkin(EPieceSkin skin)
{
    static unordered_map<int, string> allPieceSkinInfo =
    {
        {ResMgr::EPS_comic,    ":/pic/piece/comic/res/pic/piece/comic/"},
        {ResMgr::EPS_delicate, ":/pic/piece/delicate/res/pic/piece/delicate/"},
        {ResMgr::EPS_polish,   ":/pic/piece/polish/res/pic/piece/polish/"},
        {ResMgr::EPS_wood,     ":/pic/piece/wood/res/pic/piece/wood/"},
    };

    static unordered_map<int, string> allPieceInfo =
    {
        {def::EP_empty,        "OO.GIF"},

        {def::EP_redKing,      "RK.GIF"},
        {def::EP_redAdvisor,   "RA.GIF"},
        {def::EP_redBishop,    "RB.GIF"},
        {def::EP_redKnight,    "RN.GIF"},
        {def::EP_redRook,      "RR.GIF"},
        {def::EP_redCannon,    "RC.GIF"},
        {def::EP_redPawn,      "RP.GIF"},

        {def::EP_blackKing,    "BK.GIF"},
        {def::EP_blackAdvisor, "BA.GIF"},
        {def::EP_blackBishop,  "BB.GIF"},
        {def::EP_blackKnight,  "BN.GIF"},
        {def::EP_blackRook,    "BR.GIF"},
        {def::EP_blackCannon,  "BC.GIF"},
        {def::EP_blackPawn,    "BP.GIF"},

        {def::EP_select,       "OOS.GIF"},
    };

    unordered_map<int, string>::iterator itr = allPieceSkinInfo.find(skin);
    if (itr == allPieceSkinInfo.end())
        return false;

    //初始化棋子
    for (std::pair<const int, string>& pieceInfo: allPieceInfo)
    {
        def::EPiece pieceType = static_cast<def::EPiece>(pieceInfo.first);

        if (!pieces_[pieceType])
            pieces_[pieceType] = std::make_shared<QPixmap>();

        string path = allPieceSkinInfo[skin] + pieceInfo.second;
        if (!pieces_[pieceType]->load(QString(path.c_str())))
            return false;
    }

    initPieces_ = true;
    return true;
}

bool ResMgr::loadBgSkin(EBgSkin skin)
{
    static unordered_map<int, string> allBgSkinInfo =
    {
        {ResMgr::EBS_canvas,   ":/pic/bg/res/pic/bg/CANVAS.GIF"},
        {ResMgr::EBS_drops,    ":/pic/bg/res/pic/bg/DROPS.GIF"},
        {ResMgr::EBS_green,    ":/pic/bg/res/pic/bg/GREEN.GIF"},
        {ResMgr::EBS_sheet,    ":/pic/bg/res/pic/bg/SHEET.GIF"},
        {ResMgr::EBS_skeleton, ":/pic/bg/res/pic/bg/SKELETON.GIF"},
        {ResMgr::EBS_stone,    ":/pic/bg/res/pic/bg/STONE.GIF"},
        {ResMgr::EBS_wood,     ":/pic/bg/res/pic/bg/WOOD.GIF"}
    };

    unordered_map<int, string>::iterator itr = allBgSkinInfo.find(skin);
    if (itr == allBgSkinInfo.end())
        return false;

    if (!bg_)
        bg_ = std::make_shared<QPixmap>();

    if (!bg_->load(itr->second.c_str()))
        return false;

    initBg_ = true;
    return true;
}

shared_ptr<QPixmap> ResMgr::getBg()
{
    if (!initBg_)
        loadBgSkin(ResMgr::EBS_wood);

    return bg_;
}

shared_ptr<QPixmap> ResMgr::getPiece(def::EPiece piece)
{
    if (!initPieces_)
        loadPieceSkin(ResMgr::EPS_wood);

    return pieces_[piece];
}

void ResMgr::playBgm()
{
    QString path = QCoreApplication::applicationDirPath() + "/pal.mp3";
    if (!QFile(path).exists())// 文件不存在，则直接返回
        return;

    if (!player_)
    {
        player_ = std::make_shared<QMediaPlayer>();

        if (!playlist_)
        {
            playlist_ = std::make_shared<QMediaPlaylist>();
            playlist_->addMedia(QUrl::fromLocalFile(path));
            playlist_->setCurrentIndex(1);
            playlist_->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }

        player_->setPlaylist(playlist_.get());
        player_->setVolume(50);
    }

    player_->play();
}

void ResMgr::stopBgm()
{
    if (player_)
        player_->stop();
}

void ResMgr::playSound(const string& name)
{
    string path = ":/sound/effect/res/sound/effect/" + name;
    QSound::play(path.c_str());
}
