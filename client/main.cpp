#include "mainwindow.h"
#include "loading.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loading w;
    w.show();
    return a.exec();
}
