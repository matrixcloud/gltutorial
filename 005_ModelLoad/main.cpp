#include "mainwindow.h"
#include <QApplication>
#include "objloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
//    OBJLoader* loader = new OBJLoader();
//    loader->load();
}
