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
    , isInitIcons_(false)
{

}

ResMgr::~ResMgr()
{

}

bool ResMgr::loadIconSkin(ICON_SKIN_E skin)
{
    static unordered_map<int, string> allIconSkinInfo =
    {
        {ResMgr::ICON_SKIN_comic,    ":/pic/icon/comic/res/pic/icon/comic/"},
        {ResMgr::ICON_SKIN_delicate, ":/pic/icon/delicate/res/pic/icon/delicate/"},
        {ResMgr::ICON_SKIN_polish,   ":/pic/icon/polish/res/pic/icon/polish/"},
        {ResMgr::ICON_SKIN_wood,     ":/pic/icon/wood/res/pic/icon/wood/"},
    };

    static unordered_map<int, string> allIconInfo =
    {
        {def::ICON_empty,        "OO.GIF"},

        {def::ICON_redKing,      "RK.GIF"},
        {def::ICON_redAdvisor,   "RA.GIF"},
        {def::ICON_redBishop,    "RB.GIF"},
        {def::ICON_redKnight,    "RN.GIF"},
        {def::ICON_redRook,      "RR.GIF"},
        {def::ICON_redCannon,    "RC.GIF"},
        {def::ICON_redPawn,      "RP.GIF"},

        {def::ICON_blackKing,    "BK.GIF"},
        {def::ICON_blackAdvisor, "BA.GIF"},
        {def::ICON_blackBishop,  "BB.GIF"},
        {def::ICON_blackKnight,  "BN.GIF"},
        {def::ICON_blackRook,    "BR.GIF"},
        {def::ICON_blackCannon,  "BC.GIF"},
        {def::ICON_blackPawn,    "BP.GIF"},

        {def::ICON_select,       "OOS.GIF"},
    };

    unordered_map<int, string>::iterator itr = allIconSkinInfo.find(skin);
    if (itr == allIconSkinInfo.end())
        return false;

    //初始化棋子
    for (std::pair<const int, string>& iconInfo: allIconInfo)
    {
        def::ICON_E iconType = static_cast<def::ICON_E>(iconInfo.first);

        if (!icons_[iconType])
            icons_[iconType] = std::make_shared<QPixmap>();

        string path = allIconSkinInfo[skin] + iconInfo.second;
        if (!icons_[iconType]->load(QString(path.c_str())))
            return false;
    }

    isInitIcons_ = true;
    return true;
}

bool ResMgr::loadBgSkin(BG_SKIN_E skin)
{
    static unordered_map<int, string> allBgSkinInfo =
    {
        {ResMgr::BG_SKIN_canvas,   ":/pic/bg/res/pic/bg/CANVAS.GIF"},
        {ResMgr::BG_SKIN_drops,    ":/pic/bg/res/pic/bg/DROPS.GIF"},
        {ResMgr::BG_SKIN_green,    ":/pic/bg/res/pic/bg/GREEN.GIF"},
        {ResMgr::BG_SKIN_sheet,    ":/pic/bg/res/pic/bg/SHEET.GIF"},
        {ResMgr::BG_SKIN_skeleton, ":/pic/bg/res/pic/bg/SKELETON.GIF"},
        {ResMgr::BG_SKIN_stone,    ":/pic/bg/res/pic/bg/STONE.GIF"},
        {ResMgr::BG_SKIN_wood,     ":/pic/bg/res/pic/bg/WOOD.GIF"}
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
        loadBgSkin(ResMgr::BG_SKIN_wood);

    return bg_;
}

shared_ptr<QPixmap> ResMgr::getIcon(def::ICON_E icon)
{
    if (!isInitIcons_)
        loadIconSkin(ResMgr::ICON_SKIN_wood);

    return icons_[icon];
}

void ResMgr::playBgm()
{
    QString path = QCoreApplication::applicationDirPath() + "/bgm.mp3";
    if (!QFile(path).exists())// 文件不存在，则直接返回
        return;

    if (!mediaPlayer_)
    {
        mediaPlayer_ = std::make_shared<QMediaPlayer>();

        if (!mediaPlayList_)
        {
            mediaPlayList_ = std::make_shared<QMediaPlaylist>();
            mediaPlayList_->addMedia(QUrl::fromLocalFile(path));
            mediaPlayList_->setCurrentIndex(1);
            mediaPlayList_->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }

        mediaPlayer_->setPlaylist(mediaPlayList_.get());
        mediaPlayer_->setVolume(50);
    }

    mediaPlayer_->play();
}

void ResMgr::stopBgm()
{
    if (mediaPlayer_)
        mediaPlayer_->stop();
}

void ResMgr::playSound(const string& name)
{
    string path = ":/sound/effect/res/sound/effect/" + name;
    QSound::play(path.c_str());
}
