#include "forms/mainwindow.h"
#include "classes/screenshot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Screenshot s;
    s.show();
    MainWindow w;
    w.show();
    return a.exec();
}
