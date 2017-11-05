#include "palette.h"
#include "chess.h"
#include "board.h"
#include "resmgr.h"
#include "co.h"
#include "size.h"
#include <assert.h>
#include <QLabel>

Palette::Palette(Chess* chess, Board* board, ResMgr* resMgr)
    : chess_(chess)
    , board_(board)
    , resMgr_(resMgr)
    , bg_(nullptr)
    , select_(nullptr)
{
    assert(chess_ != nullptr);
    assert(board_ != nullptr);
    assert(resMgr_ != nullptr);

    pieces_.assign(co::g_row, vector<QLabel*>(co::g_col, nullptr));
}

Palette::~Palette()
{
//    for (QLabel*& label: pieces_)
//    {
//        delete label;
//        label = nullptr;
//    }
}

void Palette::drawBg()
{
    if (bg_ == nullptr)
    {
        bg_ = new QLabel(chess_);
    }

    bg_->resize(size::g_boardSize.width, size::g_boardSize.height);
    bg_->setPixmap(*resMgr_->getBoard());
}

void Palette::drawPieces()
{
    for (int i = 0; i < co::g_row; i++)
    {
        for (int j = 0; j < co::g_col; j++)
        {
            drawPiece({i, j});
        }
    }
}

void Palette::drawPiece(TPos pos)
{
    if (!co::isValid2d(pos))
        return;

    ResMgr::EPiece pieceType = board_->getPiece(pos);
    if (pieceType != ResMgr::EP_Empty)
    {
        if (pieces_[pos.x][pos.y] == nullptr)
            pieces_[pos.x][pos.y] = new QLabel(chess_);

        pieces_[pos.x][pos.y]->resize(size::g_pieceSize.width, size::g_pieceSize.height);
        pieces_[pos.x][pos.y]->setPixmap(*resMgr_->getPiece(pieceType));

        int x = size::g_boardLeftTopPos.y + size::g_pieceSize.height * pos.y - std::floor(size::g_pieceSize.height / 2.0);
        int y = size::g_boardLeftTopPos.x + size::g_pieceSize.width * pos.x - std::floor(size::g_pieceSize.width / 2.0);
        pieces_[pos.x][pos.y]->move(x, y);
    }
}

void Palette::drawSelect(TPos pos)
{
    if (!co::isValid2d(pos))
        return;

    if (select_ == nullptr)
    {
        select_ = new QLabel(chess_);
    }

    select_->resize(size::g_pieceSize.width, size::g_pieceSize.height);
    select_->setPixmap(*resMgr_->getPiece(ResMgr::EP_Select));

    int x = size::g_boardLeftTopPos.y + size::g_pieceSize.height * pos.y - std::floor(size::g_pieceSize.height / 2.0);
    int y = size::g_boardLeftTopPos.x + size::g_pieceSize.width * pos.x - std::floor(size::g_pieceSize.width / 2.0);
    select_->move(x, y);
}
