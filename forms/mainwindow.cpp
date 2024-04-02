#include "mainwindow.h"
#include "classes/screenshot.h"
#include "classes/table.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton *ltb = ui ->loadTableBtn;
    QPushButton *screen = ui -> screenBtn;
    ltb -> setAutoDefault(false);
    screen -> setAutoDefault(false);
    QObject::connect(ltb, &QPushButton::clicked, this, &MainWindow::loadTable);
    QObject::connect(screen, &QPushButton::clicked, this, &MainWindow::screenshot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadTable()
{
    Table *table = new Table;
    table -> getpath();
    if(table -> check(table->filepath))
    {
        table -> readtable();
    }
    else
        return;
}

void MainWindow::screenshot()
{
    Screenshot *scr = new Screenshot;
    scr -> shootScreen();
    scr -> saveScreenshot();
}

