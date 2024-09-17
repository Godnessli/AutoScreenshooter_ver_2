#include "setting.h"
#include "ui_setting.h"
#include <QKeyEvent>
#include <QDir>

Setting::Setting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Setting)
{
    ui->setupUi(this);

    Setting::setWindowTitle("Настройки");
    connect(ui -> toolScr, &QPushButton::clicked, this, &Setting::changeScreenKey);
    connect(ui -> toolVid, &QPushButton::clicked, this, &Setting::changeVideoKey);
    connect(ui -> toolBld, &QPushButton::clicked, this, &Setting::changeBuildTrackKey);
    connect(ui -> deleteScrTB, &QPushButton::clicked, this, &Setting::changeDeleteKey);
    connect(ui -> nextScrTB, &QPushButton::clicked, this, &Setting::changeNextKey);
    connect(ui -> prevScrTB, &QPushButton::clicked, this, &Setting::changePrevKey);
    connect(ui -> vidScrTB, &QPushButton::clicked, this, &Setting::changeVidScrKey);

    connect(ui -> accept, &QPushButton::clicked, this, &Setting::acceptSettings);
    connect(ui -> cancel, &QPushButton::clicked, this, &Setting::cancelSettings);
    connect(ui -> screenSavePathChange, &QPushButton::clicked, this, &Setting::changeSavePath);

    ui -> toolBld -> setFocusPolicy(Qt::NoFocus);
    ui -> toolScr -> setFocusPolicy(Qt::NoFocus);
    ui -> toolVid -> setFocusPolicy(Qt::NoFocus);
    ui -> accept -> setFocusPolicy(Qt::NoFocus);
    ui -> cancel -> setFocusPolicy(Qt::NoFocus);
    ui -> tableWidget -> setFocusPolicy(Qt::NoFocus);
    ui -> cBFullScreen -> setFocusPolicy(Qt::NoFocus);
    ui -> tableWidget -> verticalHeader() -> hide();

    ui -> labelScrCh -> setText(QKeySequence(settingsFile -> value("ScreenKey").toInt()).toString());
    ui -> labelVidCh -> setText(QKeySequence(settingsFile -> value("VideoKey").toInt()).toString());
    ui -> labelBldCh -> setText(QKeySequence(settingsFile -> value("BuildTrackKey").toInt()).toString());
    ui -> cBFullScreen -> setChecked(settingsFile -> value("FullScreen").toBool());

    ui -> prevScrLbl -> setText(QKeySequence(settingsFile -> value("PrevKey").toInt()).toString());
    ui -> nextScrLbl -> setText(QKeySequence(settingsFile -> value("NextKey").toInt()).toString());
    ui -> deleteScrLbl -> setText(QKeySequence(settingsFile -> value("DeleteKey").toInt()).toString());
    ui -> vidScrLbl -> setText(QKeySequence(settingsFile -> value("VideoScrKey").toInt()).toString());

    if(!settingsFile -> value("SaveScreenPath").toString().isEmpty())
        ui -> screenSavePath -> setText(settingsFile -> value("SaveScreenPath").toString());
    else
        ui -> screenSavePath -> setText(QCoreApplication::applicationDirPath());

    buildTable();
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

    settingsFile -> setValue("PrevKey", QKeySequence::fromString(ui -> prevScrLbl -> text())[0].key());
    settingsFile -> setValue("NextKey", QKeySequence::fromString(ui -> nextScrLbl -> text())[0].key());
    settingsFile -> setValue("DeleteKey", QKeySequence::fromString(ui -> deleteScrLbl -> text())[0].key());
    settingsFile -> setValue("VideoScrKey", QKeySequence::fromString(ui -> vidScrLbl -> text())[0].key());

    if(!ui -> screenSavePath -> text().isEmpty())
        settingsFile -> setValue("SaveScreenPath", ui -> screenSavePath -> text());
    else
        ui -> screenSavePath -> setText(settingsFile -> value("SaveScreenPath").toString());

    if(ui -> cBFullScreen -> isChecked() != settingsFile -> value("FullScreen").toBool())
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                   tr("Для принятия некоторых изменений\nнеобходимо перезапустить программу."),
                                                                   QMessageBox::Ok | QMessageBox::Ok);
    }
    settingsFile -> setValue("FullScreen", ui -> cBFullScreen -> isChecked());

    for(int i = 0; i < 19; ++i)
    {
        if(!ui -> tableWidget -> item(i, 1) -> text().isEmpty())
        {
            QString route = ui -> tableWidget -> item(i, 1) -> text().replace("Г", "");
            route = route.replace("М", "");
            settingsFile -> beginGroup(QString("Route_") + route);
            settingsFile -> setValue("Route", ui -> tableWidget -> item(i, 1) -> text());
        }
        if(!ui -> tableWidget -> item(i, 0) -> text().isEmpty())
            settingsFile -> setValue("Date", ui -> tableWidget -> item(i, 0) -> text());

        if(!ui -> tableWidget -> item(i, 2) -> text().isEmpty())
            settingsFile -> setValue("Garage", ui -> tableWidget -> item(i, 2) -> text());

        if(!ui -> tableWidget -> item(i, 3) -> text().isEmpty())
            settingsFile -> setValue("Time", ui -> tableWidget -> item(i, 3) -> text());

        if(!ui -> tableWidget -> item(i, 4) -> text().isEmpty())
            settingsFile -> setValue("Dest", ui -> tableWidget -> item(i, 4) -> text());

        settingsFile -> endGroup();
    }

    Setting::close();
}

void Setting::cancelSettings()
{
    ui -> labelScrCh -> setText(QKeySequence(settingsFile -> value("ScreenKey").toInt()).toString());
    ui -> labelVidCh -> setText(QKeySequence(settingsFile -> value("VideoKey").toInt()).toString());
    ui -> labelBldCh -> setText(QKeySequence(settingsFile -> value("BuildTrackKey").toInt()).toString());

    ui -> cBFullScreen -> setChecked(settingsFile -> value("FullScreen").toBool());

    if(!settingsFile -> value("SaveScreenPath").toString().isEmpty())
        ui -> screenSavePath -> setText(settingsFile -> value("SaveScreenPath").toString());
    else
        ui -> screenSavePath -> setText(QCoreApplication::applicationDirPath());

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

void Setting::changeNextKey()
{
    changeNext = true;
    ui -> nextScrLbl -> setText("Нажмите любую кнопку");
}

void Setting::changePrevKey()
{
    changePrev = true;
    ui -> prevScrLbl -> setText("Нажмите любую кнопку");
}

void Setting::changeVidScrKey()
{
    changeVidScr = true;
    ui -> vidScrLbl -> setText("Нажмите любую кнопку");
}

void Setting::changeDeleteKey()
{
    changeDelete = true;
    ui -> deleteScrLbl -> setText("Нажмите любую кнопку");
}

void Setting::buildTable()
{
    QModelIndex qmi;
    QSizePolicy qsp;

    QStringList headers;
    QFont font = ui ->tableWidget -> horizontalHeader() -> font();
    font.setPointSize(12);
    ui -> tableWidget -> setColumnCount(5);
    ui -> tableWidget -> setRowCount(19);
    headers << "Дата" << "Маршрут" << "ТС" << "Время" << "Направление";
    ui -> tableWidget -> setHorizontalHeaderLabels(headers);
    ui -> tableWidget -> horizontalHeader() -> setFont(font);
    QPalette p = palette();
    const QColor hlClr = "#87cefa";
    const QColor txtClr = "black";

    ui -> tableWidget -> setSortingEnabled(false);
    ui -> tableWidget -> setSelectionBehavior(QAbstractItemView::SelectRows);
    ui -> tableWidget -> setSelectionMode(QAbstractItemView::SingleSelection);

    ui -> tableWidget -> setItem(0, 0, new QTableWidgetItem(QString("31.07.2024")));
    ui -> tableWidget -> setItem(0, 2, new QTableWidgetItem(QString("10510")));
    ui -> tableWidget -> setItem(0, 3, new QTableWidgetItem(QString("07:13")));
    ui -> tableWidget -> setItem(0, 1, new QTableWidgetItem(QString("10М")));
    ui -> tableWidget -> setItem(1, 1, new QTableWidgetItem(QString("15М")));
    ui -> tableWidget -> setItem(2, 1, new QTableWidgetItem(QString("18Г")));
    ui -> tableWidget -> setItem(3, 1, new QTableWidgetItem(QString("22Г")));
    ui -> tableWidget -> setItem(4, 1, new QTableWidgetItem(QString("28М")));
    ui -> tableWidget -> setItem(5, 1, new QTableWidgetItem(QString("35М")));
    ui -> tableWidget -> setItem(6, 1, new QTableWidgetItem(QString("41")));
    ui -> tableWidget -> setItem(7, 1, new QTableWidgetItem(QString("42М")));
    ui -> tableWidget -> setItem(8, 1, new QTableWidgetItem(QString("46")));
    ui -> tableWidget -> setItem(9, 1, new QTableWidgetItem(QString("50")));
    ui -> tableWidget -> setItem(10, 1, new QTableWidgetItem(QString("59Г")));
    ui -> tableWidget -> setItem(11, 1, new QTableWidgetItem(QString("73М")));
    ui -> tableWidget -> setItem(12, 1, new QTableWidgetItem(QString("80М")));
    ui -> tableWidget -> setItem(13, 1, new QTableWidgetItem(QString("82М")));
    ui -> tableWidget -> setItem(14, 1, new QTableWidgetItem(QString("85М")));
    ui -> tableWidget -> setItem(15, 1, new QTableWidgetItem(QString("93М")));
    ui -> tableWidget -> setItem(16, 1, new QTableWidgetItem(QString("94Г")));
    ui -> tableWidget -> setItem(17, 1, new QTableWidgetItem(QString("98Г")));
    ui -> tableWidget -> setItem(18, 1, new QTableWidgetItem(QString("99М")));

    ui -> tableWidget -> selectionModel() -> select(qmi.siblingAtRow(1), QItemSelectionModel::Rows);

    qsp.setHorizontalPolicy(QSizePolicy::Expanding);

    ui -> tableWidget -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    p.setColor(QPalette::Highlight, hlClr);
    p.setColor(QPalette::HighlightedText, txtClr);
    setPalette(p);

    ui -> tableWidget -> sortByColumn(1, Qt::SortOrder::AscendingOrder);

    for(int i = 0; i < 19; ++i)
    {
        settingsFile -> beginGroup(QString("Route_") + ui -> tableWidget -> item(i, 1) -> text().removeAt(2));
        if(settingsFile -> group() == QString("Route_" + ui -> tableWidget -> item(i, 1) -> text().removeAt(2)))
        {
            ui -> tableWidget -> setItem(i, 0, new QTableWidgetItem(settingsFile -> value("Date").toString()));
            ui -> tableWidget -> item(i, 0) ->setTextAlignment(Qt::AlignCenter);
            ui -> tableWidget -> setItem(i, 1, new QTableWidgetItem(settingsFile -> value("Route").toString()));
            ui -> tableWidget -> item(i, 1) ->setTextAlignment(Qt::AlignCenter);
            ui -> tableWidget -> setItem(i, 2, new QTableWidgetItem(settingsFile -> value("Garage").toString()));
            ui -> tableWidget -> item(i, 2) ->setTextAlignment(Qt::AlignCenter);
            ui -> tableWidget -> setItem(i, 3, new QTableWidgetItem(settingsFile -> value("Time").toString()));
            ui -> tableWidget -> item(i, 3) ->setTextAlignment(Qt::AlignCenter);
            ui -> tableWidget -> setItem(i, 4, new QTableWidgetItem(settingsFile -> value("Dest").toString()));
            ui -> tableWidget -> item(i, 4) ->setTextAlignment(Qt::AlignCenter);
        }
        settingsFile -> endGroup();
    }
}

void Setting::changeSavePath()
{
    QWidget *w = new QWidget();
    ui -> screenSavePath -> setText(QFileDialog::getExistingDirectory(w, "Выберите папку", ui -> screenSavePath -> text()));
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

    if(changePrev == true)
    {
        changePrev = false;
        ui -> prevScrLbl -> setText(QKeySequence(event -> key()).toString());
    }

    if(changeNext == true)
    {
        changeNext = false;
        ui -> nextScrLbl -> setText(QKeySequence(event -> key()).toString());
    }

    if(changeVidScr == true)
    {
        changeVidScr = false;
        ui -> vidScrLbl -> setText(QKeySequence(event -> key()).toString());
    }

    if(changeDelete == true)
    {
        changeDelete = false;
        ui -> deleteScrLbl -> setText(QKeySequence(event -> key()).toString());
    }
}
