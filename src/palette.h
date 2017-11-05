#ifndef PALETTE_H
#define PALETTE_H

#include "def.h"
#include <vector>

class Chess;
class Board;
class ResMgr;
class QLabel;
class QPixmap;

using def::TPos;
using std::vector;

class Palette
{
public:
    explicit Palette(Chess* chess, Board* board, ResMgr* resMgr);
    ~Palette();

    void drawBg();
    void drawPieces();
    void drawPiece(TPos pos);

    void drawSelect(TPos pos);

private:
    Chess* chess_;
    Board* board_;
    ResMgr* resMgr_;

    QLabel* bg_;
    QLabel* select_;
    vector<vector<QLabel*>> pieces_;
};

#endif // PALETTE_H
