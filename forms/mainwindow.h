#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/delegate.h"
#include "classes/automate.h"
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
    QPixmap pixmap;

    Delegate *m_delegate = new Delegate;

public slots:
    void screenshot();

private slots:
    void pixUpdate();
    void filePath();
    void buildTable();
    void web();
    void tableNavigate();
    void on_autoBtn_clicked();
    void start();
    void stop();
    void enterName();
    void openStory();
    void selectGarage();
    void buildTrack();

private:
    QLineEdit *locationEdit;

    QString date,
            route,
            garage,
            problem,
            time,
            name,
            initialPath,
            jQuery;

    const QString format = "jpg",
                  screens = "Скрины",
                  dot = ".",
                  space = " ",
                  slash = "/";

    bool isBuild = false;
    Ui::MainWindow *ui;
    QThread thread;
    Automate a;

signals:
    void tableLoaded();
    void dateSelected();
    void garageSelected();

};
#endif // MAINWINDOW_H
