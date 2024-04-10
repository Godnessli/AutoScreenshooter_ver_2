#include "forms/mainwindow.h"
#include "classes/table.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "From main thread: " << QThread::currentThreadId();

    MainWindow w;
    w.show();

    return a.exec();
}
