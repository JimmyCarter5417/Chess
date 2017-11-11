#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <memory>

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
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
private:
    Ui::Chess *ui;

    std::shared_ptr<Palette> palette_;   
};

#endif // CHESS_H
