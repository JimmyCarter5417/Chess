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

    palette_->open();
    palette_->drawSelect(g_nullPos, g_nullPos);

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
        TPos currPos = g_nullPos;
        co::clientCo2Pos({e->x(), e->y()}, currPos);

        if (prevPos_ == g_nullPos)
        {
            // 若prevPos_为无效位置，且新位置非空，更新prevPos_，并显示当前点
            if (palette_->getPiece(currPos) != ResMgr::EP_Empty)
            {                
                palette_->drawSelect(g_nullPos, currPos);
                prevPos_ = currPos;
            }
        }
        else
        {
            // 可以移动棋子
            if (palette_->movePiece(prevPos_, currPos))
            {
                //显示两个选择框，prevPos_清空
                palette_->drawSelect(prevPos_, currPos);
                prevPos_ = g_nullPos;
            }
            else// 不能移动棋子
            {
                // 当前选择位置与原位置同色，才能更新prevPos_，并绘制选择框
                if ((palette_->getPiece(prevPos_) & def::g_scopeMask) == (palette_->getPiece(currPos) & def::g_scopeMask))
                {
                    palette_->drawSelect(g_nullPos, currPos);
                    prevPos_ = currPos;
                }
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
