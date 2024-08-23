#include "setting.h"
#include "ui_setting.h"
#include <QKeyEvent>
#include <QDir>

Setting::Setting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Setting)
{
    ui->setupUi(this);

    connect(ui -> toolScr, &QPushButton::clicked, this, &Setting::changeScreenKey);
    connect(ui -> toolVid, &QPushButton::clicked, this, &Setting::changeVideoKey);
    connect(ui -> toolBld, &QPushButton::clicked, this, &Setting::changeBuildTrackKey);
    connect(ui -> accept, &QPushButton::clicked, this, &Setting::acceptSettings);
    connect(ui -> cancel, &QPushButton::clicked, this, &Setting::cancelSettings);

    ui -> toolBld -> setFocusPolicy(Qt::NoFocus);
    ui -> toolScr -> setFocusPolicy(Qt::NoFocus);
    ui -> toolVid -> setFocusPolicy(Qt::NoFocus);
    ui -> accept -> setFocusPolicy(Qt::NoFocus);
    ui -> cancel -> setFocusPolicy(Qt::NoFocus);
    ui -> cBFullScreen -> setFocusPolicy(Qt::NoFocus);

    ui -> labelScrCh -> setText(QKeySequence(settingsFile -> value("ScreenKey").toInt()).toString());
    ui -> labelVidCh -> setText(QKeySequence(settingsFile -> value("VideoKey").toInt()).toString());
    ui -> labelBldCh -> setText(QKeySequence(settingsFile -> value("BuildTrackKey").toInt()).toString());
    ui -> cBFullScreen -> setChecked(settingsFile -> value("FullScreen").toBool());
}

Setting::~Setting()
{
    delete ui;
}

void Setting::acceptSettings()
{
    settingsFile -> setValue("ScreenKey", QKeySequence::fromString(ui -> labelScrCh -> text())[0].key());
    settingsFile -> setValue("VideoKey", QKeySequence::fromString(ui -> labelVidCh -> text())[0].key());
    settingsFile -> setValue("BuildTrackKey", QKeySequence::fromString(ui -> labelBldCh -> text())[0].key());

    if(ui -> cBFullScreen -> isChecked() != settingsFile -> value("FullScreen").toBool())
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                   tr("Для принятия некоторых изменений\nнеобходимо перезапустить программу."),
                                                                   QMessageBox::Ok | QMessageBox::Ok);
    }
    settingsFile -> setValue("FullScreen", ui -> cBFullScreen -> isChecked());
    Setting::close();
}

void Setting::cancelSettings()
{
    ui -> labelScrCh -> setText(QKeySequence(settingsFile -> value("ScreenKey").toInt()).toString());
    ui -> labelVidCh -> setText(QKeySequence(settingsFile -> value("VideoKey").toInt()).toString());
    ui -> labelBldCh -> setText(QKeySequence(settingsFile -> value("BuildTrackKey").toInt()).toString());
    ui -> cBFullScreen -> setChecked(settingsFile -> value("FullScreen").toBool());
    Setting::close();
}

void Setting::changeScreenKey()
{
    changeScreen = true;
    ui -> labelScrCh -> setText("Нажмите любую кнопку");
}

void Setting::changeVideoKey()
{
    changeVideo = true;
    ui -> labelVidCh -> setText("Нажмите любую кнопку");
}

void Setting::changeBuildTrackKey()
{
    changeBuildTrack = true;
    ui -> labelBldCh -> setText("Нажмите любую кнопку");
}

void Setting::keyPressEvent(QKeyEvent *event)
{
    qDebug() << QKeySequence(event -> key()).toString();
    if(changeScreen == true)
    {
        changeScreen = false;
        ui -> labelScrCh -> setText(QKeySequence(event -> key()).toString());
    }

    if(changeVideo == true)
    {
        changeVideo = false;
        ui -> labelVidCh -> setText(QKeySequence(event -> key()).toString());
    }

    if(changeBuildTrack == true)
    {
        changeBuildTrack = false;
        ui -> labelBldCh -> setText(QKeySequence(event -> key()).toString());
    }
}
