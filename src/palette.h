#ifndef PALETTE_H
#define PALETTE_H

#include "def.h"
#include "resmgr.h"
#include <vector>
#include <memory>

class Chess;
class Board;
class QLabel;
class QPixmap;


using def::TPos;
using def::byte;
using std::vector;
using std::shared_ptr;

class Palette
{
public:
    Palette(Chess* chess, Board* board, ResMgr* resMgr);
    ~Palette();

    void click(TPos currPos);

    void open();// 开局
    void rotate();// 翻转棋盘

protected:
    void initLabel();
    void initPieces();

    void drawPieces();
    void drawPiece(TPos pos);

    void drawSelect(TPos currPos);
    byte movePiece(TPos currPos);

private:
    bool init_;

    Chess* chess_;
    Board* board_;
    ResMgr* resMgr_;

    shared_ptr<QLabel> bg_;
    shared_ptr<QLabel> prevSelect_;
    shared_ptr<QLabel> currSelect_;
    vector<vector<shared_ptr<QLabel>>> pieces_;

    TPos prevPos_;
};

#endif // PALETTE_H
