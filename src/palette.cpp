#include "palette.h"
#include "chess.h"
#include "board.h"
#include "resmgr.h"
#include "co.h"
#include "size.h"
#include <assert.h>
#include <QLabel>
#include <QMessageBox>

using namespace std;
using namespace def;

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

    prevPos_ = g_nullPos;
    drawSelect(g_nullPos);
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

void Palette::drawSelect(TPos currPos)
{
    if (co::isValidPos(prevPos_))
    {
        prevSelect_->show();

        TClientCo clientCo;
        co::pos2ClientCo(prevPos_, clientCo);
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

byte Palette::movePiece(TPos currPos)
{
    byte ret = board_->movePiece(prevPos_, currPos);

    if (ret & Board::EMR_ok)
    {
        drawPiece(prevPos_);
        drawPiece(currPos);
    }

    return ret;
}

void Palette::rotate()
{
    board_->rotate();
    drawPieces();
}

void Palette::click(TPos currPos)
{
    if (prevPos_ == g_nullPos)
    {
        // 若prevPos_为无效位置，且新位置棋子属于下一步走棋的玩家，则更新prevPos_，并显示当前选择的棋子
        if (board_->getPieceOwner(currPos) == board_->getNextPlayer())
        {
            drawSelect(currPos);
            prevPos_ = currPos;
        }
    }
    else
    {
        // 可以移动棋子
        byte ret = movePiece(currPos);

        if (ret & Board::EMR_ok)
        {QMessageBox::information(NULL, "ok", "ok");
            //显示两个选择框，prevPos_清空
            drawSelect(currPos);
            prevPos_ = g_nullPos;
        }
        else // 不能移动棋子
        {
            if (ret & Board::EMR_suicide)
            {
                QMessageBox::information(NULL, "suicide", "suicide");
            }
            if (ret & Board::EMR_check)
            {
                QMessageBox::information(NULL, "check", "check");
            }
            if (ret & Board::EMR_dead)
            {
                QMessageBox::information(NULL, "dead", "dead");
            }
            // 当前选择位置与原位置同色，才能更新prevPos_，并绘制选择框
            if (board_->getPieceOwner(prevPos_) == board_->getPieceOwner(currPos))
            {
                prevPos_ = g_nullPos;//不标记上次选中的位置
                drawSelect(currPos);
                prevPos_ = currPos;
            }
        }
    }
}
