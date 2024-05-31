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
    int numRow = 0,
        pointIndex,
        pointFinishIndex;
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
    void tableNavigate();
    void buildTrack();
    void getBoolean();
    void requestJson();
    void responseJson();
    void setDate();
    void setGarage();
    void setTimeInterval();
    void chooseTimeInterval();
    void finishBuildingTrack();
    void dateSetFunc();
    void garageSet();

private:
    QLineEdit *locationEdit;

    QJsonDocument jsonList;

    QString loginScript;

    QString date,
            previousDate,
            thisRoute,
            prevRoute,
            thisGarage,
            prevGarage,
            problem,
            time,
            timeStep,
            name,
            initialPath,
            thisDate,
            prevDate;

    const QString format = "jpg",
                  screens = "Скрины",
                  dot = ".",
                  space = " ",
                  slash = "/";

    bool isBuild = false;
    bool functionComplete;
    bool dateSelected = false;
    bool garageSelected = false;

    QJsonArray json;
    QTimer *timer1 = new QTimer;
    QTimer *timer2 = new QTimer;
    QTimer *timer3 = new QTimer;
    QTimer *timer4 = new QTimer;
    QTimer *timer5 = new QTimer;
    QTimer *timer6 = new QTimer;
    Ui::MainWindow *ui;
    QThread thread;
    Automate a;

signals:
    void tableLoaded();
    void trackBuilded();
    void dateSet();
    void garSet();
    void intervalSet();

};
#endif // MAINWINDOW_H
