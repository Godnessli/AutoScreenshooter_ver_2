#include "viewer.h"
#include "forms/mainwindow.h"

Viewer::Viewer(QDialog *parent)
    : QDialog{parent}
{
    lout -> addWidget(lbldate, 0, 0);
    lout -> addWidget(lblroute, 0, 1);
    lout -> addWidget(lbldirect, 0, 2);
    lout -> addWidget(lbltime, 0, 3);
    lout -> addWidget(lblgarage, 0, 4);
    lout -> addWidget(lblproblem, 0, 5);

    QFont txtfont = lbldate -> font();
    txtfont.setPointSize(14);

    lbldate -> setFont(txtfont);
    lbldate -> setAlignment(Qt::AlignCenter);
    lbldate -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    lblroute -> setFont(txtfont);
    lblroute -> setAlignment(Qt::AlignCenter);
    lblroute -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    lbldirect -> setFont(txtfont);
    lbldirect -> setAlignment(Qt::AlignCenter);
    lbldirect -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    lbltime -> setFont(txtfont);
    lbltime -> setAlignment(Qt::AlignCenter);
    lbltime -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    lblgarage -> setFont(txtfont);
    lblgarage -> setAlignment(Qt::AlignCenter);
    lblgarage -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    txtfont.setPointSize(12);
    lblproblem -> setFont(txtfont);
    lblproblem -> setAlignment(Qt::AlignCenter);
    lblproblem -> setStyleSheet(QString(R"(
        QLabel{
            background: #FFFAF0;
            border: 1px solid #000000;
        })"));

    lout -> addWidget(photo, 1, 0, 1, 6);
    lout -> addWidget(del, 2, 0);
    del -> setMinimumHeight(30);
    lout -> addWidget(prev, 2, 1, 1, 2);
    prev -> setMinimumHeight(30);
    lout -> addWidget(next, 2, 3, 1, 2);
    next -> setMinimumHeight(30);
    lout -> addWidget(setVideo, 2, 5);
    setVideo -> setMinimumHeight(30);

    del -> setFocusPolicy(Qt::NoFocus);
    prev -> setFocusPolicy(Qt::NoFocus);
    next -> setFocusPolicy(Qt::NoFocus);
    setVideo -> setFocusPolicy(Qt::NoFocus);

    Viewer::setLayout(lout);
    Viewer::setModal(true);
}

void Viewer::showImage(QString date, QString route, QString distination, QString time, QString garage, QString problem)
{
    QString path;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() &&
        (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               date + "/" + route + "/" + time.replace(QString(":"), QString("_")) + QString(" ") +
               garage + QString(" ") + problem + "." + QString("jpg");
    else
        path = QCoreApplication::applicationDirPath() + "/" + QString("Скрины") + "/" +
               date + "/" + route + "/" + time.replace(QString(":"), QString("_")) + QString(" ") +
               garage + QString(" ") + problem + "." + QString("jpg");

    QPixmap image(path);
    photo -> setPixmap(image);

    Viewer::setMinimumSize(image.width(), image.height() + 100);
    Viewer::setMaximumSize(image.width(), image.height() + 100);

    del -> setMinimumWidth(viewer -> width() / 3 / 2 - 10);
    prev -> setMinimumWidth(viewer -> width() / 3 - 30);
    next -> setMinimumWidth(viewer -> width() / 3 - 20);
    setVideo -> setMinimumWidth(viewer -> width() / 3 / 2 - 10);

    lbldate -> setText("Дата: " + date);
    lblroute -> setText("Маршрут: " + route);
    lbldirect -> setText("Направление:\n" + distination);
    lbltime -> setText("Время: " + time);
    lblgarage -> setText("Гар. Номер: " + garage);
    lblproblem -> setText("Проблема:\n" + problem);
}

void Viewer::keyPressEvent(QKeyEvent *event)
{
    qDebug() << QKeySequence(event -> key()).toString();

    if(event -> key() == settingsFile -> value("NextKey"))
    {
        emit nextScr();
    }

    if(event -> key() == settingsFile -> value("PrevKey"))
    {
        emit prevScr();
    }

    if(event -> key() == settingsFile -> value("DeleteKey"))
    {
        emit delScr();
    }

    if(event -> key() == settingsFile -> value("VideoScrKey"))
    {
        emit vidScr();
    }
}
