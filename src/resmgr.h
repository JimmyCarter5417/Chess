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

    shared_ptr<QPixmap> getBg();
    shared_ptr<QPixmap> getPiece(def::EPiece piece);

    void playBgm();
    void stopBgm();
    void playSound(const string& name);

private:
    ResMgr();

private:
    unordered_map<int, shared_ptr<QPixmap>> pieces_;
    shared_ptr<QPixmap> bg_;
    shared_ptr<QMediaPlaylist> playlist_;
    shared_ptr<QMediaPlayer> player_;

    bool initBg_;
    bool initPieces_;
};

#endif // RESMGR_H
