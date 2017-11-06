#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include "def.h"

class Board;
class ResMgr;
class Palette;

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
    void mousePressEvent(QMouseEvent *e);
    
private:
    Ui::Chess *ui;

    Board* board_;
    ResMgr* resMgr_;
    Palette* palette_;

    def::TPos oldPos_;
};

#endif // CHESS_H
