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

    pieces_.assign(co::g_rowNum, vector<QLabel*>(co::g_colNum, nullptr));
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
    bg_->setPixmap(*resMgr_->getBg());
}

void Palette::drawPieces()
{
    for (int i = 0; i < co::g_rowNum; i++)
    {
        for (int j = 0; j < co::g_colNum; j++)
        {
            drawPiece({i, j});
        }
    }
}

void Palette::drawPiece(TPos pos)
{
    if (!co::isValidPos(pos))
        return;

    ResMgr::EPiece pieceType = board_->getPiece(pos);
    if (pieceType != ResMgr::EP_Empty)
    {
        if (pieces_[pos.row][pos.col] == nullptr)
            pieces_[pos.row][pos.col] = new QLabel(chess_);

        pieces_[pos.row][pos.col]->resize(size::g_pieceSize.width, size::g_pieceSize.height);
        if (QPixmap* pic = resMgr_->getPiece(pieceType))
            pieces_[pos.row][pos.col]->setPixmap(*pic);

        TClientCo clientCo;
        co::pos2ClientCo(pos, clientCo);
        pieces_[pos.row][pos.col]->move(clientCo.x, clientCo.y);
    }
}

void Palette::drawSelect(TPos pos)
{
    if (co::isValidPos(pos))
    {
        if (select_ == nullptr)
        {
            select_ = new QLabel(chess_);

            select_->resize(size::g_pieceSize.width, size::g_pieceSize.height);
            if (QPixmap* pic = resMgr_->getPiece(ResMgr::EP_Select))
                select_->setPixmap(*pic);
        }

        TClientCo clientCo;
        co::pos2ClientCo(pos, clientCo);
        select_->move(clientCo.x, clientCo.y);
    }
}

void Palette::move(TPos curPos, TPos newPos)
{
    if (pieces_[curPos.row][curPos.col] != nullptr)
    {
        std::swap(pieces_[curPos.row][curPos.col], pieces_[newPos.row][newPos.col]);

        TClientCo clientCo;
        co::pos2ClientCo(newPos, clientCo);
        pieces_[newPos.row][newPos.col]->move(clientCo.x, clientCo.y);
    }
}
