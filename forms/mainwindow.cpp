#include "mainwindow.h"
#include "classes/screenshot.h"
#include "classes/table.h"
#include "ui_mainwindow.h"
#include "classes/delegate.h"
#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton *ltb = ui ->loadTableBtn;
    QPushButton *screen = ui -> screenBtn;
    QPushButton *autoBtn = ui -> autoBtn;
    ltb -> setAutoDefault(false);
    screen -> setAutoDefault(false);
    QObject::connect(ltb, &QPushButton::clicked, this, &MainWindow::buildTable);
    QObject::connect(screen, &QPushButton::clicked, this, &MainWindow::screenshot);
    QObject::connect(autoBtn, &QPushButton::clicked, this, &MainWindow::autoscr);
    web();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTable()
{
    QModelIndex qmi;
    Table *table = new Table;
    QSizePolicy qsp;
    QVector<QVector<QString>> tab = table ->readtable();
    QPalette p = palette();
    const QColor hlClr = "#87cefa";
    const QColor txtClr = "black";

    ui -> tableWidget_Item -> setRowCount(tab.size());
    ui -> tableWidget_Item -> setColumnCount(8);
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

    qsp.setHorizontalPolicy(QSizePolicy::Expanding);

    ui -> tableWidget_Item -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    p.setColor(QPalette::Highlight, hlClr);
    p.setColor(QPalette::HighlightedText, txtClr);
    setPalette(p);

    ui -> tableWidget_Item -> setItemDelegate(m_delegate);
    ui -> tableWidget_Item -> selectRow(numRow);
}

void MainWindow::screenshot()
{
    Screenshot *scr = new Screenshot;

    QTableWidgetItem *twi1 = ui -> tableWidget_Item -> item(numRow, 0);
    QString date = twi1 ->text();

    QTableWidgetItem *twi2 = ui -> tableWidget_Item -> item(numRow, 1);
    QString route = twi2 ->text();

    QTableWidgetItem *twi3 = ui -> tableWidget_Item -> item(numRow, 5);
    QString garage = twi3 ->text();

    QTableWidgetItem *twi4 = ui -> tableWidget_Item -> item(numRow, 6);
    QString problem = twi4 ->text();

    QTableWidgetItem *twi5 = ui -> tableWidget_Item -> item(numRow, 3);
    QString time = twi5 ->text();

    scr -> shootScreen();
    scr -> saveScreenshot(date, route, garage, time, problem);

    QVector<int> rws = m_delegate -> rows();

    rws.push_back(numRow);
    ++numRow;

    m_delegate -> setRows(rws);
    ui -> tableWidget_Item -> update();

    ui -> tableWidget_Item -> selectRow(numRow);
    ui -> tableWidget_Item -> scrollToItem(
        ui -> tableWidget_Item -> item(numRow + 2, 2));

    Sleep(1000);
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

void MainWindow::autoscr()
{
    for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); i++)
    {
        screenshot();
    }
}
