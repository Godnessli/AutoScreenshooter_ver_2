#include "mainwindow.h"
#include "classes/viewer.h"
#include "classes/table.h"
#include "ui_mainwindow.h"
#include "classes/delegate.h"
#include "forms/setting.h"
#include "forms/dialog.h"
#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QSqlQuery>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Viewer *viewer = new Viewer;
    Setting *sett = new Setting;
    Dialog *dia = new Dialog;

    QPushButton *ltb = ui ->loadTableBtn;
    QPushButton *screen = ui -> screenBtn;
    QPushButton *buildTrack = ui -> buildTrack;

    ui -> autoBtn -> setEnabled(false);
    ui -> buildTrack -> setEnabled(false);
    ui -> openScreen -> setEnabled(false);
    ui -> videoBtn -> setEnabled(false);
    ui -> buildRoute -> setEnabled(false);
    ui ->buildTrackself -> setEnabled(false);

    ltb -> setAutoDefault(false);
    screen -> setAutoDefault(false);

    connect(ltb, &QPushButton::clicked, this, &MainWindow::checkTable);
    connect(ui -> videoBtn, &QPushButton::clicked, this, &MainWindow::video);
    connect(screen, &QPushButton::clicked, this, &MainWindow::screenshot);
    connect(buildTrack, &QPushButton::clicked, this, &MainWindow::buildTrack);
    connect(ui -> buildRoute, &QPushButton::clicked, this, &MainWindow::buildRoute);
    connect(timer1, &QTimer::timeout, this, &MainWindow::entered);
    connect(timer2, &QTimer::timeout, this, &MainWindow::responseJson);
    connect(timer4, &QTimer::timeout, this, &MainWindow::dateSetFunc);
    connect(timer5, &QTimer::timeout, this, &MainWindow::garageSet);
    connect(timer6, &QTimer::timeout, this, &MainWindow::responseJsonRoute);
    connect(timer3, &QTimer::timeout, this, &MainWindow::setTimeInterval);
    connect(timer7, &QTimer::timeout, this, &MainWindow::checkTrack);
    connect(timer8, &QTimer::timeout, this, &MainWindow::signalOfTrackBuilded);
    connect(ui -> tableWidget_Item, &QTableWidget::itemClicked, this, &MainWindow::selectedItems);
    connect(ui -> tableWidget_Item, &QTableWidget::itemChanged, this, &MainWindow::paintRows);
    connect(ui -> tableWidget_Item -> horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::paintRows);
    connect(ui -> openScreen, &QPushButton::clicked, this, &MainWindow::showImage);
    connect(ui -> closeApp, &QAction::triggered, this, &QApplication::quit);
    connect(dia, &Dialog::accepted, this, &MainWindow::buildTrackSelf);

    a.setRunning(false);

    sett -> setModal(true);
    dia -> setModal(true);
    connect(ui -> control, &QAction::triggered, sett, &Setting::show);
    connect(ui -> buildTrackself, &QPushButton::clicked, dia, &Dialog::show);

    layout -> addWidget(lbl, 0, 0, 1, 2);
    layout -> addWidget(logIn, 1, 0, 1, 2);
    layout -> addWidget(password, 2, 0, 1, 2);
    layout -> addWidget(in, 4, 0);
    layout -> addWidget(out, 4, 1);

    keysLbl -> setAlignment(Qt::AlignRight);

    settings -> setModal(true);
    settings -> setLayout(settingLayout);

    settingLayout -> addWidget(screenLbl, 0, 0);
    settingLayout -> addWidget(screenButton, 0, 1);
    settingLayout -> addWidget(videoLbl, 1, 0);
    settingLayout -> addWidget(buildTrackLbl, 2, 0);
    settingLayout -> addWidget(accept, 3, 0);
    settingLayout -> addWidget(cancel, 3, 1);

    connect(in, SIGNAL(clicked()), this, SLOT(login()));
    connect(out, &QPushButton::clicked, this, &MainWindow::closeDialog);
    //connect(prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    //connect(del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    //connect(setVideo, &QPushButton::clicked, this, &MainWindow::setVid);

    ui -> autoBtn -> setFocusPolicy(Qt::NoFocus);
    ui -> autoCheck -> setFocusPolicy(Qt::NoFocus);
    ui -> buildTrack -> setFocusPolicy(Qt::NoFocus);
    ui -> screenBtn -> setFocusPolicy(Qt::NoFocus);
    ui -> loadTableBtn -> setFocusPolicy(Qt::NoFocus);
    ui -> openScreen -> setFocusPolicy(Qt::NoFocus);
    ui -> settingsMenu -> setFocusPolicy(Qt::NoFocus);
    ui -> tableWidget_Item -> setFocusPolicy(Qt::NoFocus);

    QString connectString = "Driver={SQL Server};";
    connectString.append("Server=10.0.20.16;");
    connectString.append("Database=APPSERVER;");
    connectString.append("Uid=trn-admin;");
    connectString.append("Pwd=2578325783;");
    db.setDatabaseName(connectString);

    if(db.open())
    {
        qDebug() << "Connected";
        QSqlQuery allTables("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='tbWorkTokens';");

        while(allTables.next())
        {
            qDebug() << allTables.value(0).toString();
        }
        QSqlQuery workLogins("SELECT * FROM tbWorkLogins", db);
        QSqlQuery workTokens("SELECT * FROM tbWorkTokens", db);

        std::vector<std::vector<std::string>> idwl;
        std::vector<std::vector<std::string>> idwt;

        while(workLogins.next())
        {
            std::vector<std::string> push;
            for(int i = 0; i < 8; ++i)
            {
                push.push_back(workLogins.value(i).toString().toStdString());
            }
            idwl.push_back(push);
        }

        qDebug() << idwl;

        while(workTokens.next())
        {
            std::vector<std::string> push;
            for(int i = 0; i < 5; ++i)
            {
                push.push_back(workTokens.value(i).toString().toStdString());
            }
            idwt.push_back(push);
        }

        if(idwl.size() > 1)
        {
            if(QString::fromStdString(idwl[0][2]).toLower() != QString::fromStdString(idwl[1][2]).simplified().toLower())
            {
                for(int i = 0; i < idwl.size(); ++i)
                {
                    for(int j = 0; j < idwt.size(); ++j)
                    {
                        if(idwl[i][1] == idwt[j][1])
                        {
                            if(idwt[j][4] == "webkursk1"  && idwl[i][5] == "0" &&
                                (settingsFile -> value("Login").toString().toLower() != QString::fromStdString(idwl[i][2]).toLower()))
                                key1 = true;
                            else if(idwt[j][4] == "webkursk2"  && idwl[i][5] == "0" &&
                                    (settingsFile -> value("Login").toString().toLower() != QString::fromStdString(idwl[i][2]).toLower()))
                                key2 = true;
                        }
                    }
                }
            }
            else
            {
                key1 = true;
            }
        }
        else
        {
            if(idwt.size() > 1)
            {
                key1 = true;
            }
            else if(idwt.size() == 1)
            {
                if(idwl[0][1] == idwt[0][1])
                {
                    if(idwt[0][4] == "webkursk1" && idwl[0][5] == "0" &&
                        (settingsFile -> value("Login").toString().toLower() != QString::fromStdString(idwl[0][2]).toLower()))
                        key1 = true;
                    else if(idwt[0][4] == "webkursk2" && idwl[0][5] == "0" &&
                             (settingsFile -> value("Login").toString().toLower() != QString::fromStdString(idwl[0][2]).toLower()))
                        key2 = true;
                }
            }
        }

        qDebug() << idwt;

        if(key1 && key2)
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                       QString("Оба ключа заняты. Работа программы невозможна.\nПодождите, пока ") + QString::fromStdString(idwl[0][2]) +
                                                                           QString(" или ") + QString::fromStdString(idwl[1][2]) + QString("\nзавершат работу"),
                                                                       QMessageBox::Ok | QMessageBox::Ok);
            QLabel *err = new QLabel("Ключи недоступны");
            err -> setAlignment(Qt::AlignCenter);
            layout -> addWidget(err, 3, 0, 1, 2);
        }
        else if(key1)
        {
            if(settingsFile -> value("Login").isValid() &&
                settingsFile -> value("Password").isValid() &&
                settingsFile -> value("Key").isValid())
            {
                settingsFile -> setValue("Key", "webkursk2");
            }
            else
            {
                keys -> addItem("Ключ 2");
                keys -> setItemData(0, QString("webkursk2"));
                layout -> addWidget(keys, 3, 1);
                layout -> addWidget(keysLbl, 3, 0);
            }
        }
        else if(key2)
        {
            if(settingsFile -> value("Login").isValid() &&
                settingsFile -> value("Password").isValid() &&
                settingsFile -> value("Key").isValid())
            {
                settingsFile -> setValue("Key", "webkursk1");
            }
            else
            {
                keys -> addItem("Ключ 2");
                keys -> setItemData(0, QString("webkursk1"));
                layout -> addWidget(keys, 3, 1);
                layout -> addWidget(keysLbl, 3, 0);
            }
        }
        else
        {
            if(!settingsFile -> value("Login").isValid() ||
                !settingsFile -> value("Password").isValid() ||
                !settingsFile -> value("Key").isValid())
            {
                keys -> addItem("Ключ 1");
                keys -> setItemData(0, QString("webkursk1"));
                keys -> addItem("Ключ 2");
                keys -> setItemData(1, QString("webkursk2"));
                layout -> addWidget(keys, 3, 1);
                layout -> addWidget(keysLbl, 3, 0);
            }
        }
    }
    else
    {
        qDebug() << "Not Connected";
    }

    web();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeDialog()
{
    enter -> close();
}

void MainWindow::showImage()
{
    connect(viewer -> next, &QPushButton::clicked, this, &MainWindow::nextImage);
    connect(viewer -> prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    connect(viewer -> del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    connect(viewer -> setVideo, &QPushButton::clicked, this, &MainWindow::setVid);
    connect(viewer, &Viewer::nextScr, this, &MainWindow::nextImage);
    connect(viewer, &Viewer::prevScr, this, &MainWindow::prevImage);
    connect(viewer, &Viewer::delScr, this, &MainWindow::deleteScreen);
    connect(viewer, &Viewer::vidScr, this, &MainWindow::setVid);

    viewer -> showImage(ui -> tableWidget_Item -> selectedItems().at(0) -> text(),
                        ui -> tableWidget_Item -> selectedItems().at(1) -> text(),
                        ui -> tableWidget_Item -> selectedItems().at(2) -> text(),
                        ui -> tableWidget_Item -> selectedItems().at(3) -> text(),
                        ui -> tableWidget_Item -> selectedItems().at(5) -> text(),
                        ui -> tableWidget_Item -> selectedItems().at(6) -> text());

    if(!viewer -> isVisible())
        viewer -> show();
    else
        viewer -> update();
}

void MainWindow::nextImage()
{
    for(int i = ui -> tableWidget_Item -> selectedItems().at(0) -> row() + 1; i < ui -> tableWidget_Item -> rowCount(); ++i)
    {
        if(ui -> tableWidget_Item -> item(i, 7) -> text() == QString("Есть"))
        {
            ui -> tableWidget_Item -> selectRow(i);
            break;
        }
    }

    QString path;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    disconnect(viewer -> next, &QPushButton::clicked, this, &MainWindow::nextImage);
    disconnect(viewer -> prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    disconnect(viewer -> del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    disconnect(viewer -> setVideo, &QPushButton::clicked, this, &MainWindow::setVid);
    disconnect(viewer, &Viewer::nextScr, this, &MainWindow::nextImage);
    disconnect(viewer, &Viewer::prevScr, this, &MainWindow::prevImage);
    disconnect(viewer, &Viewer::delScr, this, &MainWindow::deleteScreen);
    disconnect(viewer, &Viewer::vidScr, this, &MainWindow::setVid);
    showImage();
}

void MainWindow::prevImage()
{
    for(int i = ui -> tableWidget_Item -> selectedItems().at(0) -> row() - 1; i >= 0; --i)
    {
        if(ui -> tableWidget_Item -> item(i, 7) -> text() == QString("Есть"))
        {
            ui -> tableWidget_Item -> selectRow(i);
            break;
        }
    }

    QString path;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    disconnect(viewer -> next, &QPushButton::clicked, this, &MainWindow::nextImage);
    disconnect(viewer -> prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    disconnect(viewer -> del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    disconnect(viewer -> setVideo, &QPushButton::clicked, this, &MainWindow::setVid);
    disconnect(viewer, &Viewer::nextScr, this, &MainWindow::nextImage);
    disconnect(viewer, &Viewer::prevScr, this, &MainWindow::prevImage);
    disconnect(viewer, &Viewer::delScr, this, &MainWindow::deleteScreen);
    disconnect(viewer, &Viewer::vidScr, this, &MainWindow::setVid);
    showImage();
}

void MainWindow::deleteScreen()
{
    QString path;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    QDir dir;
    dir.remove(path);

    ui -> tableWidget_Item -> selectedItems().at(7) -> setText("");

    for(int i = ui -> tableWidget_Item -> selectedItems().at(0) -> row() + 1; i < ui -> tableWidget_Item -> rowCount(); ++i)
    {
        if(ui -> tableWidget_Item -> item(i, 7) -> text() == QString("Есть"))
        {
            ui -> tableWidget_Item -> selectRow(i);
            break;
        }

        if(i == ui -> tableWidget_Item -> rowCount() - 1)
        {
            for(int j = ui -> tableWidget_Item -> selectedItems().at(0) -> row() - 1; j >= 0; --j)
            {
                if(ui -> tableWidget_Item -> item(j, 7) -> text() == QString("Есть"))
                {
                    ui -> tableWidget_Item -> selectRow(j);
                    break;
                }

                if(j == 0)
                {
                    viewer -> close();
                }
            }
        }
    }

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    disconnect(viewer -> next, &QPushButton::clicked, this, &MainWindow::nextImage);
    disconnect(viewer -> prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    disconnect(viewer -> del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    disconnect(viewer -> setVideo, &QPushButton::clicked, this, &MainWindow::setVid);
    disconnect(viewer, &Viewer::nextScr, this, &MainWindow::nextImage);
    disconnect(viewer, &Viewer::prevScr, this, &MainWindow::prevImage);
    disconnect(viewer, &Viewer::delScr, this, &MainWindow::deleteScreen);
    disconnect(viewer, &Viewer::vidScr, this, &MainWindow::setVid);
    showImage();
    paintRows();
}

void MainWindow::setVid()
{
    QString path;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    QDir dir;
    dir.remove(path);

    ui -> tableWidget_Item -> selectedItems().at(7) -> setText("Видео");

    for(int i = ui -> tableWidget_Item -> selectedItems().at(0) -> row() + 1; i < ui -> tableWidget_Item -> rowCount(); ++i)
    {
        if(ui -> tableWidget_Item -> item(i, 7) -> text() == QString("Есть"))
        {
            ui -> tableWidget_Item -> selectRow(i);
            break;
        }

        if(i == ui -> tableWidget_Item -> rowCount() - 1)
        {
            for(int j = ui -> tableWidget_Item -> selectedItems().at(0) -> row() - 1; j >= 0; --j)
            {
                if(ui -> tableWidget_Item -> item(j, 7) -> text() == QString("Есть"))
                {
                    ui -> tableWidget_Item -> selectRow(j);
                    break;
                }

                if(j == 0)
                {
                    viewer -> close();
                }
            }
        }
    }

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && (settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath()))
        path = settingsFile -> value("SaveScreenPath").toString() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
    else
        path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
               ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
               ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
               QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
               ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

    disconnect(viewer -> next, &QPushButton::clicked, this, &MainWindow::nextImage);
    disconnect(viewer -> prev, &QPushButton::clicked, this, &MainWindow::prevImage);
    disconnect(viewer -> del, &QPushButton::clicked, this, &MainWindow::deleteScreen);
    disconnect(viewer -> setVideo, &QPushButton::clicked, this, &MainWindow::setVid);
    disconnect(viewer, &Viewer::nextScr, this, &MainWindow::nextImage);
    disconnect(viewer, &Viewer::prevScr, this, &MainWindow::prevImage);
    disconnect(viewer, &Viewer::delScr, this, &MainWindow::deleteScreen);
    disconnect(viewer, &Viewer::vidScr, this, &MainWindow::setVid);
    showImage();
    paintRows();
}

void MainWindow::checkTable()
{
    if(QDir(table -> filepath).isReadable())
    {
        if(!table -> filepath.isEmpty())
        {
            xlnt::workbook wb;
            try {
                wb.load(table -> filepath.toStdString());
                auto ws = wb.active_sheet();
                for(auto row : ws.rows(false))
                {
                    ws.row_properties(row[0].row()).hidden = false;
                }
                for(int i = 0; i < tab.size(); ++i)
                {
                    QString poz = "I" + QString::number(ui -> tableWidget_Item -> item(i, 8) -> text().toInt() + 1);
                    if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
                        continue;
                    ws.cell(poz.toStdString()).value(ui -> tableWidget_Item -> item(i, 7) -> text().toStdString());
                }
                wb.save(table -> filepath.toStdString());
                buildTable();
            } catch (xlnt::exception) {
                QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                           tr("Ваш файл с таблицей открыт.\n Закройте файл и повторите попытку.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                           QMessageBox::Ok | QMessageBox::Cancel);

                if(resBtn != QMessageBox::Ok) {
                    buildTable();
                }
            }
        }
        else
        {
            buildTable();
        }
    }
    else
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                   tr("Файл перемещен, переименован или удален.\n Укажите новый путь к файлу.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                   QMessageBox::Ok | QMessageBox::Cancel);

        if(resBtn == QMessageBox::Ok) {
            table -> getpath();
        } else {
            buildTable();
        }
    }
}

void MainWindow::paintRows()
{
    count = 0;
    m_delegate -> setGoodRows({});
    m_delegate -> setQuestionRows({});
    m_delegate -> setBadRows({});

    QVector<int> grws;
    QVector<int> qrws;
    QVector<int> brws;

    qDebug() << "1 passed";
    for(int i = 0; i < ui -> tableWidget_Item ->rowCount(); ++i)
    {
        QString check = ui -> tableWidget_Item -> item(i, 7) -> text();
        QString path;

        if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable() && settingsFile -> value("SaveScreenPath").toString() != QCoreApplication::applicationDirPath())
            path = settingsFile -> value("SaveScreenPath").toString() + "/" +
                   ui -> tableWidget_Item -> item(i, 0) -> text() + "/" +
                   ui -> tableWidget_Item -> item(i, 1) -> text() + "/" +
                   ui -> tableWidget_Item -> item(i, 3) -> text().replace(QString(":"), QString("_")) +
                   QString(" ") + ui -> tableWidget_Item -> item(i, 5) -> text() + QString(" ") +
                   ui -> tableWidget_Item -> item(i, 6) -> text() + "." + format;
        else
            path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
                   ui -> tableWidget_Item -> item(i, 0) -> text() + "/" +
                   ui -> tableWidget_Item -> item(i, 1) -> text() + "/" +
                   ui -> tableWidget_Item -> item(i, 3) -> text().replace(QString(":"), QString("_")) +
                   QString(" ") + ui -> tableWidget_Item -> item(i, 5) -> text() + QString(" ") +
                   ui -> tableWidget_Item -> item(i, 6) -> text() + "." + format;

        if(check == QString("Есть") && !QDir(path).isReadable())
        {
            count++;
            qrws.push_back(i);
            ui -> tableWidget_Item -> item(i, 7) -> setText("Есть(Скрина нет)");
        }
        else if(check == QString("Есть(Скрина нет)") && QDir(path).isReadable())
        {
            count++;
            grws.push_back(i);
            ui -> tableWidget_Item -> item(i, 7) -> setText("Есть");
        }
        else if(check == QString("Есть"))
        {
            count++;
            grws.push_back(i);
        }
        else if(check == QString("Видео"))
        {
            count++;
            brws.push_back(i);
        }
        else if(!check.isEmpty())
        {
            count++;
            qrws.push_back(i);
        }
    }
    qDebug() << "2 passed";

    ui -> maked -> display(count);
    ui -> balance -> display(ui -> all -> value() - count);
    m_delegate -> setGoodRows(grws);
    m_delegate -> setQuestionRows(qrws);
    m_delegate -> setBadRows(brws);

    ui -> tableWidget_Item -> update();
}

void MainWindow::buildTable()
{
    disconnect(ui -> tableWidget_Item, &QTableWidget::itemChanged, this, &MainWindow::paintRows);
    QModelIndex qmi;
    QSizePolicy qsp;
    table -> readtable();
    tab = table -> tab;
    if(!tab.empty())
    {
        ui -> tableWidget_Item -> clear();
        QStringList headers;
        QFont font = ui ->tableWidget_Item -> horizontalHeader() -> font();
        font.setPointSize(16);
        headers << "Дата" << "Маршрут" << "Направление" << "План" << "Факт" << "Гаржный номер" << "Проблема" << "Скрин";
        ui -> tableWidget_Item -> setHorizontalHeaderLabels(headers);
        ui -> tableWidget_Item -> horizontalHeader() -> setFont(font);
        numRow = 0;
        QPalette p = palette();
        const QColor hlClr = "#87cefa";
        const QColor txtClr = "black";


        ui -> tableWidget_Item -> setSortingEnabled(false);
        ui -> tableWidget_Item -> setRowCount(tab.size());
        ui -> tableWidget_Item -> setColumnCount(9);
        ui -> tableWidget_Item -> setSelectionBehavior(QAbstractItemView::SelectRows);
        ui -> tableWidget_Item -> setSelectionMode(QAbstractItemView::SingleSelection);

        ui -> tableWidget_Item -> selectionModel() -> select(qmi.siblingAtRow(1), QItemSelectionModel::Rows);

        for(int i = 0; i < ui -> tableWidget_Item ->rowCount(); ++i)
        {
            for(int j = 0; j < ui -> tableWidget_Item -> columnCount(); ++j)
            {
                ui -> tableWidget_Item -> setItem(i, j, new QTableWidgetItem(QString(tab[i][j])));
            }
        }

        ui -> tableWidget_Item -> setColumnHidden(8, true);

        qsp.setHorizontalPolicy(QSizePolicy::Expanding);

        ui -> tableWidget_Item -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        p.setColor(QPalette::Highlight, hlClr);
        p.setColor(QPalette::HighlightedText, txtClr);
        setPalette(p);

        numRow = 0;
        ui -> tableWidget_Item -> setItemDelegate(m_delegate);
        isBuild = true;

        a.rCount = ui -> tableWidget_Item -> rowCount();
        date = ui -> tableWidget_Item -> item(numRow, 0) -> text();
        time = ui -> tableWidget_Item -> item(numRow, 3) -> text();
        thisRoute = ui -> tableWidget_Item -> item(numRow, 1) -> text();
        ui -> tableWidget_Item -> setSortingEnabled(true);
        ui -> tableWidget_Item -> sortByColumn(5, Qt::SortOrder::AscendingOrder);
        ui -> tableWidget_Item -> sortByColumn(1, Qt::SortOrder::AscendingOrder);

        for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); ++i)
        {
            if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
            {
                ui -> tableWidget_Item -> selectRow(i);
                break;
            }

            if(i + 1 == ui -> tableWidget_Item -> rowCount())
            {
                ui -> tableWidget_Item -> selectRow(0);
            }
        }

        ui -> tableWidget_Item -> scrollToItem(ui -> tableWidget_Item -> selectedItems().at(0));
        ui -> all -> display(ui -> tableWidget_Item -> rowCount());

        for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); ++i)
        {
            QString path;

            if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable())
                path = settingsFile -> value("SaveScreenPath").toString() + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
                       QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
                       ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
            else
                path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
                       ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
                       QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
                       ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

            if(!ui ->tableWidget_Item -> item(i, 7) -> text().isEmpty())
                count++;

            if(QDir(path).isReadable())
                ui -> tableWidget_Item -> item(i, 7) -> setText("Есть");
        }
        ui -> maked -> display(count);
        ui -> balance -> display(ui -> all -> value() - count);

        ui -> widget -> page() -> runJavaScript("$('#free-view-wrap').click()");
        paintRows();
        firstBuildedTable = true;
        setDate();
        connect(ui -> tableWidget_Item, &QTableWidget::itemChanged, this, &MainWindow::paintRows);
        startTime = QTime::currentTime();
        firstBuild = true;
        makedCount = 0;
    }
    else
        return;
}

void MainWindow::screenshot()
{
    if (a.wasRun)
    {
        numRow = a.numRow;
        a.wasRun = false;
    }
    else
        a.numRow = numRow;

    pixUpdate();

    if(isBuild)
    {
        filePath();
    }

    if(isBuild)
    {
        pixmap.save(initialPath);
        if(ui -> tableWidget_Item -> selectedItems().at(7) -> text().isEmpty())
            count++;
        ui -> tableWidget_Item -> selectedItems().at(7) -> setText("Есть");
        tab[ui -> tableWidget_Item -> selectedItems().at(0) -> row()][7] = QString("Есть");
        tableNavigate();
        buildTrack();
    }
    else
        pixmap.save("Без имени.jpg");
}

void MainWindow::video()
{
    if(ui -> tableWidget_Item -> selectedItems().at(7) -> text().isEmpty())
        count++;

    if(ui -> tableWidget_Item -> item(ui -> tableWidget_Item -> selectedItems().at(0) -> row(), 7) -> text() == "Есть")
    {
        QString path;

        if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable())
            path = settingsFile -> value("SaveScreenPath").toString() + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
                   QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
                   ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;
        else
            path = QCoreApplication::applicationDirPath() + "/" + screens + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(0) -> text() + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(1) -> text() + "/" +
                   ui -> tableWidget_Item -> selectedItems().at(3) -> text().replace(QString(":"), QString("_")) +
                   QString(" ") + ui -> tableWidget_Item -> selectedItems().at(5) -> text() + QString(" ") +
                   ui -> tableWidget_Item -> selectedItems().at(6) -> text() + "." + format;

        if(QDir(path).isReadable())
        {
            QDir dir;
            dir.remove(path);
        }
    }
    ui -> tableWidget_Item -> item(ui -> tableWidget_Item -> selectedItems().at(0) -> row(), 7) -> setText("Видео");
    tab[ui -> tableWidget_Item -> selectedItems().at(0) -> row()][7] = QString("Видео");
    tableNavigate();
    buildTrack();
}

void MainWindow::pixUpdate()
{
    QPixmap pix(ui -> widget -> size());
    ui -> widget -> render(&pix);

    pixmap = pix;
    a.pixmap = pix;

    emit a.pixUpdated();
}

void MainWindow::filePath()
{
    QTableWidgetItem *twi1 = ui -> tableWidget_Item -> item(numRow, 0);
    date = twi1 ->text();

    QTableWidgetItem *twi2 = ui -> tableWidget_Item -> item(numRow, 1);
    thisRoute = twi2 ->text();

    QTableWidgetItem *twi3 = ui -> tableWidget_Item -> item(numRow, 5);
    QString garage = twi3 ->text();

    QTableWidgetItem *twi4 = ui -> tableWidget_Item -> item(numRow, 6);
    problem = twi4 ->text();

    QTableWidgetItem *twi5 = ui -> tableWidget_Item -> item(numRow, 3);
    time = twi5 ->text();

    name = time.replace(QString(":"), QString("_")) +
           space + garage + space + problem + dot + format;

    if(QDir(settingsFile -> value("SaveScreenPath").toString()).isReadable())
        initialPath = settingsFile -> value("SaveScreenPath").toString();
    else
        initialPath = QCoreApplication::applicationDirPath();

    if(QDir(initialPath).mkdir(screens) && (initialPath == QCoreApplication::applicationDirPath()))
    {
        initialPath += slash + screens;
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else
            initialPath = QCoreApplication::applicationDirPath();
    }
    else if(QDir(initialPath).cd(screens) && initialPath == QCoreApplication::applicationDirPath())
    {
        initialPath += slash + screens;
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else
            initialPath = QCoreApplication::applicationDirPath();
    }
    else if(initialPath != QCoreApplication::applicationDirPath())
    {
        QDir(initialPath).rmdir(screens);
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
    }
    else
        initialPath = QCoreApplication::applicationDirPath();

    initialPath += slash + name;
    a.initialPath = initialPath;

    emit a.filePathCreated();
}

void MainWindow::setLoginData()
{
    if(settingsFile -> value("Login").isValid() && settingsFile -> value("Password").isValid() && (!key1 || !key2))
    {
        insert = (
            R"DELIM(
            $('#Login').val('LOGININSERT');
            $('#Password').val('PSWRDINSERT');
            $('#Key').val('KEYINSERT');
            $('#signIn').click();
            var check = false;
            var completed = false;
            )DELIM");
        insert.replace("LOGININSERT", settingsFile -> value("Login").toString());
        insert.replace("PSWRDINSERT", settingsFile -> value("Password").toString());
        insert.replace("KEYINSERT", settingsFile -> value("Key").toString());
    }
    else
    {
        enter -> setMinimumSize(410, 150);
        enter -> setMaximumSize(410, 150);
        enter -> setWindowTitle("Вход в аккаунт");
        logIn -> setPlaceholderText("Введите здесь ваш логин");
        password -> setPlaceholderText("Введите здесь ваш пароль");

        enter -> setModal(true);
        enter -> setWindowFlag(Qt::WindowStaysOnTopHint);
        enter -> show();
    }
}

void MainWindow::login()
{
    if(!logIn -> text().isEmpty() && !password -> text().isEmpty())
    {
        insert = (
            R"DELIM(
        $('#Login').val('LOGININSERT');
        $('#Password').val('PSWRDINSERT');
        $('#Key').val('KEYINSERT');
        $('#signIn').click();
        var check = false;
        var completed = false;
        )DELIM");

        insert.replace("LOGININSERT", logIn -> text());
        insert.replace("PSWRDINSERT", password -> text());
        insert.replace("KEYINSERT", keys -> currentData().toString());
        enter -> close();
        ui -> widget -> page() -> runJavaScript(insert);
        timer1 -> start(600);
    }
    else
    {
        enter -> setMinimumSize(410, 170);
        enter -> setMaximumSize(410, 170);
        label -> setText("Введите логин и пароль");
        layout -> addWidget(label, 5, 0, 1, 2);
    }
}

void MainWindow::entered()
{
    timer1 -> stop();
    ui -> widget -> page() -> runJavaScript(R"(
                                                if(document.getElementById("auth-form-container"))
                                                  check = true;)");

    checkLogin();
}

void MainWindow::checkLogin()
{
    ui -> widget -> page() -> runJavaScript(R"(check;)",
                                      [this](const QVariant &b)
                                      {
                                          if(b.toBool()) {
                                              setLoginData();
                                              layout -> removeWidget(label);
                                              enter -> setMinimumSize(410, 170);
                                              enter -> setMaximumSize(410, 170);
                                              label -> setText("Неверно введены данные");
                                              layout -> addWidget(label, 5, 0, 1, 2);
                                          }
                                          else {
                                              settingsFile -> setValue("Login", logIn -> text());
                                              settingsFile -> setValue("Password", password -> text());
                                              settingsFile -> setValue("Key", keys -> currentData().toString());
                                          }
                                      });
}

void MainWindow::web()
{
    QWebEnginePage *page = new QWebEnginePage();

    page -> load(QUrl("http://10.0.20.16/WNavSystem/kursk"));

    QObject::connect(page, &QWebEnginePage::certificateError,
                     [](QWebEngineCertificateError e) { e.acceptCertificate(); });

    ui -> widget -> setPage(page);
    ui -> widget -> show();

    setLoginData();

    QWebEngineScript logIn;
    logIn.setInjectionPoint(QWebEngineScript::DocumentReady);
    logIn.setWorldId(QWebEngineScript::MainWorld);
    logIn.setName("user_login.js");
    logIn.setRunsOnSubFrames(true);
    logIn.setSourceCode(insert);

    ui -> widget -> page() -> scripts().insert(logIn);
}

void MainWindow::tableNavigate()
{
    if (a.wasRun)
        numRow = a.numRow;
    else
        a.numRow = numRow;

    numRow = ui -> tableWidget_Item -> currentRow();
    ++numRow;

    paintRows();

    for(int i = numRow; i < ui -> tableWidget_Item -> rowCount(); ++i)
    {
        if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
        {
            ui -> tableWidget_Item -> selectRow(i);
            break;
        }
    }

    makedCount += 1;
    float speed = (QTime::currentTime().msecsSinceStartOfDay() - prevTime.msecsSinceStartOfDay()) / 1000.0;
    float midSpeed = (QTime::currentTime().msecsSinceStartOfDay() - startTime.msecsSinceStartOfDay()) / 1000.0;
    ui -> maked -> display(count);
    ui -> balance -> display(ui -> all -> value() - count);
    ui -> speed -> display(1.0 / (speed / 60.0));
    ui -> midSpeed -> display(makedCount / (midSpeed / 60.0));

    if(ui -> tableWidget_Item -> selectedItems().at(7) -> text() == QString("Есть"))
        ui -> openScreen -> setEnabled(true);
    else
        ui -> openScreen -> setEnabled(false);

    countBeforeSave++;
    if(countBeforeSave == 50)
    {
        if(QDir(table -> filepath).isReadable())
        {
            if(!table -> filepath.isEmpty())
            {
                xlnt::workbook wb;
                try {
                    wb.load(table -> filepath.toStdString());
                    auto ws = wb.active_sheet();
                    for(auto row : ws.rows(false))
                    {
                        ws.row_properties(row[0].row()).hidden = false;
                    }
                    for(int i = 0; i < tab.size(); ++i)
                    {
                        QString poz = "I" + QString::number(ui -> tableWidget_Item -> item(i, 8) -> text().toInt() + 1);
                        if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
                            continue;

                        ws.cell(poz.toStdString()).value(ui -> tableWidget_Item -> item(i, 7) -> text().toStdString());
                    }
                    wb.save(table -> filepath.toStdString());
                    countBeforeSave = 0;
                } catch (xlnt::exception) {
                    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                               tr("Ваш файл с таблицей открыт.\n Закройте файл и повторите попытку.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                               QMessageBox::Ok | QMessageBox::Cancel);
                    if(resBtn == QMessageBox::Cancel) {
                        a.setRunning(false);

                        ui -> autoBtn -> setText("Автомат");

                        ui -> screenBtn -> setEnabled(true);
                        ui -> videoBtn -> setEnabled(true);
                        ui -> buildTrack -> setEnabled(true);
                        ui -> screenBtn -> setEnabled(true);
                        ui -> loadTableBtn -> setEnabled(true);

                        disconnect(timer7, &QTimer::timeout, this, &MainWindow::signalOfTrackBuilded);
                        disconnect(this, &MainWindow::trackBuilded, this, &MainWindow::screenshot);
                    }
                }
            }
        }
        else
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                       tr("Файл перемещен, переименован или удален.\n Укажите новый путь к файлу.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                       QMessageBox::Ok | QMessageBox::Cancel);

            if(resBtn == QMessageBox::Ok) {
                table -> getpath();
            }
        }
    }
}

void MainWindow::on_autoBtn_clicked()
{
    switch (a.running())
    {
    case false:
        start();
        break;

    default:
        emit a.wait();
        stop();
        break;
    }
}

void MainWindow::start()
{
    ui -> autoBtn -> setText("Стоп");

    ui -> screenBtn -> setEnabled(false);
    ui -> videoBtn -> setEnabled(false);
    ui -> buildTrack -> setEnabled(false);
    ui -> screenBtn -> setEnabled(false);
    ui -> loadTableBtn -> setEnabled(false);

    buildTrack();

    connect(this, &MainWindow::trackBuilded, this, &MainWindow::screenshot);

    a.setRunning(true);
}

void MainWindow::stop()
{
    a.setRunning(false);

    ui -> autoBtn -> setText("Автомат");

    ui -> screenBtn -> setEnabled(true);
    ui -> videoBtn -> setEnabled(true);
    ui -> buildTrack -> setEnabled(true);
    ui -> screenBtn -> setEnabled(true);
    ui -> loadTableBtn -> setEnabled(true);

    disconnect(this, &MainWindow::trackBuilded, this, &MainWindow::screenshot);
}

void MainWindow::buildTrack()
{
    auto checkEndOfTable{[this] () {
        bool endoftable = false;
        for(int i = numRow; i < ui -> tableWidget_Item -> rowCount(); ++i)
        {
            if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
                break;
            else if(i == ui -> tableWidget_Item -> rowCount() - 1)
                endoftable = true;
        }
        return endoftable;
    }};

    if(ui -> balance -> value() == 0)
    {
        a.setRunning(false);

        ui -> autoBtn -> setText("Автомат");

        ui -> screenBtn -> setEnabled(true);
        ui -> videoBtn -> setEnabled(true);
        ui -> buildTrack -> setEnabled(true);
        ui -> screenBtn -> setEnabled(true);
        ui -> loadTableBtn -> setEnabled(true);

        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_kur 1.0",
                                                                   tr("Разбор рейсов окончен"),
                                                                   QMessageBox::Ok | QMessageBox::Ok);
    }
    else if(checkEndOfTable())
    {
        for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); ++i)
        {
            if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
            {
                numRow = i;
                ui -> tableWidget_Item -> selectRow(i);
                break;
            }
        }
        buildTrack();
    }
    else
    {
        if(ui -> tableWidget_Item -> selectedItems().at(1) -> text() != prevRoute)
        {
            json = emptyJson;
            prevRoute = ui -> tableWidget_Item -> selectedItems().at(1) -> text();
            correctPosition = true;
            setDate();
        }
        else if(correctPosition)
        {
            correctPosition = false;
            json = emptyJson;
            prevRoute = ui -> tableWidget_Item -> selectedItems().at(1) -> text();
            setDate();
        }
        else
        {
            prevRoute = ui -> tableWidget_Item -> selectedItems().at(1) -> text();
            prevTime = QTime::currentTime();

            if(ui -> tableWidget_Item -> selectedItems().at(5) -> text() != thisGarage)
            {
                json = emptyJson;
                thisGarage = ui -> tableWidget_Item -> selectedItems().at(5) -> text();
                setGarage();
            }
            else
            {
                timer3 -> start(100);
            }

            if(!a.running())
                ui ->buildTrackself -> setEnabled(true);
        }
    }
}

void MainWindow::setDate()
{
    QString setDateJs = R"(
    simulate = new MouseEvent('click', {
      shiftKey: true,
      bubbles: true
    });

    $('#tabs-page-headers')[0].children[2].children[0].click();
    $('#history-date').val('DATE');
    $('#history-tab-all').click();
    $('#load-transport-history').click();
)";

    thisDate = "";
    if(firstBuildedTable || !correctPosition)
    {
        thisDate += ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(6, 4) + "-" +
                    ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(3, 2) + "-" +
                    ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(0, 2);
        setDateJs.replace("DATE", thisDate);
    }
    else
    {
        settingsFile -> beginGroup(QString("Route_") + ui -> tableWidget_Item -> selectedItems().at(1) -> text().removeAt(2));
        thisDate += settingsFile -> value("Date").toString().sliced(6, 4) + "-" +
                    settingsFile -> value("Date").toString().sliced(3, 2) + "-" +
                    settingsFile -> value("Date").toString().sliced(0, 2);
        setDateJs.replace("DATE", thisDate);
        settingsFile -> endGroup();
    }

    ui -> widget -> page() -> runJavaScript(setDateJs);

    timer4 -> start(200);
}

void MainWindow::dateSetFunc()
{
    ui -> widget -> page()
        -> runJavaScript("$('#load-transport-history')[0].classList.length < 2",
                        [this](const QVariant &b)
                        {
                            dateSelected = b.toBool();
                        });

    if(dateSelected)
    {
        qDebug() << "ready";
        timer4 -> stop();

        factDate = ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(6, 4) + "-" +
                   ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(3, 2) + "-" +
                   ui -> tableWidget_Item -> selectedItems().at(0) -> text().sliced(0, 2);

        if(firstBuildedTable)
        {
            requestJsonRoute();
            firstBuildedTable = false;
        }
        else if(correctPosition)
        {
            ui -> widget -> page() -> runJavaScript("$('#free-view-wrap').click()");
            QTimer::singleShot(2000, this, &MainWindow::setGarage);
        }
        else
        {
            QTimer::singleShot(2000, this, &MainWindow::setGarage);
        }
    }
}

void MainWindow::setGarage()
{
    changeRouteTimerDate -> stop();
    if(factDate != thisDate)
    {
        QString setGarageJs = R"(
            $('#tabs-page-headers')[0].children[2].children[0].click();
            $("#history_select_all_ts_chosen").mousedown();
            $('.chosen-results li:contains(GARAGE )').mouseup();
            $("#history-load-navigation").click();)";

        settingsFile -> beginGroup(QString("Route_") + ui -> tableWidget_Item -> selectedItems().at(1) -> text().removeAt(2));
        setGarageJs.replace("GARAGE", settingsFile -> value("Garage").toString());
        settingsFile -> endGroup();

        ui -> widget -> page() -> runJavaScript(setGarageJs);

        timer5 -> start(150);
    }
    else
    {
        QString setGarageJs = R"(
            $('#tabs-page-headers')[0].children[2].children[0].click();
            $("#history_select_all_ts_chosen").mousedown();
            $('.chosen-results li:contains(GARAGE )').mouseup();
            $("#history-load-navigation").click();)";

        setGarageJs.replace("GARAGE", ui -> tableWidget_Item -> selectedItems().at(5) -> text());

        ui -> widget -> page() -> runJavaScript(setGarageJs);

        if(a.running())
            timer5 -> start(350);
        else
            timer5 -> start(250);
    }
}

void MainWindow::garageSet()
{
    ui -> widget -> page()
        -> runJavaScript("$('#history-load-navigation')[0].classList.length < 4",
                        [this](const QVariant &b)
                        {
                            garageSelected = b.toBool();
                        });

    if(garageSelected)
    {
        timer5 -> stop();
        requestJson();
    }
}

void MainWindow::requestJson()
{
    if(thisDate != factDate)
    {
        QString requestJsonJs = R"(
        data = [];
          var TS = {
            date: $("#history-date")[0].value,
            uniqueID: $('#history-select-all-ts option:contains(GARAGE )')[0].value
          }
          var response = fetch('http://10.0.20.16/WNavSystem/kursk/Map/GetHistoryNavigation', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json; charset=utf-8'
            },
            body: JSON.stringify(TS),
          }).then(async (response) => {
            data = await response.json();
          }))";

        QString route = ui -> tableWidget_Item -> selectedItems().at(1) -> text().replace("Г", "");
        route = route.replace("М", "");
        settingsFile -> beginGroup(QString("Route_") + route);
        requestJsonJs.replace("GARAGE", settingsFile -> value("Garage").toString());
        settingsFile -> endGroup();

        ui -> widget -> page() -> runJavaScript(requestJsonJs);

        timer2 -> start(200);
    }
    else
    {
        QString requestJsonJs = R"(
        data = [];
          var TS = {
            date: $("#history-date")[0].value,
            uniqueID: $('#history-select-all-ts option:contains(GARAGE )')[0].value
          }
          var response = fetch('http://10.0.20.16/WNavSystem/kursk/Map/GetHistoryNavigation', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json; charset=utf-8'
            },
            body: JSON.stringify(TS),
          }).then(async (response) => {
            data = await response.json();
          }))";

        requestJsonJs.replace("GARAGE", ui -> tableWidget_Item -> selectedItems().at(5) -> text());

        ui -> widget -> page() -> runJavaScript(requestJsonJs);

        if(a.running())
            timer2 -> start(300);
        else
            timer2 -> start(200);
    }
}

void MainWindow::responseJson()
{
    ui -> widget -> page() -> runJavaScript(R"(data;)",
                                      [this](const QVariant &b)
                                      {
                                          json = b.toJsonArray();
                                      });

    if(!json.isEmpty())
    {
        timer2 -> stop();
        ui -> widget -> page() ->runJavaScript("$('#tabs-page3')[0].scrollTo(0,0);");

        if(a.running())
            timer3 -> start(200);
        else
            timer3 -> start(300);
    }
}
void MainWindow::selectedItems()
{
    ui ->buildTrackself -> setEnabled(false);
    numRow = ui -> tableWidget_Item -> selectedItems().at(0) -> row();
    if(ui -> tableWidget_Item -> selectedItems().at(7) -> text() == QString("Есть"))
        ui -> openScreen -> setEnabled(true);
    else
        ui -> openScreen -> setEnabled(false);
}

void MainWindow::requestJsonRoute()
{
    QString variable = R"(
var routeData;
var body = {date: $("#history-date")[0].value,
  routeRegNums: [$('#history-select-mr option:contains(10)')[0].value,
                $('#history-select-mr option:contains(15)')[0].value,
                $('#history-select-mr option:contains(18)')[0].value,
                $('#history-select-mr option:contains(22)')[0].value,
                $('#history-select-mr option:contains(28)')[0].value,
                $('#history-select-mr option:contains(35)')[0].value,
                $('#history-select-mr option:contains(41)')[0].value,
                $('#history-select-mr option:contains(42)')[0].value,
                $('#history-select-mr option:contains(46)')[0].value,
                $('#history-select-mr option:contains(50)')[0].value,
                $('#history-select-mr option:contains(59)')[0].value,
                $('#history-select-mr option:contains(73)')[0].value,
                $('#history-select-mr option:contains(80)')[0].value,
                $('#history-select-mr option:contains(82)')[0].value,
                $('#history-select-mr option:contains(85)')[0].value,
                $('#history-select-mr option:contains(93)')[0].value,
                $('#history-select-mr option:contains(94)')[0].value,
                $('#history-select-mr option:contains(98)')[0].value,
                $('#history-select-mr option:contains(99)')[0].value]
            };)";

    QString requestJsonRouteJs = R"(

async function fetchRoutes() {
    const response = await fetch('http://10.0.20.16/WNavSystem/kursk/Map/GetRouteVariantList', {
        method: 'POST',
        body: JSON.stringify(body),
        headers: {
            'Content-Type': 'application/json; charset=utf-8'
        },
    });
    const json = await response.json();
    return json;
}
fetchRoutes().then(json => {routeData = json});)";

    ui -> widget -> page() -> runJavaScript(variable);
    ui -> widget -> page() -> runJavaScript(requestJsonRouteJs);

    timer6 -> start(200);
}

void MainWindow::responseJsonRoute()
{
    ui -> widget -> page() -> runJavaScript(R"(routeData.stopStationList;)",
                                      [this](const QVariant &b)
                                      {
                                          stopStationList = b.toJsonArray();
                                      });

    ui -> widget -> page() -> runJavaScript(R"(routeData.routeVariantList)",
                                      [this](const QVariant &b)
                                      {
                                          routeVariantList = b.toJsonArray();
                                      });

    if(!stopStationList.isEmpty() && !routeVariantList.isEmpty())
    {
        timer6 -> stop();

        mapOfCars["10М"] = 0;
        mapOfCars["15М"] = 1;
        mapOfCars["18Г"] = 2;
        mapOfCars["22Г"] = 3;
        mapOfCars["28М"] = 4;
        mapOfCars["35М"] = 5;
        mapOfCars["41"] = 6;
        mapOfCars["42М"] = 7;
        mapOfCars["46"] = 8;
        mapOfCars["50"] = 9;
        mapOfCars["59Г"] = 10;
        mapOfCars["73М"] = 11;
        mapOfCars["80М"] = 12;
        mapOfCars["82М"] = 13;
        mapOfCars["85М"] = 14;
        mapOfCars["93М"] = 15;
        mapOfCars["94Г"] = 16;
        mapOfCars["98Г"] = 17;
        mapOfCars["99М"] = 18;
        \
        ui -> buildTrack -> setEnabled(true);
        ui -> autoBtn -> setEnabled(true);
        ui -> videoBtn -> setEnabled(true);
        ui -> buildRoute -> setEnabled(true);
    }
}

int MainWindow::point_index(QJsonArray arr, QString timeOfRace, double sclot, double sclon,
                             bool (*ch)(double, double, double, double, double, double))
{
    int startPoint = 0;
    int zoneA = 0, zoneB = 0, zoneC = 0, zoneD = 0, zoneE = 0, zoneF = 0;
    QTime raceTimeMin = QTime::fromString(timeOfRace, "hh:mm");
    QTime raceTimeMax = QTime::fromString(timeOfRace, "hh:mm");

    if(ui -> tableWidget_Item -> selectedItems().at(6) -> text() == QString("РО") ||
        ui -> tableWidget_Item -> selectedItems().at(6) -> text().toLower() == QString("ракетная опасность"))
    {
        if(raceTimeMin.minute() < 30)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 30, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 30, 0);

        if(raceTimeMax.minute() > 30)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 30, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 30, 0);
    }
    else
    {
        if(raceTimeMin.minute() < 15)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 45, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 15, 0);

        if(raceTimeMax.minute() > 45)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 45, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 15, 0);
    }

    for(int i = 0; i < arr.size(); ++i)
    {
        QString timeNav = arr[i].toObject()["timeNav"].toString();
        timeNav = timeNav.sliced(11);
        timeNav = timeNav.sliced(0, 5);
        QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
        pointTimeNav.setHMS(pointTimeNav.hour(), pointTimeNav.minute(), 0);

        if(pointTimeNav > raceTimeMax)
        {
            break;
        }
        else if(pointTimeNav < raceTimeMax && pointTimeNav > raceTimeMin)
        {
            if(ch(0.0001, 0.00016, sclot, sclon,
                      json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                startPoint = i;
                break;
            }
            else if(ch(0.0005, 0.0008, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneA = i;
                startPoint = zoneA;
                continue;
            }
            else if(ch(0.001, 0.0016, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneB = i;
                if(zoneA != 0 && zoneB > zoneA)
                {
                    continue;
                }
                else
                    startPoint = zoneB;
            }
            else if(ch(0.0015, 0.0029, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneC = i;
                if(zoneB != 0 && zoneC > zoneB)
                {
                    continue;
                }
                else
                    startPoint = zoneC;
            }
            else if(ch(0.0026, 0.0042, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneD = i;
                if(zoneC != 0 && zoneD > zoneC)
                {
                    continue;
                }
                else
                    startPoint = zoneD;
            }
            else if(ch(0.0035, 0.0056, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneE = i;
                if(zoneD != 0 && zoneE > zoneD)
                {
                    continue;
                }
                else
                    startPoint = zoneE;
            }
            else if(ch(0.0047, 0.0075, sclot, sclon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneF = i;
                if(zoneE != 0 && zoneF > zoneE)
                {
                    continue;
                }
                else
                    startPoint = zoneF;
            }
        }
        else {
            continue;
        }
    }

    if(startPoint == 0 && (ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("41") ||
                            ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("46") ||
                            ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("50")))
    {
        for(int i = 0; i < arr.size(); ++i)
        {
            QString timeNav = arr[i].toObject()["timeNav"].toString();
            timeNav = timeNav.sliced(11);
            timeNav = timeNav.sliced(0, 5);
            QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
            pointTimeNav.setHMS(pointTimeNav.hour(), pointTimeNav.minute(), 0);

            if(pointTimeNav > raceTimeMax)
            {
                break;
            }
            else if(pointTimeNav < raceTimeMax && pointTimeNav > raceTimeMin)
            {
                startPoint = i;
                break;
            }
            else {
                continue;
            }
        }
    }
    return startPoint;
}

void MainWindow::setTimeInterval()
{
    timer3 -> stop();

    auto check{ [] (double latErr, double lonErr, double sclat, double sclon,
                  double lat, double lon) {return (((((int)(sclat * 10000 + 0.5) / 10000.0) - latErr <=  lat) &&
                                          (lat <= ((int)(sclat * 10000 + 0.5) / 10000.0) + latErr)) &&
                                          ((((int)(sclon * 10000 + 0.5) / 10000.0) - lonErr <= lon) &&
                                          (lon <= ((int)(sclon * 10000 + 0.5) / 10000.0) + lonErr)));}};


    startPoint = 0;
    int poz;

    if(ui -> tableWidget_Item -> selectedItems().at(4) -> text().isEmpty())
        poz = 3;
    else
        poz = 4;

    int startStop;

    if(ui -> tableWidget_Item -> selectedItems().at(2) -> text() == QString("Вперед"))
    {
        startStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                    toObject()["raceList"].toArray()[0].toObject()["stopStationList"][0].toInt();
    }
    else
    {
        startStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                    toObject()["raceList"].toArray()[1].toObject()["stopStationList"][0].toInt();
    }

    double startCoordinationLat, startCoordinationLon;

    for(short i = 0; i < stopStationList.size(); ++i)
    {
        if(stopStationList[i].toObject()["regNum"] == startStop)
        {
            startCoordinationLat = stopStationList[i].toObject()["location"][0].toDouble();
            startCoordinationLon = stopStationList[i].toObject()["location"][1].toDouble();
            break;
        }
    }

    if(factDate != thisDate)
    {
        QString route = ui -> tableWidget_Item -> selectedItems().at(1) -> text().replace("Г", "");
        route = route.replace("М", "");
        settingsFile -> beginGroup(QString("Route_") + route);
        startPoint = point_index(json, settingsFile -> value("Time").toString(), startCoordinationLat, startCoordinationLon, check);
        settingsFile -> endGroup();
    }
    else
        startPoint = point_index(json, ui -> tableWidget_Item -> selectedItems().at(poz) -> text(), startCoordinationLat, startCoordinationLon, check);

    qsizetype ls;
    int lastStop;

    if(correctPosition)
    {
        QString route = ui -> tableWidget_Item -> selectedItems().at(1) -> text().replace("Г", "");
        route = route.replace("М", "");
        settingsFile -> beginGroup(QString("Route_") + route);
        if(settingsFile -> value("Dest") == QString("A"))
        {
            ls = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                 toObject()["raceList"].toArray()[0].toObject()["stopStationList"].toArray().size();

            lastStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                       toObject()["raceList"].toArray()[0].toObject()["stopStationList"].toArray().at(ls - 1).toInt();
        }
        else
        {
            ls = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                 toObject()["raceList"].toArray()[1].toObject()["stopStationList"].toArray().size();

            lastStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                       toObject()["raceList"].toArray()[1].toObject()["stopStationList"].toArray().at(ls - 1).toInt();
        }
        settingsFile -> endGroup();
    }
    else
    {
        if(ui -> tableWidget_Item -> selectedItems().at(2) -> text() == QString("Вперед"))
        {
            ls = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                           toObject()["raceList"].toArray()[0].toObject()["stopStationList"].toArray().size();

            lastStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                           toObject()["raceList"].toArray()[0].toObject()["stopStationList"].toArray().at(ls - 1).toInt();
        }
        else
        {
            ls = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                           toObject()["raceList"].toArray()[1].toObject()["stopStationList"].toArray().size();

            lastStop = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                           toObject()["raceList"].toArray()[1].toObject()["stopStationList"].toArray().at(ls - 1).toInt();
        }
    }

    double stopCoordinateLat = 0.0,
        stopCoordinateLon = 0.0;

    for(short i = 0; i < stopStationList.size(); ++i)
    {
        if(stopStationList[i].toObject()["regNum"] == lastStop)
        {
            stopCoordinateLat = stopStationList[i].toObject()["location"][0].toDouble();
            stopCoordinateLon = stopStationList[i].toObject()["location"][1].toDouble();
            break;
        }
    }

    pointFinishIndex = 0;

    bool inZone = false;
    int zoneA = 0, zoneB = 0, zoneC = 0, zoneD = 0, zoneE = 0, zoneF = 0;
    if(routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
        toObject()["raceList"].toArray()[0].toObject()["stopStationList"].toArray().at(0).toInt() != lastStop)
    {
        for(int i = startPoint; i < json.size(); ++i)
        {
            if(check(0.01, 0.016, stopCoordinateLat, stopCoordinateLon,
                      json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                inZone = true;
            }
            else if(!check(0.013, 0.016, stopCoordinateLat, stopCoordinateLon,
                              json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()) && inZone)
            {
                break;
            }

            if(check(0.0001, 0.00016, stopCoordinateLat, stopCoordinateLon,
                    json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                pointFinishIndex = i;
                break;
            }
            else if(check(0.0005, 0.0008, stopCoordinateLat, stopCoordinateLon,
                        json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneA = i;
                pointFinishIndex = zoneA;
                continue;
            }
            else if(check(0.001, 0.0016, stopCoordinateLat, stopCoordinateLon,
                             json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneB = i;
                if(zoneA != 0 && zoneB > zoneA)
                {
                    pointFinishIndex = zoneA;
                    continue;
                }
            }
            else if(check(0.0015, 0.0029, stopCoordinateLat, stopCoordinateLon,
                             json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneC = i;
                if(zoneB != 0 && zoneC > zoneB)
                {
                    pointFinishIndex = zoneB;
                    continue;
                }
            }
            else if(check(0.0026, 0.0042, stopCoordinateLat, stopCoordinateLon,
                             json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneD = i;
                if(zoneC != 0 && zoneD > zoneC)
                {
                    pointFinishIndex = zoneC;
                    continue;
                }
            }
            else if(check(0.0035, 0.0056, stopCoordinateLat, stopCoordinateLon,
                             json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneE = i;
                if(zoneD != 0 && zoneE > zoneD)
                {
                    pointFinishIndex = zoneC;
                    continue;
                }
            }
            else if(check(0.0047, 0.0075, stopCoordinateLat, stopCoordinateLon,
                             json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneF = i;
                if(zoneE != 0 && zoneF > zoneE)
                {
                    pointFinishIndex = zoneE;
                    continue;
                }
                else if(zoneE == 0)
                    pointFinishIndex = zoneF;
            }
        }
    }
    else
    {
        for(int i = startPoint + 300; i < json.size(); ++i)
        {
            if(check(0.01, 0.016, stopCoordinateLat, stopCoordinateLon,
                      json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                inZone = true;
            }
            else if(!check(0.013, 0.016, stopCoordinateLat, stopCoordinateLon,
                            json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()) && inZone)
            {
                break;
            }

            if(check(0.0001, 0.00016, stopCoordinateLat, stopCoordinateLon,
                      json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                pointFinishIndex = i;
                break;
            }
            else if(check(0.0005, 0.0008, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneA = i;
                pointFinishIndex = zoneA;
                continue;
            }
            else if(check(0.001, 0.0016, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneB = i;
                if(zoneA != 0 && zoneB > zoneA)
                {
                    pointFinishIndex = zoneA;
                    continue;
                }
            }
            else if(check(0.0015, 0.0029, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneC = i;
                if(zoneB != 0 && zoneC > zoneB)
                {
                    pointFinishIndex = zoneB;
                    continue;
                }
            }
            else if(check(0.0026, 0.0042, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneD = i;
                if(zoneC != 0 && zoneD > zoneC)
                {
                    pointFinishIndex = zoneC;
                    continue;
                }
            }
            else if(check(0.0035, 0.0056, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneE = i;
                if(zoneD != 0 && zoneE > zoneD)
                {
                    pointFinishIndex = zoneC;
                    continue;
                }
            }
            else if(check(0.0047, 0.0075, stopCoordinateLat, stopCoordinateLon,
                           json[i].toObject()["lat"].toDouble(), json[i].toObject()["lon"].toDouble()))
            {
                zoneF = i;
                if(zoneE != 0 && zoneF > zoneE)
                {
                    pointFinishIndex = zoneE;
                    continue;
                }
                else if(zoneE == 0)
                    pointFinishIndex = zoneF;
            }
        }
    }

    if(ui -> autoCheck -> isChecked() || a.running())
    {
        if(ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("41") ||
           ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("46") ||
           ui -> tableWidget_Item -> selectedItems().at(1) -> text() == QString("50") || correctPosition)
        {
            if(a.running())
            {
                if((pointFinishIndex != 0 && startPoint != 0 && pointFinishIndex - startPoint < 2300) || correctPosition)
                    chooseTimeInterval();
                else if(startPoint != 0)
                {
                    bool direction = 0;

                    std::vector<int> arrayOfIds;
                    if(ui -> tableWidget_Item -> selectedItems().at(2) -> text() == QString("Вперед"))
                        direction = 0;
                    else
                        direction = 1;

                    for(int i = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                                 toObject()["raceList"].toArray()[direction].toObject()["pointList"].toArray().size() - 1; i >= 0; --i)
                    {
                        stopCoordinateLat = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                                            toObject()["raceList"].toArray()[direction].toObject()["pointList"].toArray()[i][0].toDouble();

                        stopCoordinateLon = routeVariantList[mapOfCars[ui -> tableWidget_Item -> selectedItems().at(1) -> text()]].
                                            toObject()["raceList"].toArray()[direction].toObject()["pointList"].toArray()[i][1].toDouble();

                        if(startPoint + 2300 <= json.size())
                        {
                            for(int j = startPoint; j < startPoint + 2300; ++j)
                            {
                                if(check(0.001, 0.0016, stopCoordinateLat, stopCoordinateLon,
                                          json[j].toObject()["lat"].toDouble(), json[j].toObject()["lon"].toDouble()))
                                {
                                    arrayOfIds.push_back(j);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            for(int j = startPoint; j < json.size(); ++j)
                            {
                                if(check(0.001, 0.0016, stopCoordinateLat, stopCoordinateLon,
                                          json[j].toObject()["lat"].toDouble(), json[j].toObject()["lon"].toDouble()))
                                {
                                    arrayOfIds.push_back(j);
                                    break;
                                }
                            }
                        }
                    }

                    std::sort(arrayOfIds.begin(), arrayOfIds.end());

                    for(int i = 0; i < arrayOfIds.size(); ++i)
                    {
                        if(arrayOfIds[i + 1] - arrayOfIds[i] > 200)
                        {
                            pointFinishIndex = arrayOfIds[i];
                            break;
                        }
                        else if(i == arrayOfIds.size() - 1)
                        {
                            pointFinishIndex = arrayOfIds[i];
                            break;
                        }
                    }

                    if(pointFinishIndex == 0)
                    {
                        ui -> tableWidget_Item -> item(ui -> tableWidget_Item -> selectedItems().at(0) -> row(), 7) -> setText("Видео");
                        tableNavigate();
                        QTimer::singleShot(300, this, &MainWindow::buildTrack);
                    }
                    else
                        chooseTimeInterval();
                }
                else
                {
                    ui -> tableWidget_Item -> item(ui -> tableWidget_Item -> selectedItems().at(0) -> row(), 7) -> setText("Видео");
                    tableNavigate();
                    QTimer::singleShot(300, this, &MainWindow::buildTrack);
                }
            }
            else
                chooseTimeInterval();
        }
        else
        {
            if(pointFinishIndex != 0 && startPoint != 0 && pointFinishIndex - startPoint < 1830)
                chooseTimeInterval();
            else
            {
                ui -> tableWidget_Item -> item(ui -> tableWidget_Item -> selectedItems().at(0) -> row(), 7) -> setText("Видео");
                tableNavigate();
                QTimer::singleShot(300, this, &MainWindow::buildTrack);
            }
        }
    }
    else
        chooseTimeInterval();
}

void MainWindow::chooseTimeInterval()
{

    QString chooseTIJs = R"(
    $(`#history-navigation-table tbody [index = STARTINDEX]`).click()
    $(`#history-navigation-table tbody [index = FINISHINDEX]`)[0].dispatchEvent(simulate))";

    chooseTIJs.replace("STARTINDEX", QString::fromStdString(std::to_string(startPoint)));
    chooseTIJs.replace("FINISHINDEX", QString::fromStdString(std::to_string(pointFinishIndex)));
    ui -> widget -> page() -> runJavaScript(chooseTIJs);

    if(thisDate != factDate)
    {
        ui -> widget -> page() -> runJavaScript("$('#free-view-wrap').click()");
    }

    buildRoute();
}

void MainWindow::buildRoute()
{

    QString chooseRoute = R"(
    $('#tabs-page1 div:contains(ROUTE)')[0].click()

    $('#choose-transport-action-tracking-marsh')[0].click()

    $('#choose-transport-action-dialog')[0].click()

    var e = document.getElementsByClassName("modal-backdrop")
    e[0].remove()

    $('#map')[0].children[3].children[5].style.display = "none"

    $('#map')[0].children[3].children[6].style.display = "block"
    )";

    chooseRoute.replace("ROUTE", ui -> tableWidget_Item -> selectedItems().at(1) -> text());
    ui -> widget -> page() -> runJavaScript(chooseRoute);
    firstBuild = false;

    if(firstBuild || correctPosition)
    {
        QTimer::singleShot(1100, this, &MainWindow::buildTrack);
        qDebug() << "next track";
    }
    else if(a.running())
    {
        QTimer::singleShot(1300, this, &MainWindow::signalOfTrackBuilded);
    }
}

void MainWindow::buildTrackSelf()
{
    ui -> widget -> page() -> runJavaScript("$('#free-view-wrap').click()");

    startPoint = 0;
    pointFinishIndex = 0;

    settingsFile -> beginGroup("TimeBuffer");
    QTime raceTimeMin = QTime::fromString(settingsFile -> value("TimeStart").toString(), "hh:mm");
    QTime raceTimeMax = QTime::fromString(settingsFile -> value("TimeStart").toString(), "hh:mm");
    settingsFile -> endGroup();

    if(ui -> tableWidget_Item -> selectedItems().at(6) -> text() == QString("РО") ||
        ui -> tableWidget_Item -> selectedItems().at(6) -> text().toLower() == QString("ракетная опасность"))
    {
        if(raceTimeMin.minute() < 30)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 30, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 30, 0);

        if(raceTimeMax.minute() > 30)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 30, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 30, 0);
    }
    else
    {
        if(raceTimeMin.minute() < 5)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 55, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 5, 0);

        if(raceTimeMax.minute() > 55)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 55, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 5, 0);
    }

    for(int i = 0; i < json.size(); ++i)
    {
        QString timeNav = json[i].toObject()["timeNav"].toString();
        timeNav = timeNav.sliced(11);
        timeNav = timeNav.sliced(0, 5);
        QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
        pointTimeNav.setHMS(pointTimeNav.hour(), pointTimeNav.minute(), 0);

        if(pointTimeNav > raceTimeMax)
        {
            break;
        }
        else if(pointTimeNav < raceTimeMax && pointTimeNav > raceTimeMin)
        {
            startPoint = i;
            break;
        }
        else {
            continue;
        }
    }

    settingsFile -> beginGroup("TimeBuffer");
    raceTimeMin = QTime::fromString(settingsFile -> value("TimeEnd").toString(), "hh:mm");
    raceTimeMax = QTime::fromString(settingsFile -> value("TimeEnd").toString(), "hh:mm");
    settingsFile -> endGroup();

    if(ui -> tableWidget_Item -> selectedItems().at(6) -> text() == QString("РО") ||
        ui -> tableWidget_Item -> selectedItems().at(6) -> text().toLower() == QString("ракетная опасность"))
    {
        if(raceTimeMin.minute() < 30)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 30, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 30, 0);

        if(raceTimeMax.minute() > 30)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 30, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 30, 0);
    }
    else
    {
        if(raceTimeMin.minute() < 10)
            raceTimeMin.setHMS(raceTimeMin.hour() - 1, raceTimeMin.minute() + 50, 0);
        else
            raceTimeMin.setHMS(raceTimeMin.hour(), raceTimeMin.minute() - 10, 0);

        if(raceTimeMax.minute() > 50)
            raceTimeMax.setHMS(raceTimeMax.hour() + 1, raceTimeMax.minute() - 50, 0);
        else
            raceTimeMax.setHMS(raceTimeMax.hour(), raceTimeMax.minute() + 10, 0);
    }

    for(int i = startPoint; i < json.size(); ++i)
    {
        QString timeNav = json[i].toObject()["timeNav"].toString();
        timeNav = timeNav.sliced(11);
        timeNav = timeNav.sliced(0, 5);
        QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
        pointTimeNav.setHMS(pointTimeNav.hour(), pointTimeNav.minute(), 0);

        if(pointTimeNav > raceTimeMax)
        {
            break;
        }
        else if(pointTimeNav < raceTimeMax && pointTimeNav > raceTimeMin)
        {
            pointFinishIndex = i;
            continue;
        }
        else {
            continue;
        }
    }

    chooseTimeInterval();
}

void MainWindow::checkTrack()
{
    ui -> widget -> page()
        -> runJavaScript("$('.history-selected-row').length",
                        [this](const QVariant &b)
                        {
                            int length = b.toInt();
                            if(length > 100)
                            {
                                timer7 -> stop();
                                timer8 -> start(150);
                            }
                        });
}

void MainWindow::signalOfTrackBuilded()
{
    timer8 -> stop();
    emit trackBuilded();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QDir(table -> filepath).isReadable())
    {
        if(!table -> filepath.isEmpty())
        {
            xlnt::workbook wb;
            try {
                wb.load(table -> filepath.toStdString());
                auto ws = wb.active_sheet();
                for(auto row : ws.rows(false))
                {
                    ws.row_properties(row[0].row()).hidden = false;
                }
                for(int i = 0; i < tab.size(); ++i)
                {
                    QString poz = "I" + QString::number(ui -> tableWidget_Item -> item(i, 8) -> text().toInt() + 1);
                    if(ui -> tableWidget_Item -> item(i, 7) -> text().isEmpty())
                        continue;
                    ws.cell(poz.toStdString()).value(ui -> tableWidget_Item -> item(i, 7) -> text().toStdString());
                }
                wb.save(table -> filepath.toStdString());
            } catch (xlnt::exception) {
                QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                           tr("Ваш файл с таблицей открыт.\n Закройте файл и повторите попытку.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                           QMessageBox::Ok | QMessageBox::Cancel);

                if(resBtn == QMessageBox::Ok) {
                    event -> ignore();
                } else {
                    event -> accept();
                }
            }
        }
        else
            event -> accept();
    }
    else
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "AutoScreenshooter_ver_2",
                                                                   tr("Файл перемещен, переименован или удален.\n Укажите новый путь к файлу.\n При нажатии 'Cancel' данные будут утеряны."),
                                                                   QMessageBox::Ok | QMessageBox::Cancel);

        if(resBtn == QMessageBox::Ok) {
            event -> ignore();
            table -> getpath();
        } else {
            event -> accept();
        }
    }

    ui -> widget -> page() -> runJavaScript("$('#menu-item-logout').click()");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << QKeySequence(event -> key()).toString();
    if(event -> key() == settingsFile -> value("ScreenKey")  && ui -> screenBtn -> isEnabled())
        screenshot();

    if(event -> key() == settingsFile -> value("VideoKey") && ui -> videoBtn -> isEnabled())
        video();

    if(event -> key() == settingsFile -> value("BuildTrackKey")  && ui -> buildTrack -> isEnabled())
        buildTrack();

    /*if(viewer -> isActiveWindow())
    {
        if(event -> key() == settingsFile -> value("PrevKey"))
            prevImage();

        if(event -> key() == settingsFile -> value("NextKey"))
            nextImage();

        if(event -> key() == settingsFile -> value("DeleteKey"))
            deleteScreen();

        if(event -> key() == settingsFile -> value("VideoScrKey"))
            setVid();
    }*/
}
