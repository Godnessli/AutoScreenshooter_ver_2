#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/delegate.h"
#include "classes/requestinterceptor.h"
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
    RequestInterceptor *ri = new RequestInterceptor();

public slots:
    void screenshot();

private slots:
    void pixUpdate();
    void filePath();
    void buildTable();
    void web();
    void on_autoBtn_clicked();
    void start();
    void stop();
    void openStory();
    void tableNavigate();
    void buildTrack();
    void getBoolean();

private:
    QLineEdit *locationEdit;

    QJsonDocument jsonList;

    QString loginScript;

    QString date,
            route,
            garage,
            nextGarage,
            problem,
            time,
            name,
            initialPath;

    const QString format = "jpg",
                  screens = "Скрины",
                  dot = ".",
                  space = " ",
                  slash = "/";

    bool isBuild = false;
    bool functionComplete;
    QTimer *timer = new QTimer;
    Ui::MainWindow *ui;
    QThread thread;
    Automate a;

signals:
    void tableLoaded();
    void trackBuilded();

};
#endif // MAINWINDOW_H
