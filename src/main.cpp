#include "chess.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Chess w;
    w.setWindowFlags(w.windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
    w.show();
    
    return a.exec();
}
