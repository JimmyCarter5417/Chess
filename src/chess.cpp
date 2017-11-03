#include "chess.h"
#include "ui_chess.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPoint>
#include <QDebug>
#include <QDir>


#include "resmgr.h"
#include "palette.h"

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chess)
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);
    QLabel* label = new QLabel(this);
    QPixmap* img;
    if (ResMgr::getInstance()->loadBoard(ResMgr::EBS_Canvas))
        img = ResMgr::getInstance()->getBoard();

    label->resize(521, 577);
    label->setPixmap(*img);

    QPixmap* img2;
    if (ResMgr::getInstance()->loadPieces(ResMgr::EPS_Delicate))
        img2 = ResMgr::getInstance()->getPiece(ResMgr::EP_BlackKing);
    QLabel* ll = new QLabel(this);
    ll->resize(57, 57);
    ll->setPixmap(*img2);
    ll->move(92 - img2->width() / 2, 92 - img2->height() / 2);
    //ll->hide();

    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    qDebug()<<"current currentPath: "<<QDir::currentPath();
}

Chess::~Chess()
{
    delete ui;
}
