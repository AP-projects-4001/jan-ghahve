#include "loading.h"
#include <QApplication>
#include <QThread>
//#include "myclient.h"
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loading w;
    w.show();

    return a.exec();
}
