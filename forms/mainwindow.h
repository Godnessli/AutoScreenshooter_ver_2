#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "classes/delegate.h"
#include "classes/automate.h"
#include "xlnt/xlnt.hpp"
#include "classes/table.h"
#include <QMainWindow>
#include <QPushButton>
#include <QtWebEngineCore>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <QCloseEvent>
#include <iomanip>
#include <math.h>

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
        startPoint,
        pointFinishIndex,
        pPointFinishIndex,
        countBeforeSave = 0;
    QPixmap pixmap;

    Delegate *m_delegate = new Delegate;

public slots:
    void screenshot();
    void video();

    void keyPressEvent(QKeyEvent *event);
private slots:
    void pixUpdate();
    void filePath();
    void checkTable();
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
    void requestJsonRoute();
    void responseJsonRoute();
    void setTimeInterval();
    void chooseTimeInterval();
    void dateSetFunc();
    void garageSet();
    void selectedItems();
    void closeEvent(QCloseEvent *event);
    void setLoginData();
    void login();
    void checkLogin();
    void entered();
    void closeDialog();
    void showImage();
    void nextImage();
    void prevImage();
    void deleteScreen();
    void setVid();
    void controlSetting();
    void changeScreenKey();
    void signalOfTrackBuilded();
    void checkTrack();
private:
    QMap<QString, unsigned> mapOfCars;
    QVector<QVector<QString>> tab;
    QLineEdit *locationEdit;
    QStringList logpass;

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
            prevDate,
            loginText,
            insert;

    const QString format = "jpg",
                  screens = "Скрины",
                  dot = ".",
                  space = " ",
                  slash = "/";

    bool isBuild = false;
    bool functionComplete;
    bool dateSelected = false;
    bool garageSelected = false;
    bool changed = false;
    bool changeScreen = false;
    int count = 0;
    int makedCount = 0;

    QJsonArray json,
               stopStationList,
               routeVariantList;
    QJsonArray emptyJson;
    QTimer *timer1 = new QTimer;
    QTimer *timer2 = new QTimer;
    QTimer *timer3 = new QTimer;
    QTimer *timer4 = new QTimer;
    QTimer *timer5 = new QTimer;
    QTimer *timer6 = new QTimer;
    QTimer *timer7 = new QTimer;
    QTimer *timer8 = new QTimer;
    QTime prevTime, startTime;
    Table *table = new Table;

    QSettings *settingsFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                "/" + "cache" + "/" + "settings.ini"), QSettings::IniFormat);
    QLineEdit *logIn = new QLineEdit;
    QLineEdit *password = new QLineEdit;
    QDialog *enter = new QDialog;
    QDialog *viewer = new QDialog;
    QDialog *settings = new QDialog;
    QLabel *screenLbl = new QLabel("Скриншот: ");
    QLabel *videoLbl = new QLabel("Видео: ");
    QLabel *buildTrackLbl = new QLabel("Постройка трека: ");
    QPushButton *screenButton = new QPushButton();
    QLabel *lbl = new QLabel("Введите здесь ваши данные. В дальнейшем ввод будет автоматически");
    QLabel *keysLbl = new QLabel("Ключ:");
    QLabel *label = new QLabel;
    QPushButton *in = new QPushButton("Войти");
    QPushButton *out = new QPushButton("Выйти");
    QGridLayout *layout = new QGridLayout(enter);
    QGridLayout *settingLayout = new QGridLayout(settings);
    QComboBox *keys = new QComboBox(enter);
    QPushButton *accept = new QPushButton("Принять");
    QPushButton *cancel = new QPushButton("Отмена");
    QPushButton *del = new QPushButton("Удалить скрин");
    QPushButton *prev = new QPushButton("Предыдущий");
    QPushButton *next = new QPushButton("Следующий");
    QPushButton *setVideo = new QPushButton("Видео");
    QGridLayout *lout = new QGridLayout(viewer);
    QLabel *photo = new QLabel();

    QLabel *lbldate = new QLabel();
    QLabel *lblroute = new QLabel();
    QLabel *lbldirect = new QLabel();
    QLabel *lbltime = new QLabel();
    QLabel *lblgarage = new QLabel();
    QLabel *lblproblem = new QLabel();

    Ui::MainWindow *ui;
    QThread thread;
    Automate a;

    void paintRows();
    int point_index(QJsonArray arr, QString timeOfRace, double sclot, double sclon, bool (*ch)(double, double, double, double, double, double));
signals:
    void tableLoaded();
    void responseAccepted();
    void garrageSet();
    void trackBuilded();

};
#endif // MAINWINDOW_H
