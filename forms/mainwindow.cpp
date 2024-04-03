#include "mainwindow.h"
#include "classes/screenshot.h"
#include "classes/table.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtWebEngineWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton *ltb = ui ->loadTableBtn;
    QPushButton *screen = ui -> screenBtn;
    ltb -> setAutoDefault(false);
    screen -> setAutoDefault(false);
    QObject::connect(ltb, &QPushButton::clicked, this, &MainWindow::buildTable);
    QObject::connect(screen, &QPushButton::clicked, this, &MainWindow::screenshot);
    web();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTable()
{
    Table *table = new Table;
    QSizePolicy qsp;
    QVector<QVector<QString>> tab = table ->readtable();

    ui -> tableWidget_Item -> setRowCount(tab.size());
    ui -> tableWidget_Item -> setColumnCount(8);

    for(int i = 0; i < ui -> tableWidget_Item ->rowCount(); ++i)
    {
        for(int j = 0; j < ui -> tableWidget_Item -> columnCount(); ++j)
        {
            ui -> tableWidget_Item -> setItem(i, j, new QTableWidgetItem(QString(tab[i][j])));
        }
    }

    qsp.setHorizontalPolicy(QSizePolicy::Expanding);
    ui -> tableWidget_Item -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::screenshot()
{
    Screenshot *scr = new Screenshot;
    scr -> shootScreen();
    scr -> saveScreenshot();
}

void MainWindow::web()
{
    QWebEnginePage *page = new QWebEnginePage();

    page -> load(QUrl("https://webnavlo.nta.group/WNavSystemB"));

    QObject::connect(page, &QWebEnginePage::certificateError,
                     [](QWebEngineCertificateError e) { e.acceptCertificate(); });


    ui -> widget -> setPage(page);
    ui -> widget -> show();
}
