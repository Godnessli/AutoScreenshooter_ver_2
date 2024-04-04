#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/delegate.h"
#include <QMainWindow>
#include <QPushButton>
#include <QtWebEngineCore>
#include <QTableWidget>
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int numRow = 0;

    Delegate *m_delegate = new Delegate;

private slots:
    void buildTable();
    void screenshot();
    void web();
    void autoscr();

private:
    bool isBuild = false;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
