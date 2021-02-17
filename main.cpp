#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Hamed BBB-Player");
    a.setDesktopFileName("Hamed BBB-Player");
    a.setApplicationVersion("0.1");
    mainwindow w;
    w.show();
    return a.exec();
}
