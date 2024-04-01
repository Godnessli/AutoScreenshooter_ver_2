#include "mainwindow.h"
#include "classes/table.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton *ltb = ui ->loadTableBtn;
    ltb -> setAutoDefault(false);
    QObject::connect(ltb, &QPushButton::clicked, this, &MainWindow::loadTable);
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
