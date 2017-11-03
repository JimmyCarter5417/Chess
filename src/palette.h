#ifndef PALETTE_H
#define PALETTE_H

#include "def.h"
#include <vector>

class Chess;
class QLabel;
class QPixmap;

using std::vector;
using def::TPos;
using def::TSize;

class Palette
{
public:
    explicit Palette(Chess* chess);
    ~Palette();

private:
    Chess* chess_;

    TSize pieceSize_;
    TSize boardSize_;

    vector<vector<QLabel*>> palette_;
};

#endif // PALETTE_H
