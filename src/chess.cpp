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
#include <QKeyEvent>
#include <QMessageBox>

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
    palette_(new Palette(this, board_.get(), resMgr_))
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);   

    palette_->open();

//    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
//    qDebug()<<"current currentPath: "<<QDir::currentPath();
}

Chess::~Chess()
{
    delete ui;
}

void Chess::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        TPos currPos = g_nullPos;
        co::clientCo2Pos({event->x(), event->y()}, currPos);

        palette_->click(currPos);


    }
//    if (Qt ::RightButton == event->button())
//    {

//    }
//    if (Qt ::MidButton == event->button())
//    {

//    }
}

void Chess::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_A))
    {
        QMessageBox::aboutQt(NULL, "aboutQt");
    }
    else if ((event->key() == Qt::Key_R))
    {
        palette_->rotate();
    }
}
