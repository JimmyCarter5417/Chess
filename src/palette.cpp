#include "palette.h"
#include "chess.h"
#include "board.h"
#include "resmgr.h"
#include "co.h"
#include "size.h"
#include <assert.h>
#include <QLabel>

using namespace std;

Palette::Palette(Chess* chess, Board* board, ResMgr* resMgr)
    : chess_(chess)
    , board_(board)
    , resMgr_(resMgr)   
{
    assert(chess_ != nullptr);
    assert(board_ != nullptr);
    assert(resMgr_ != nullptr);    
}

Palette::~Palette()
{

}

ResMgr::EPiece Palette::getPiece(TPos pos) const
{
    return board_->getPiece(pos);
}

// 初始化背景图片、两个选择图标
void Palette::initLabel()
{
    bg_ = make_shared<QLabel>(chess_);
    bg_->resize(size::g_boardSize.width, size::g_boardSize.height);
    if (QPixmap* pic = resMgr_->getBg())
        bg_->setPixmap(*pic);

    prevSelect_ = make_shared<QLabel>(chess_);
    prevSelect_->resize(size::g_pieceSize.width, size::g_pieceSize.height);
    if (QPixmap* pic = resMgr_->getPiece(ResMgr::EP_Select))
        prevSelect_->setPixmap(*pic);

    currSelect_ = make_shared<QLabel>(chess_);
    currSelect_->resize(size::g_pieceSize.width, size::g_pieceSize.height);
    if (QPixmap* pic = resMgr_->getPiece(ResMgr::EP_Select))
        currSelect_->setPixmap(*pic);
}

// 每一个位置一个label
void Palette::initPieces()
{
    pieces_.assign(co::g_rowNum, vector<shared_ptr<QLabel>>(co::g_colNum));

    for (int i = 0; i < co::g_rowNum; i++)
    {
        for (int j = 0; j < co::g_colNum; j++)
        {
            pieces_[i][j] = make_shared<QLabel>(chess_);
            pieces_[i][j]->resize(size::g_pieceSize.width, size::g_pieceSize.height);
        }
    }
}

void Palette::open()
{
    if (!init_)
    {
        initLabel();
        initPieces();
        init_ = true;
    }

    board_->init();
    drawPieces();
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

// 将board的pos位置棋子绘制到palette上
void Palette::drawPiece(TPos pos)
{
    if (QPixmap* pic = resMgr_->getPiece(board_->getPiece(pos)))
        pieces_[pos.row][pos.col]->setPixmap(*pic);

    TClientCo clientCo;
    co::pos2ClientCo(pos, clientCo);
    pieces_[pos.row][pos.col]->move(clientCo.x, clientCo.y);
}

void Palette::drawSelect(TPos prevPos, TPos currPos)
{
    if (co::isValidPos(prevPos))
    {
        prevSelect_->show();

        TClientCo clientCo;
        co::pos2ClientCo(prevPos, clientCo);
        prevSelect_->move(clientCo.x, clientCo.y);
    }
    else
    {
        prevSelect_->hide();
    }

    if (co::isValidPos(currPos))
    {
        currSelect_->show();

        TClientCo clientCo;
        co::pos2ClientCo(currPos, clientCo);
        currSelect_->move(clientCo.x, clientCo.y);
    }
    else
    {
        currSelect_->hide();
    }
}

bool Palette::movePiece(TPos curPos, TPos newPos)
{
    if (!board_->movePiece(curPos, newPos))
        return false;

    drawPiece(curPos);
    drawPiece(newPos);

    return true;
}
