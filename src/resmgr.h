#ifndef RESMGR_H
#define RESMGR_H

#include "util/def.h"

#include <unordered_map>
#include <memory>
#include <string>

using std::string;
using std::shared_ptr;
using std::unordered_map;

class QMediaPlaylist;
class QMediaPlayer;
class QPixmap;

class ResMgr
{
public:    
    enum ICON_SKIN_E
    {
        ICON_SKIN_comic,
        ICON_SKIN_delicate,
        ICON_SKIN_polish,
        ICON_SKIN_wood,

        ICON_SKIN_end
    };

    enum BG_SKIN_E
    {
        BG_SKIN_canvas,
        BG_SKIN_drops,
        BG_SKIN_green,
        BG_SKIN_sheet,
        BG_SKIN_skeleton,
        BG_SKIN_stone,
        BG_SKIN_wood,

        BG_SKIN_end
    };

    static ResMgr* getInstance();
    ~ResMgr();

    bool loadIconSkin(ICON_SKIN_E skin);
    bool loadBgSkin(BG_SKIN_E skin);

    shared_ptr<QPixmap> getBg();
    shared_ptr<QPixmap> getIcon(def::ICON_E icon);

    void playBgm();
    void stopBgm();
    void playSound(const string& name);

private:
    ResMgr();

private:
    unordered_map<int, shared_ptr<QPixmap>> icons_;
    shared_ptr<QPixmap> bg_;
    shared_ptr<QMediaPlaylist> mediaPlayList_;
    shared_ptr<QMediaPlayer> mediaPlayer_;

    bool initBg_;
    bool isInitIcons_;
};

#endif // RESMGR_H
