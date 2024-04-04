#include "mainwindow.h"
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
    if(!tab.empty())
    {
        numRow = 0;
        m_delegate -> setRows({});
        QPalette p = palette();
        QVector<int> rws;
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
            QString check = ui -> tableWidget_Item -> item(numRow, 7) -> text();
            if(check == QString("Есть") || check == QString("Видео") || check == QString("ГеоДанные"))
            {
                numRow += 1;
                rws.push_back(i);
            }
            else
                numRow += 1;
        }

        m_delegate -> setRows(rws);
        ui -> tableWidget_Item -> update();

        qsp.setHorizontalPolicy(QSizePolicy::Expanding);

        ui -> tableWidget_Item -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

        p.setColor(QPalette::Highlight, hlClr);
        p.setColor(QPalette::HighlightedText, txtClr);
        setPalette(p);

        ui -> tableWidget_Item -> setItemDelegate(m_delegate);
        ui -> tableWidget_Item -> selectRow(numRow);
        isBuild = true;
    }
    else
        return;
}

void MainWindow::screenshot()
{
    QPixmap pixmap(ui -> widget -> size());
    ui -> widget -> render(&pixmap);

    QString date,
            route,
            garage,
            problem,
            time;

    if(isBuild)
    {
        QTableWidgetItem *twi1 = ui -> tableWidget_Item -> item(numRow, 0);
        date = twi1 ->text();

        QTableWidgetItem *twi2 = ui -> tableWidget_Item -> item(numRow, 1);
        route = twi2 ->text();

        QTableWidgetItem *twi3 = ui -> tableWidget_Item -> item(numRow, 5);
        garage = twi3 ->text();

        QTableWidgetItem *twi4 = ui -> tableWidget_Item -> item(numRow, 6);
        problem = twi4 ->text();

        QTableWidgetItem *twi5 = ui -> tableWidget_Item -> item(numRow, 3);
        time = twi5 ->text();
    }

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

    if(isBuild)
    {
        pixmap.save(initialPath);
        QVector<int> rws = m_delegate -> rows();

        rws.push_back(numRow);
        ++numRow;

        m_delegate -> setRows(rws);
        ui -> tableWidget_Item -> update();

        ui -> tableWidget_Item -> selectRow(numRow);
        if(numRow + 2 != ui -> tableWidget_Item -> rowCount())
            ui -> tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(numRow + 2, 0));
        else if(numRow + 1 != ui -> tableWidget_Item -> rowCount())
            ui -> tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(numRow + 1, 0));
        else if(numRow != ui -> tableWidget_Item -> rowCount())
            ui -> tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(numRow, 0));
        else
            ui ->tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(
                    ui ->tableWidget_Item->rowCount(), 0));
    }
    else
        pixmap.save("Без имени");
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
