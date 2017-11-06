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
    palette_(new Palette(this, board_, resMgr_)),
    oldPos_(g_nullPos)
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);

    resMgr_->loadBg(ResMgr::EBS_Skeleton);
    resMgr_->loadPieces(ResMgr::EPS_Wood);

    palette_->drawBg();
    palette_->drawPieces();
    palette_->drawSelect({0,4});
    palette_->drawSelect({9,4});


//    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
//    qDebug()<<"current currentPath: "<<QDir::currentPath();
}

Chess::~Chess()
{
    delete ui;

    if (palette_)
    {
        delete palette_;
        palette_ = nullptr;
    }
}

void Chess::mousePressEvent(QMouseEvent *e)
{
    if (Qt::LeftButton == e->button())
    {
        TPos pos = g_nullPos;
        co::clientCo2xy({e->x(), e->y()}, pos);

        if (oldPos_ == g_nullPos)
        {
            oldPos_ = pos;

        }

            palette_->drawSelect(pos);
            palette_->move(oldPos_, pos);
            oldPos_ = pos;




        /*if (board_->isValidKnightMove(pos, {pos.row - 1, pos.col + 2}))
            palette_->drawSelect(pos);

        if (board_->isValidBishopMove(pos, {pos.row - 2, pos.col + 2}))
            palette_->drawSelect(pos);

        if (board_->isValidCannonMove(pos, {pos.row, pos.col + 1}))
            palette_->drawSelect(pos);

        if (board_->isValidRookMove(pos, {pos.row - 3, pos.col}))
            palette_->drawSelect(pos);

        if (board_->isValidAdvisorMove(pos, {pos.row + 1, pos.col - 1}))
            palette_->drawSelect(pos);

        if (board_->isValidKingMove(pos, {pos.row + 1, pos.col - 1}))
            palette_->drawSelect(pos);*/
    }
//    if (Qt ::RightButton == e->button())
//    {

//    }
//    if (Qt ::MidButton == e->button())
//    {

//    }
}
