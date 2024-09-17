#include <QApplication>
#include "forms/mainwindow.h"
#include "classes/table.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir(QCoreApplication::applicationDirPath()).mkdir("cache");
    QSettings *settingsFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                    "/" + "cache" + "/" + "settings.ini"), QSettings::IniFormat);

    if(!settingsFile -> value("ScreenKey").isValid())
        settingsFile -> setValue("ScreenKey", Qt::Key_1);

    if(!settingsFile -> value("VideoKey").isValid())
        settingsFile -> setValue("VideoKey", Qt::Key_2);

    if(!settingsFile -> value("BuildTrackKey").isValid())
        settingsFile -> setValue("BuildTrackKey", Qt::Key_3);

    if(!settingsFile -> value("DeleteScreenKey").isValid())
        settingsFile -> setValue("DeleteScreenKey", Qt::Key_4);

    if(!settingsFile -> value("FullScreen").isValid())
        settingsFile -> setValue("FullScreen", false);

    settingsFile -> sync();

    MainWindow w;

    if(settingsFile -> value("FullScreen").toBool())
        w.showFullScreen();
    else
        w.show();

    return a.exec();
}
