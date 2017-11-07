#include "chess.h"
#include "ui_chess.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPoint>
#include <QDebug>
#include <QDir>
#include <QMouseEvent>

#include "board.h"
#include "resmgr.h"
#include "palette.h"
#include "co.h"

using namespace def;

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chess),
    board_(new Board),
    resMgr_(ResMgr::getInstance()),
    palette_(new Palette(this, board_.get(), resMgr_)),
    prevPos_(g_nullPos)
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);

    resMgr_->loadBg(ResMgr::EBS_Wood);
    resMgr_->loadPieces(ResMgr::EPS_Wood);

    palette_->drawBg();
    palette_->drawPieces();
    palette_->drawSelect({9,4});


//    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
//    qDebug()<<"current currentPath: "<<QDir::currentPath();
}

Chess::~Chess()
{
    delete ui;
}

void Chess::mousePressEvent(QMouseEvent *e)
{
    if (Qt::LeftButton == e->button())
    {
        TPos nextPos = g_nullPos;
        co::clientCo2Pos({e->x(), e->y()}, nextPos);

        if (prevPos_ == g_nullPos)
        {
            if ((board_->getPiece(nextPos) & g_pieceMask) != g_empty)
            {
                prevPos_ = nextPos;
                palette_->drawSelect(nextPos);
            }
        }
        else
        {
            bool flag = false;
            switch (board_->getPiece(prevPos_) & g_pieceMask)
            {
            case g_king:
                flag = board_->isValidKingMove(prevPos_, nextPos);
                break;
            case g_advisor:
                flag = board_->isValidAdvisorMove(prevPos_, nextPos);
                break;
            case g_bishop:
                flag = board_->isValidBishopMove(prevPos_, nextPos);
                break;
            case g_knight:
                flag = board_->isValidKnightMove(prevPos_, nextPos);
                break;
            case g_rook:
                flag = board_->isValidRookMove(prevPos_, nextPos);
                break;
            case g_cannon:
                flag = board_->isValidCannonMove(prevPos_, nextPos);
                break;
            case g_pawn:
                flag = board_->isValidPawnMove(prevPos_, nextPos);
                break;
            default:
                flag = false;
                break;
            }

            if (flag)
            {
                palette_->drawSelect(nextPos);
                //palette_->move(prevPos_, pos);
                prevPos_ = nextPos;
            }
        }

    }
//    if (Qt ::RightButton == e->button())
//    {

//    }
//    if (Qt ::MidButton == e->button())
//    {

//    }
}
