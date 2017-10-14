#include "namize.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Namize w;
    w.show();

    return a.exec();
}
