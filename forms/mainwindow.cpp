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
    connect(timer, &QTimer::timeout, this, &MainWindow::getBoolean);

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
        route = ui -> tableWidget_Item -> item(numRow, 1) -> text();

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

    emit a.pixUpdated();
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

    emit a.filePathCreated();
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
$('#signIn').click();

var completed = false;)DELIM"));

    ui -> widget -> page() -> scripts().insert(logIn);
}

void MainWindow::tableNavigate()
{
    if (a.wasRun)
        numRow = a.numRow;
    else
        a.numRow = numRow;

    QVector<int> rws = m_delegate -> rows();

    numRow = ui -> tableWidget_Item -> currentRow();
    rws.push_back(numRow);

    ++numRow;

    date = ui -> tableWidget_Item -> item(numRow, 0) -> text();
    previousDate = ui -> tableWidget_Item -> item(numRow - 1, 0) -> text();
    time = ui -> tableWidget_Item -> item(numRow, 3) -> text();
    garage = ui -> tableWidget_Item -> item(numRow, 5) -> text();
    route = ui -> tableWidget_Item -> item(numRow, 1) -> text();
    nextGarage = ui -> tableWidget_Item -> item(numRow + 1, 5) -> text();
    timeStep = ui -> tableWidget_Item -> item(numRow, 4) -> text();

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

    connect(&thread, &QThread::started, this, &MainWindow::buildTrack);

    connect(this, SIGNAL(trackBuilded()), this, SLOT(filePath()));
    connect(&a, SIGNAL(filePathCreated()), this, SLOT(pixUpdate()));
    connect(&a, SIGNAL(filePathCreated()), this, SLOT(tableNavigate()));
    connect(&a, SIGNAL(pixUpdated()), &a, SLOT(screenshot()));
    connect(&a, SIGNAL(screencreate()), this, SLOT(buildTrack()));

    connect(&a, SIGNAL(finished()), &thread, SLOT(terminate()));
    connect(&a, SIGNAL(finished()), this, SLOT(stop()));
    connect(&a, SIGNAL(wait()), &thread, SLOT(quit()));

    a.setRunning(true);
    thread.start();
}

void MainWindow::stop()
{
    a.setRunning(false);

    disconnect(&thread, &QThread::started, this, &MainWindow::buildTrack);

    disconnect(this, SIGNAL(trackBuilded()), this, SLOT(filePath()));
    disconnect(&a, SIGNAL(filePathCreated()), this, SLOT(pixUpdate()));
    disconnect(&a, SIGNAL(filePathCreated()), this, SLOT(tableNavigate()));
    disconnect(&a, SIGNAL(pixUpdated()), &a, SLOT(screenshot()));
    disconnect(&a, SIGNAL(screencreate()), this, SLOT(buildTrack()));

    disconnect(&a, SIGNAL(finished()), &thread, SLOT(terminate()));
    disconnect(&a, SIGNAL(finished()), this, SLOT(stop()));
    disconnect(&a, SIGNAL(wait()), &thread, SLOT(quit()));
}

void MainWindow::openStory()
{
    QFile wcjs(":/qtwebchannel/qwebchannel.js");
    if(!wcjs.open(QIODevice::ReadOnly))
        qDebug() << "Couldn't load Qt's QWebChannel API!";
    QString qWebChannelJs = QString::fromLatin1(wcjs.readAll());
    wcjs.close();

    ui -> widget -> page() -> runJavaScript(qWebChannelJs);

    QWebChannel channel;
    channel.registerObject("Js", this);
    ui -> widget -> page() -> setWebChannel(&channel);

    QString jscodeSetDate =
R"DELIM($('#tabs-page-headers')[0].children[2].children[0].click();
$('#history-date').val('datefix');
$('#history-tab-all').click();
$('#load-transport-history').click();
)DELIM";

    QString datefix = date.sliced(6) + "-" + date.sliced(3, 2) + "-" + date.sliced(0, 2);
    jscodeSetDate.replace("datefix", datefix);

    ui -> widget -> page() -> runJavaScript(jscodeSetDate);
}

void MainWindow::buildTrack()
{
    QString jscodeShowTrack =
R"(
function setDate() {
    return new Promise(function(resolve) {
        if("THISDATE" != "PREVDATE")
        {
            $('#tabs-page-headers')[0].children[2].children[0].click();
            $('#history-date').val('THISDATE');
            $('#history-tab-all').click();
            $('#load-transport-history').click();
        }
        resolve();
    })
}

function build() {
    return new Promise(function(resolve) {
        setTimeout(function() {
            if(!alreadyCreated);
            {
                $("#menu")[0].value = false;
                var simulate = new MouseEvent('click', {
                    shiftKey: true,
                    bubbles: true
                });

                var data = [];
                $("#menu")[0].value = false;

                var TS = {
                                    date: $("#history-date")[0].value,
                                    uniqueID: $('#history-select-all-ts option:contains(garagenum )')[0].value
                                }

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
                }

                function f1() {
                    return new Promise(function(resolve) {
                        console.log(1);
                        $('#tabs-page-headers')[0].children[2].children[0].click()
                        $("#history_select_all_ts_chosen").mousedown()
                        $('.chosen-results li:contains(garagenum )').mouseup()
                        $("#history-load-navigation").click()
                        resolve();
                    })
                }

                function f2() {
                    return new Promise(function(resolve) {
                            setTimeout(function() {
                            console.log(2);
                                var response = fetch('https://webnavlo.nta.group/WNavSystemB/Map/GetHistoryNavigation', {
                                    method: 'POST',
                                    headers: {
                                        'Content-Type': 'application/json; charset=utf-8'
                                    },
                                    body: JSON.stringify(TS),
                                }).then(async (response) => {
                                      data = await response.json();
                                    console.log(data)
                                })
                            resolve();
                            }, 250);
                    })
                }

                function f4() {
                    return new Promise(function(resolve) {
                        setTimeout(function() {
                            console.log(4);
                            $('#tabs-page-headers')[0].children[0].children[0].click()
                            $('#tabs-page1 div:contains(RouteNum)')[0].click()
                            $('#choose-transport-action-tracking-marsh')[0].click()
                            $('#map')[0].children[3].children[5].style.display = "none"
                            $('#map')[0].children[3].children[6].style.display = "block"
                            resolve();
                        }, 200);
                    })
                }

            function f5() {
                return new Promise(function(resolve){
                    setTimeout(function(){
                        console.log(5);
                        $('#tabs-page-headers')[0].children[2].children[0].click();
                        resolve();
                    }, 1000);
                })
            }

            function f6() {
                return new Promise(function(resolve){
                    setTimeout(function(){
                        console.log(6);
                        $('#tabs-page3')[0].scrollTo(0,0);
                        resolve();
                    }, 100);
                })
            }
            }

            function f3() {
                return new Promise(function(resolve) {
                        console.log(3);
                        var start = pointIndex("StartTime")
                        var end = pointIndex("EndTime")
                        $(`#history-navigation-table tbody [index = ${start}]`).click()
                        $(`#history-navigation-table tbody [index = ${end}]`)[0].dispatchEvent(simulate)
                        resolve();
                })
            }

            var alreadyCreated = true;

            if(TS.uniqueID != $('#history-select-all-ts option:contains(NextGarageNum )')[0].value)
            {
                alreadyCreated = false;
            }

            f1().then(function() {
                return f2();
            });
            var inter = setInterval(function() {
                        if(data.length != 0){
                        clearInterval(inter);
                      f3().then(function() {
                          return f4();
                      }).then(function() {
                          return f5();
                      }).then(function() {
                          return f6();
                      }).then(function() {
                          $("#menu")[0].value = true;
                      });
                }
            }, 250);
            resolve();
        }, 500);
    })
}

setDate().then(function() {
    return build();
});
)";

    QString thisDate = date.sliced(6) + "-" + date.sliced(3, 2) + "-" + date.sliced(0, 2);
    QString prevDate = previousDate.sliced(6) + "-" + previousDate.sliced(3, 2) + "-" + previousDate.sliced(0, 2);

    jscodeShowTrack.replace("THISDATE", thisDate);
    jscodeShowTrack.replace("PREVDATE", prevDate);
    jscodeShowTrack.replace("garagenum", garage);
    jscodeShowTrack.replace("StartTime", time);
    jscodeShowTrack.replace("RouteNum", route);
    jscodeShowTrack.replace("NextGarageNum", nextGarage);

    QTime startTime = QTime::fromString(time, "hh:mm");
    QTime endTime;
    if(timeStep != "")
    {
        QTime betweenPointsTime = QTime::fromString(timeStep, "hh:mm");
        startTime = startTime.addMSecs(betweenPointsTime.msecsSinceStartOfDay());
    }
    else
        endTime = startTime.addSecs(3600);

    jscodeShowTrack.replace("EndTime", QString(endTime.toString("hh:mm")));

    ui -> widget -> page() -> runJavaScript(jscodeShowTrack);

    functionComplete = false;

    timer -> start(1000);
}

void MainWindow::getBoolean()
{
    ui -> widget -> page() -> runJavaScript("$('#menu')[0].value",
                                      [this](const QVariant &b)
                                      {functionComplete = b.toBool();});

    if(functionComplete)
    {
        qDebug() << functionComplete;

        timer -> stop();
        emit trackBuilded();
    }
}
