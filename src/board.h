#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "resmgr.h"

using def::TPos;

class Board
{
public:
    static Board* getInstance();

    void init();

    ResMgr::EPiece getPiece(TPos pos);

private:
    Board();

private:
    def::byte board_[256];
};

#endif // BOARD_H
