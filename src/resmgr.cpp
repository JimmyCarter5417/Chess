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

using namespace std;

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
        {EPS_comic,    ":/pic/piece/comic/res/pic/piece/comic/"},
        {EPS_delicate, ":/pic/piece/delicate/res/pic/piece/delicate/"},
        {EPS_polish,   ":/pic/piece/polish/res/pic/piece/polish/"},
        {EPS_wood,     ":/pic/piece/wood/res/pic/piece/wood/"},
    };

    static unordered_map<int, string> allPieceInfo =
    {
        {EP_empty,        "OO.GIF"},

        {EP_redKing,      "RK.GIF"},
        {EP_redAdvisor,   "RA.GIF"},
        {EP_redBishop,    "RB.GIF"},
        {EP_redKnight,    "RN.GIF"},
        {EP_redRook,      "RR.GIF"},
        {EP_redCannon,    "RC.GIF"},
        {EP_redPawn,      "RP.GIF"},

        {EP_blackKing,    "BK.GIF"},
        {EP_blackAdvisor, "BA.GIF"},
        {EP_blackBishop,  "BB.GIF"},
        {EP_blackKnight,  "BN.GIF"},
        {EP_blackRook,    "BR.GIF"},
        {EP_blackCannon,  "BC.GIF"},
        {EP_blackPawn,    "BP.GIF"},

        {EP_select,       "OOS.GIF"},
    };

    unordered_map<int, string>::iterator itr = allPieceSkinInfo.find(skin);
    if (itr == allPieceSkinInfo.end())
        return false;

    //初始化棋子
    for (pair<const int, string>& pieceInfo: allPieceInfo)
    {
        EPiece pieceType = static_cast<EPiece>(pieceInfo.first);

        if (pieces_[pieceType].get() == nullptr)
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
        {EBS_canvas,   ":/pic/bg/res/pic/bg/CANVAS.GIF"},
        {EBS_drops,    ":/pic/bg/res/pic/bg/DROPS.GIF"},
        {EBS_green,    ":/pic/bg/res/pic/bg/GREEN.GIF"},
        {EBS_sheet,    ":/pic/bg/res/pic/bg/SHEET.GIF"},
        {EBS_skeleton, ":/pic/bg/res/pic/bg/SKELETON.GIF"},
        {EBS_stone,    ":/pic/bg/res/pic/bg/STONE.GIF"},
        {EBS_wood,     ":/pic/bg/res/pic/bg/WOOD.GIF"}
    };

    unordered_map<int, string>::iterator itr = allBgSkinInfo.find(skin);
    if (itr == allBgSkinInfo.end())
        return false;

    if (bg_.get() == nullptr)
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

shared_ptr<QPixmap> ResMgr::getPiece(EPiece piece)
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

    if (player_.get() == nullptr)
    {
        player_ = make_shared<QMediaPlayer>();

        if (playlist_.get() == nullptr)
        {
            playlist_ = make_shared<QMediaPlaylist>();
            playlist_->addMedia(QUrl::fromLocalFile(path));
            playlist_->setCurrentIndex(0);
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
