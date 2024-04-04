#include <QtWidgets>
#include <classes/delegate.h>
#include <forms/mainwindow.h>
#include "screenshot.h"
#include "forms/mainwindow.h"

Screenshot::Screenshot()
    :  screenshotLabel(new QLabel(this))
{
    QMainWindow w;
    screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    screenshotLabel->setAlignment(Qt::AlignCenter);

    const QRect screenGeometry = screen()->geometry();
    screenshotLabel->setMinimumSize(screenGeometry.width() / 8, screenGeometry.height() / 8);

    shootScreen();
}

void Screenshot::saveScreenshot(QString date, QString route, QString garage, QString time, QString problem)
{
    MainWindow mw;
    shootScreen();

    const QString format = "jpg";

    QString screens = "Скрины",
        dot = ".",
        space = " ",
        slash = "/",
        name = time.replace(QString(":"), QString("_")) +
               space + garage + space + problem + dot + format,
        initialPath = QCoreApplication::applicationDirPath();

    if (QDir(initialPath).mkdir(screens))
    {
        initialPath += slash + screens;
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(route))
                initialPath += slash + route;
            else if(QDir(initialPath).cd(route))
                initialPath += slash + route;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(route))
                initialPath += slash + route;
            else if(QDir(initialPath).cd(route))
                initialPath += slash + route;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else
            initialPath = QCoreApplication::applicationDirPath();
    }
    else if(QDir(initialPath).cd(screens))
    {
        initialPath += slash + screens;
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(route))
                initialPath += slash + route;
            else if(QDir(initialPath).cd(route))
                initialPath += slash + route;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(route))
                initialPath += slash + route;
            else if(QDir(initialPath).cd(route))
                initialPath += slash + route;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else
            initialPath = QCoreApplication::applicationDirPath();
    }
    else
        initialPath = QCoreApplication::applicationDirPath();

    initialPath += slash + name;

    if (!originalPixmap.save(initialPath, nullptr, 30))
    {
        QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                                                         .arg(QDir::toNativeSeparators(initialPath)));
    }
}

void Screenshot::shootScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    originalPixmap = screen->grabWindow(0);
}
