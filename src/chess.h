#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QActionGroup>
#include <memory>

class Board;
class ResMgr;
class Palette;

using std::shared_ptr;

namespace Ui {
class Chess;
}

class Chess : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Chess(QWidget *parent = 0);
    ~Chess();

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
private slots:
    void on_openAction_triggered();
    void on_undoAction_triggered();

    void on_soundEffectAction_triggered(bool checked);
    void on_bgmAction_triggered(bool checked);

    void on_canvasBgAction_triggered(bool checked);
    void on_dropsBgAction_triggered(bool checked);
    void on_greenBgAction_triggered(bool checked);
    void on_sheetBgAction_triggered(bool checked);
    void on_skeletonBgAction_triggered(bool checked);
    void on_stoneBgAction_triggered(bool checked);
    void on_woodBgAction_triggered(bool checked);

    void on_comicPieceAction_triggered(bool checked);
    void on_delicatePieceAction_triggered(bool checked);
    void on_polishPieceAction_triggered(bool checked);
    void on_woodPieceAction_triggered(bool checked);

private:
    Ui::Chess *ui;

    shared_ptr<QActionGroup> bgActionGroup_;
    shared_ptr<QActionGroup> pieceActionGroup_;
    shared_ptr<Palette> palette_;

};

#endif // CHESS_H
