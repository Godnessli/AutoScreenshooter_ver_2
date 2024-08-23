#ifndef SETTING_H
#define SETTING_H

#include "mainwindow.h"
#include <QDialog>
#include <QSettings>
#include <QCoreApplication>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    Setting(QWidget *parent = nullptr);
    ~Setting();

protected slots:
    void changeScreenKey();
    void changeVideoKey();
    void changeBuildTrackKey();

    void acceptSettings();
    void cancelSettings();
private:
    QSettings *settingsFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                    "/" + "cache" + "/" + "settings.ini"), QSettings::IniFormat);
    void keyPressEvent(QKeyEvent *event);
    bool changeScreen = false;
    bool changeVideo = false;
    bool changeBuildTrack = false;
    Ui::Setting *ui;
    MainWindow *mw;

};

#endif // SETTING_H
