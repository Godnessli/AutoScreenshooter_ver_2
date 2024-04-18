#include "mainwindow.h"
#include "classes/table.h"
#include "classes/requestinterceptor.h"
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

    qDebug() << "MainWindow get called from?: " << QThread::currentThreadId();

    QPushButton *ltb = ui ->loadTableBtn;
    QPushButton *screen = ui -> screenBtn;
    QPushButton *buildTrack = ui -> buildTrack;

    ui -> autoBtn -> setEnabled(false);
    ui -> buildTrack -> setEnabled(false);
    ui -> openTable -> setEnabled(false);

    ltb -> setAutoDefault(false);
    screen -> setAutoDefault(false);

    connect(ltb, &QPushButton::clicked, this, &MainWindow::buildTable);
    connect(screen, &QPushButton::clicked, this, &MainWindow::screenshot);
    connect(buildTrack, &QPushButton::clicked, this, &MainWindow::buildTrack);
    connect(this, &MainWindow::tableLoaded, this, &MainWindow::openStory);
    connect(this, &MainWindow::garageSelected, this, &MainWindow::buildRoute);

    a.setRunning(false);

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

        numRow = 0;
        ui -> tableWidget_Item -> setItemDelegate(m_delegate);
        ui -> tableWidget_Item -> selectRow(numRow);
        isBuild = true;

        ui -> autoBtn -> setEnabled(true);
        ui -> buildTrack -> setEnabled(true);
        ui -> openTable -> setEnabled(true);

        a.rCount = ui -> tableWidget_Item -> rowCount();
        date = ui -> tableWidget_Item -> item(numRow, 0) -> text();
        time = ui -> tableWidget_Item -> item(numRow, 3) -> text();
        garage = ui -> tableWidget_Item -> item(numRow, 5) -> text();

        emit tableLoaded();
    }
    else
        return;
}

void MainWindow::screenshot()
{
    if (a.wasRun)
    {
        numRow = a.numRow;
        a.wasRun = false;
    }
    else
        a.numRow = numRow;

    pixUpdate();

    if(isBuild)
    {
        filePath();
    }

    if(isBuild)
    {
        pixmap.save(initialPath);
        tableNavigate();
    }
    else
        pixmap.save("Без имени.jpg");
}

void MainWindow::pixUpdate()
{
    QPixmap pix(ui -> widget -> size());
    ui -> widget -> render(&pix);

    pixmap = pix;
    a.pixmap = pix;
}

void MainWindow::filePath()
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

    ui -> tableWidget_Item -> item(numRow, 7) -> setText("Есть");

    name = time.replace(QString(":"), QString("_")) +
           space + garage + space + problem + dot + format;

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
    a.initialPath = initialPath;
}

void MainWindow::web()
{
    QWebEngineProfile::defaultProfile() -> setUrlRequestInterceptor(ri);
    QWebEnginePage *page = new QWebEnginePage();

    page -> load(QUrl("https://webnavlo.nta.group/WNavSystemB"));

    QObject::connect(page, &QWebEnginePage::certificateError,
                     [](QWebEngineCertificateError e) { e.acceptCertificate(); });

    ui -> widget -> setPage(page);
    ui -> widget -> show();

    QWebEngineScript logIn;
    logIn.setInjectionPoint(QWebEngineScript::DocumentReady);
    logIn.setWorldId(QWebEngineScript::MainWorld);
    logIn.setName("user_login.js");
    logIn.setRunsOnSubFrames(true);
    logIn.setSourceCode(QStringLiteral(
        R"DELIM($("#Login").val("KuznecovAV");
$('#Password').val('1234');
$('#Key').val('piteravto5');
$('#signIn').click();)DELIM"));

    ui -> widget -> page() -> scripts().insert(logIn);

    connect(ui -> widget, &QWebEngineView::loadFinished, this, &MainWindow::enterName);
}

void MainWindow::tableNavigate()
{
    if (a.wasRun)
        numRow = a.numRow;
    else
        a.numRow = numRow;

    qDebug() << "Row: " << numRow << "     Thread: " << QThread::currentThreadId();
    QVector<int> rws = m_delegate -> rows();

    numRow = ui -> tableWidget_Item -> currentRow();
    rws.push_back(numRow);
    ++numRow;

    m_delegate -> setRows(rws);
    ui -> tableWidget_Item -> update();

    ui -> tableWidget_Item -> selectRow(numRow);
    if(numRow!= ui -> tableWidget_Item -> rowCount())
        ui -> tableWidget_Item -> scrollToItem(
            ui -> tableWidget_Item -> item(numRow, 0));
    else {
        ui ->tableWidget_Item -> scrollToItem(
            ui -> tableWidget_Item -> item(
                ui ->tableWidget_Item->rowCount(), 0));
        if (numRow == ui -> tableWidget_Item -> rowCount())
        {
            QString check = "";
            numRow = 0;
            for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); ++i)
            {
                QString data = ui -> tableWidget_Item -> item(i, 7) -> text();
                if(data == check)
                {
                    numRow = i;
                    ui -> tableWidget_Item -> selectRow(numRow);
                    ui -> tableWidget_Item -> scrollToItem(
                        ui -> tableWidget_Item -> item(numRow, 0));
                    ui -> tableWidget_Item -> item(numRow, 0);
                    break;
                }
            }
        }
    }
    emit a.navigated();
}

void MainWindow::on_autoBtn_clicked()
{
    switch (a.running())
    {
    case false:
        start();
        break;

    default:
        emit a.wait();
        stop();
        break;
    }
}

void MainWindow::start()
{
    a.moveToThread(&thread);
    ui -> autoBtn -> setText("Стоп");

    ui -> buildTrack -> setEnabled(false);
    ui -> openTable -> setEnabled(false);
    ui -> screenBtn -> setEnabled(false);
    ui -> loadTableBtn -> setEnabled(false);

    connect(&thread, &QThread::started, &a, &Automate::screenshot);

    connect(&a, SIGNAL(navigated()), &a, SLOT(screenshot()));
    connect(&a, SIGNAL(navigated()), this, SLOT(pixUpdate()));
    connect(&a, SIGNAL(screencreate()), this, SLOT(tableNavigate()));
    connect(&a, SIGNAL(navigated()), this, SLOT(filePath()));
    connect(&a, SIGNAL(finished()), &thread, SLOT(terminate()));
    connect(&a, SIGNAL(finished()), this, SLOT(stop()));
    connect(&a, SIGNAL(wait()), &thread, SLOT(quit()));

    a.setRunning(true);
    thread.start();
}

void MainWindow::stop()
{
    a.setRunning(false);

    ui -> buildTrack -> setEnabled(true);
    ui -> openTable -> setEnabled(true);
    ui -> screenBtn -> setEnabled(true);
    ui -> loadTableBtn -> setEnabled(true);
    ui -> autoBtn -> setText("Автомат");

    disconnect(&thread, &QThread::started, &a, &Automate::screenshot);
    disconnect(&a, SIGNAL(navigated()), &a, SLOT(screenshot()));
    disconnect(&a, SIGNAL(screencreate()), this, SLOT(tableNavigate()));

    disconnect(&a, SIGNAL(finished()), &thread, SLOT(terminate()));
    disconnect(&a, SIGNAL(finished()), this, SLOT(stop()));
    disconnect(&a, SIGNAL(wait()), &thread, SLOT(quit()));
}

void MainWindow::enterName()
{
    /*QWebEngineScript logIn;
    logIn.setInjectionPoint(QWebEngineScript::DocumentReady);
    logIn.setWorldId(QWebEngineScript::MainWorld);
    logIn.setName("user_login.js");
    logIn.setRunsOnSubFrames(true);
    logIn.setSourceCode(QStringLiteral(
R"DELIM($("#Login").val("KuznecovAV");
$('#Password').val('1234');
$('#Key').val('piteravto5');
$('#signIn').click();)DELIM"));

    ui -> widget -> page() -> scripts().insert(logIn);*/

    disconnect(ui -> widget, &QWebEngineView::loadFinished, this, &MainWindow::enterName);
}

void MainWindow::openStory()
{
    QString jscodeSetDate =
R"DELIM($('#tabs-page-headers')[0].children[2].children[0].click();
$('#history-date').val('datefix');
$('#history-tab-all').click();
$('#load-transport-history').click();)DELIM";

    QString datefix = date.sliced(6) + "-" + date.sliced(3, 2) + "-" + date.sliced(0, 2);
    jscodeSetDate.replace("datefix", datefix);

    ui -> widget -> page() -> runJavaScript(jscodeSetDate);
}

void MainWindow::buildTrack()
{
    QString jscodeShowTrack =
R"($("#history_select_all_ts_chosen").mousedown();
$('.chosen-results li:contains(10182)').mouseup();
$("#history-load-navigation").click();

var data = [];

var TS = {
    date: $("#history-date")[0].value,
    uniqueID: $('#history-select-all-ts option:contains(10182)')[0].value
}

var response = fetch('https://webnavlo.nta.group/WNavSystemB/Map/GetHistoryNavigation', {
    method: 'POST',
    headers: {
        'Content-Type': 'application/json; charset=utf-8'
    },
    body: JSON.stringify(TS),
}).then(async (response) => {
    data = await response.json();
    console.log(data);
})

var simulate = new MouseEvent('click', {
    shiftKey: true,
    bubbles: true
});

function pointIndex (timeOfPoint) {
    for (var i = 0; i < data.length; i++)
    {
        var point = data[i].timeNav.split('');
        var time = point.slice(11, 16);
        var strtime = time.toString();
        if(strtime.replaceAll(',', '') === timeOfPoint) {
            return i;
            break;
        }
    }
})";

    //jscodeBuildTrack.replace("garagenum", garage);
    //jscodeBuildTrack.replace("StartTime", time);

    //QTime startTime = QTime::fromString(time, "hh:mm");
    //int hour = startTime.hour();
    //int minute = startTime.minute();
    //hour += 1;

    //QTime endTime(hour, minute);
    //QString endTimeStr = endTime.toString();
    //jscodeBuildTrack.replace("EndTime", endTimeStr.sliced(0, 5));

    /*QWebEngineScript showTrack;
    showTrack.setInjectionPoint(QWebEngineScript::Deferred);
    showTrack.setWorldId(QWebEngineScript::MainWorld);
    showTrack.setName("show_track.js");
    showTrack.setRunsOnSubFrames(true);
    showTrack.setSourceCode(jscodeShowTrack);

    ui -> widget -> page() -> scripts().insert(showTrack);*/
    ui -> widget -> page() -> runJavaScript(jscodeShowTrack);

    Sleep(2000);
    emit garageSelected();
}

void MainWindow::buildRoute()
{
    ui -> widget -> page() -> runJavaScript("var start = pointIndex('17:00');"
                                          "var end = pointIndex('18:00');"

                                          "$(`#history-navigation-table tbody [index = ${start}]`).click();"
                                          "$(`#history-navigation-table tbody [index = ${end}]`)[0].dispatchEvent(simulate);");

    QString jscodeShowRoute =
R"DELIM($('#tabs-page-headers')[0].children[0].children[0].click();
$('#tabs-page1 div:contains(370)')[0].click();
$('#choose-transport-action-tracking-marsh')[0].click();
$('#map')[0].children[3].children[5].style.display = "none";
$('#map')[0].children[3].children[6].style.display = "block";)DELIM";

    QWebEngineScript showRoute;
    showRoute.setInjectionPoint(QWebEngineScript::DocumentReady);
    showRoute.setWorldId(QWebEngineScript::MainWorld);
    showRoute.setName("show_route.js");
    showRoute.setRunsOnSubFrames(true);
    showRoute.setSourceCode(jscodeShowRoute);

    ui -> widget -> page() -> scripts().insert(showRoute);

    QWebEngineScript showCarNumber;
    showCarNumber.setInjectionPoint(QWebEngineScript::DocumentReady);
    showCarNumber.setWorldId(QWebEngineScript::MainWorld);
    showCarNumber.setName("show_car_number.js");
    showCarNumber.setRunsOnSubFrames(true);
    showCarNumber.setSourceCode(
R"DELIM($('#tabs-page-headers')[0].children[2].children[0].click()
$('#tabs-page3')[0].scrollTo(0,0);)DELIM");

    ui -> widget -> page() -> scripts().insert(showCarNumber);

    disconnect(ui -> widget, &QWebEngineView::loadFinished, this, &MainWindow::buildRoute);
}
