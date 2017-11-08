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
using std::vector;
using std::shared_ptr;

class Palette
{
public:
    Palette(Chess* chess, Board* board, ResMgr* resMgr);
    ~Palette();

    ResMgr::EPiece getPiece(TPos pos) const;
    void open();// 开局
    void drawSelect(TPos prevPos, TPos currPos);
    bool movePiece(TPos curPos, TPos newPos);

protected:
    void initLabel();
    void initPieces();

    void drawPieces();
    void drawPiece(TPos pos);

private:
    bool init_;

    Chess* chess_;
    Board* board_;
    ResMgr* resMgr_;

    shared_ptr<QLabel> bg_;
    shared_ptr<QLabel> prevSelect_;
    shared_ptr<QLabel> currSelect_;
    vector<vector<shared_ptr<QLabel>>> pieces_;
};

#endif // PALETTE_H
