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
    ui(new Ui::Chess)
{
    //setWindowFlags(Qt::WindowMinimized | Qt::WindowSystemMenuHint);
    ui->setupUi(this);

    bgActionGroup_ = std::make_shared<QActionGroup>(this);
    bgActionGroup_->addAction(ui->canvasBgAction);
    bgActionGroup_->addAction(ui->dropsBgAction);
    bgActionGroup_->addAction(ui->greenBgAction);
    bgActionGroup_->addAction(ui->sheetBgAction);
    bgActionGroup_->addAction(ui->skeletonBgAction);
    bgActionGroup_->addAction(ui->stoneBgAction);
    bgActionGroup_->addAction(ui->woodBgAction);
    bgActionGroup_->setExclusive(true);

    pieceActionGroup_ = std::make_shared<QActionGroup>(this);
    pieceActionGroup_->addAction(ui->comicPieceAction);
    pieceActionGroup_->addAction(ui->delicatePieceAction);
    pieceActionGroup_->addAction(ui->polishPieceAction);
    pieceActionGroup_->addAction(ui->woodPieceAction);
    pieceActionGroup_->setExclusive(true);

    // 默认风格
    ui->woodBgAction->setChecked(true);
    ui->woodPieceAction->setChecked(true);

    palette_ = std::make_shared<Palette>(this, ui->bg, ResMgr::getInstance());
    palette_->open();

    //qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    //qDebug()<<"current currentPath: "<<QDir::currentPath();
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


void Chess::on_openAction_triggered()
{
    palette_->open();
}

void Chess::on_undoAction_triggered()
{
    palette_->undo();
}

void Chess::on_soundEffectAction_triggered(bool checked)
{
    palette_->soundEffect(checked);
}

void Chess::on_bgmAction_triggered(bool checked)
{
    palette_->bgm(checked);
}

void Chess::on_canvasBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_canvas);
    }
}

void Chess::on_dropsBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_drops);
    }
}

void Chess::on_greenBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_green);
    }
}

void Chess::on_sheetBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_sheet);
    }
}

void Chess::on_skeletonBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_skeleton);
    }
}

void Chess::on_stoneBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_stone);
    }
}

void Chess::on_woodBgAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadBgSkin(ResMgr::EBS_wood);
    }
}

void Chess::on_comicPieceAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadPieceSkin(ResMgr::EPS_comic);
    }
}

void Chess::on_delicatePieceAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadPieceSkin(ResMgr::EPS_delicate);
    }
}

void Chess::on_polishPieceAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadPieceSkin(ResMgr::EPS_polish);
    }
}

void Chess::on_woodPieceAction_triggered(bool checked)
{
    if (checked)
    {
        palette_->loadPieceSkin(ResMgr::EPS_wood);
    }
}
