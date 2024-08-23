#include <QApplication>
#include "forms/mainwindow.h"
#include "classes/table.h"
#include <iostream>

QScopedPointer<QFile> m_logFile;

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch(type)
    {
    case QtWarningMsg:
        out << "WRN";
        break;
    case QtCriticalMsg:
        out << "CRT";
        break;
    case QtFatalMsg:
        out << "FTL";
        break;
    }

    out << context.category << ": " << msg << "\n";
    out.flush();
}

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
    QString fileName = QCoreApplication::applicationDirPath();
    QDir(fileName).mkdir("cache");
    fileName = fileName + "/" + "cache" + "/" + "logs.txt";
    QFile(fileName).remove();
    m_logFile.reset(new QFile(fileName));
    m_logFile.data() -> open(QFile::Append | QFile::Text);
    qInstallMessageHandler(myMessageHandler);

    if(settingsFile -> value("FullScreen").toBool())
        w.showFullScreen();
    else
        w.show();

    return a.exec();
}
