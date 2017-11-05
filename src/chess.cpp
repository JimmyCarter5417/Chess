#include "chess.h"
#include "ui_chess.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPoint>
#include <QDebug>
#include <QDir>

#include "board.h"
#include "resmgr.h"
#include "palette.h"

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chess),
    board_(Board::getInstance()),
    resMgr_(ResMgr::getInstance()),
    palette_(new Palette(this, board_, resMgr_))
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);

    resMgr_->loadBoard(ResMgr::EBS_Wood);
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
