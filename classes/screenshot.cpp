#include <QtWidgets>
#include <forms/mainwindow.h>
#include "screenshot.h"

//! [0]
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

void Screenshot::saveScreenshot()
{
    shootScreen();

    const QString format = "jpg";

    QString screens = "Скрины",
        date = "17.02",
        route = "41",
        problem = "ОБРЫВ БЛОКА ГЛОНАСС",
        time = "20:12",
        garage = "10612",
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

    if (!originalPixmap.save(initialPath, nullptr, 30)) {
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
