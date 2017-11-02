#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>

namespace Ui {
class Chess;
}

class Chess : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Chess(QWidget *parent = 0);
    ~Chess();
    
private:
    Ui::Chess *ui;
};

#endif // CHESS_H
