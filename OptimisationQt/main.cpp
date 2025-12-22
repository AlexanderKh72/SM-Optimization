#include "optimisation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Optimisation w;
    w.show();
    return a.exec();
}
