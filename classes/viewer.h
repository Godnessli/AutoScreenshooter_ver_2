#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QDir>
#include <QLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QSettings>
#include <QCoreApplication>

class Viewer : public QDialog
{
    Q_OBJECT
public:
    explicit Viewer(QDialog *parent = nullptr);
    void showImage(QString date, QString route, QString distination, QString time, QString garage, QString problem);
    void keyPressEvent(QKeyEvent *event);

    QPushButton *del = new QPushButton("Удалить скрин");
    QPushButton *prev = new QPushButton("Предыдущий");
    QPushButton *next = new QPushButton("Следующий");
    QPushButton *setVideo = new QPushButton("Видео");

protected:
    QDialog *viewer = new QDialog;

    QSettings *settingsFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                    "/" + "cache" + "/" + "settings.ini"), QSettings::IniFormat);

    QGridLayout *lout = new QGridLayout(viewer);
    QLabel *photo = new QLabel();

    QLabel *lbldate = new QLabel();
    QLabel *lblroute = new QLabel();
    QLabel *lbldirect = new QLabel();
    QLabel *lbltime = new QLabel();
    QLabel *lblgarage = new QLabel();
    QLabel *lblproblem = new QLabel();
signals:
    void nextScr();
    void prevScr();
    void delScr();
    void vidScr();
};

#endif // VIEWER_H
