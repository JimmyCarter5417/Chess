#include "chess.h"
#include "ui_chess.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QPoint>
#include <QDebug>
#include <QDir>

Chess::Chess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chess)
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);

    //QLabel* label = new QLabel(this);
    //label->resize(550, 600);
    //label->move(QPoint(10, 10));
   // QGridLayout* mainLayout = new QGridLayout(this);
    //mainLayout->addWidget(label, 0, 0, Qt::AlignCenter);
    //QImage* img = new QImage;
    QPixmap* img = new QPixmap(tr(":/board/res/CANVAS.GIF"));
   // img->load(tr(":/board/res/QIANHONG.GIF"));
    //if(img->load(tr("BOARD.BMP")))
    {
        //ui->label->setPixmap(QPixmap::fromImage(*img));
        ui->label->setPixmap(*img);
        //label->setPixmap(*img);
    }

    QPixmap m(tr(":/chessman/res/WOOD/BA.GIF"));
    QLabel* ll = new QLabel(this);
    ll->resize(57, 57);
    ll->setPixmap(m);
    ll->move(92 - m.width() / 2, 92 - m.height() / 2);
    ll->hide();

    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    qDebug()<<"current currentPath: "<<QDir::currentPath();
}

Chess::~Chess()
{
    delete ui;
}
