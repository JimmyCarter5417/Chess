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

#include "resmgr.h"
#include "palette.h"
#include "co.h"

using namespace def;

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chess),
    palette_(new Palette(this, ResMgr::getInstance()))
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
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_R)
    {
        palette_->rotate();
    }
    else if (event->key() == Qt::Key_O)
    {
        palette_->open();
    }
    else if (event->key() == Qt::Key_U)
    {
        palette_->undo();
    }
    /*else if (event->key() == Qt::Key_R)
    {
        palette_->redo();
    }*/
}
