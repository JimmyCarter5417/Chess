#ifndef PALETTE_H
#define PALETTE_H

#include "util/def.h"
#include "resmgr.h"
#include "board/board.h"

#include <vector>
#include <memory>

class Chess;
class QLabel;
class QPixmap;
class QMediaPlaylist;
class QMediaPlayer;

using def::TPos;
using def::TMove;
using std::vector;
using std::shared_ptr;

class Palette
{
public:
    Palette(Chess* chess, QLabel* bg, ResMgr* resMgr);
    ~Palette();

    void click(TPos currPos);

    void open();// 开局
    void rotate(bool on);// 翻转棋盘
    void undo();// 悔棋
    void soundEffect(bool on);
    void bgm(bool on);
    void run();

    void loadBgSkin(ResMgr::BG_SKIN_E skin);
    void loadIconSkin(ResMgr::ICON_SKIN_E skin);

protected:
    void initLabels();
    void initIcons();

    void drawBg();
    void drawIcons();
    void drawIcon(TPos pos);

    void drawSelect(TMove move);
    uint8_t makeMove(TMove move);

private:
    bool soundEffect_;
    bool rotate_;

    Chess* chess_;   
    ResMgr* resMgr_;
    shared_ptr<board::IBoard> board_;

    QLabel* bg_;
    shared_ptr<QLabel> prevSelect_;
    shared_ptr<QLabel> currSelect_;
    vector<vector<shared_ptr<QLabel>>> icons_;

    TPos prevPos_;

    shared_ptr<QMediaPlaylist> mediaPlayList_;
    shared_ptr<QMediaPlayer> mediaPlayer_;
};

#endif // PALETTE_H
