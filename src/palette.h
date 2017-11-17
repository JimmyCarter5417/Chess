#ifndef PALETTE_H
#define PALETTE_H

#include "util/def.h"
#include "resmgr.h"
#include "model/model.h"

#include <vector>
#include <memory>

class Chess;
class QLabel;
class QPixmap;
class QMediaPlaylist;
class QMediaPlayer;

using def::TPos;
using def::int8;
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

    void loadBgSkin(ResMgr::EBgSkin skin);
    void loadPieceSkin(ResMgr::EPieceSkin skin);

protected:
    void initLabel();
    void initPieces();

    void drawBg();
    void drawPieces();
    void drawPiece(TPos pos);

    void drawSelect(TMove move);
    int8 makeMove(TMove move);

private:
    bool soundEffect_;
    bool rotate_;

    Chess* chess_;   
    ResMgr* resMgr_;
    shared_ptr<model::IModel> board_;

    //shared_ptr<QLabel> bg_;
    QLabel* bg_;
    shared_ptr<QLabel> prevSelect_;
    shared_ptr<QLabel> currSelect_;
    vector<vector<shared_ptr<QLabel>>> pieces_;

    TPos prevPos_;

    shared_ptr<QMediaPlaylist> playlist_;
    shared_ptr<QMediaPlayer> player_;
};

#endif // PALETTE_H
