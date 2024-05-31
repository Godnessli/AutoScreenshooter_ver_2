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
    connect(timer2, &QTimer::timeout, this, &MainWindow::responseJson);
    connect(timer1, &QTimer::timeout, this, &MainWindow::getBoolean);
    connect(timer4, &QTimer::timeout, this, &MainWindow::dateSetFunc);
    connect(timer5, &QTimer::timeout, this, &MainWindow::garageSet);
    connect(timer6, &QTimer::timeout, this, &MainWindow::chooseTimeInterval);

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
        thisGarage = ui -> tableWidget_Item -> item(numRow, 5) -> text();
        thisRoute = ui -> tableWidget_Item -> item(numRow, 1) -> text();

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
    thisRoute = twi2 ->text();

    QTableWidgetItem *twi3 = ui -> tableWidget_Item -> item(numRow, 5);
    thisGarage = twi3 ->text();

    QTableWidgetItem *twi4 = ui -> tableWidget_Item -> item(numRow, 6);
    problem = twi4 ->text();

    QTableWidgetItem *twi5 = ui -> tableWidget_Item -> item(numRow, 3);
    time = twi5 ->text();

    ui -> tableWidget_Item -> item(numRow, 7) -> setText("Есть");

    name = time.replace(QString(":"), QString("_")) +
           space + thisGarage + space + problem + dot + format;

    initialPath = QCoreApplication::applicationDirPath();

    if (QDir(initialPath).mkdir(screens))
    {
        initialPath += slash + screens;
        if(QDir(initialPath).mkdir(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
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

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
            else
                initialPath = QCoreApplication::applicationDirPath();
        }
        else if(QDir(initialPath).cd(date))
        {
            initialPath += slash + date;

            if(QDir(initialPath).mkdir(thisRoute))
                initialPath += slash + thisRoute;
            else if(QDir(initialPath).cd(thisRoute))
                initialPath += slash + thisRoute;
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
    thisGarage = ui -> tableWidget_Item -> item(numRow, 5) -> text();
    thisRoute = ui -> tableWidget_Item -> item(numRow, 1) -> text();
    prevRoute = ui -> tableWidget_Item -> item(numRow - 1, 1) -> text();
    prevGarage = ui -> tableWidget_Item -> item(numRow - 1, 5) -> text();
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
    connect(&a, SIGNAL(pixUpdated()), &a, SLOT(screenshot()));
    connect(&a, SIGNAL(pixUpdated()), this, SLOT(tableNavigate()));
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

    ui -> autoBtn -> setText("Автомат");

    ui -> buildTrack -> setEnabled(true);
    ui -> openTable -> setEnabled(true);
    ui -> screenBtn -> setEnabled(true);
    ui -> loadTableBtn -> setEnabled(true);

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

void MainWindow::buildTrack()
{
    connect(this, &MainWindow::dateSet, this, &MainWindow::setGarage);
    connect(this, &MainWindow::garSet, this, &MainWindow::setTimeInterval);
    connect(this, &MainWindow::intervalSet, this, &MainWindow::finishBuildingTrack);

    thisDate = date.sliced(6) + "-" + date.sliced(3, 2) + "-" + date.sliced(0, 2);

    if(numRow != 0)
    {
        prevDate = previousDate.sliced(6) + "-" + previousDate.sliced(3, 2) + "-" + previousDate.sliced(0, 2);

        functionComplete = false;

        if(thisDate != prevDate)
        {
            setDate();
        }
        else if(thisGarage != prevGarage)
        {
            setGarage();
        }
        else
        {
            chooseTimeInterval();
        }
    }
    else
    {
        setDate();
    }
}

void MainWindow::getBoolean()
{
    ui -> widget -> page() -> runJavaScript("$('#menu')[0].value",
                                      [this](const QVariant &b)
                                      {functionComplete = b.toBool();});

    if(functionComplete)
    {
        qDebug() << functionComplete;

        timer1 -> stop();
        ui -> widget -> page() -> runJavaScript("$('#menu')[0].value = false;");
        emit trackBuilded();
    }
}

void MainWindow::requestJson()
{
    QString requestJsonJs = R"(
    data = [];
      var TS = {
        date: $("#history-date")[0].value,
        uniqueID: $('#history-select-all-ts option:contains(GARAGE )')[0].value
      }
      var response = fetch('https://webnavlo.nta.group/WNavSystemB/Map/GetHistoryNavigation', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json; charset=utf-8'
        },
        body: JSON.stringify(TS),
      }).then(async (response) => {
        data = await response.json();
      }))";

    requestJsonJs.replace("GARAGE", thisGarage);

    ui -> widget -> page() -> runJavaScript(requestJsonJs);

    timer2 -> start(200);
}

void MainWindow::responseJson()
{
    ui -> widget -> page() -> runJavaScript(R"(data;)",
                                      [this](const QVariant &b)
                                      {
                                          json = b.toJsonArray();
                                      });

    if(!json.isEmpty())
    {
        timer2 -> stop();
        timer5 -> start(250);
    }
}

void MainWindow::setDate()
{
    QString setDateJs = R"(
    simulate = new MouseEvent('click', {
      shiftKey: true,
      bubbles: true
    });

    $("#menu")[0].value = false;

    $('#tabs-page-headers')[0].children[2].children[0].click();
    $('#history-date').val('DATE');
    $('#history-tab-all').click();
    $('#load-transport-history').click();
)";

    setDateJs.replace("DATE", thisDate);

    ui -> widget -> page() -> runJavaScript(setDateJs);

    timer4 -> start(200);
}

void MainWindow::dateSetFunc()
{
    ui -> widget -> page()
        -> runJavaScript("$('#load-transport-history')[0].classList.length < 2",
                        [this](const QVariant &b)
                        {
                            qDebug() << b.toBool();
                            dateSelected = b.toBool();
                        });

    if(dateSelected)
    {
        timer4 -> stop();
        emit dateSet();
    }
}

void MainWindow::setGarage()
{
    QString setGarageJs = R"(
        $('#tabs-page-headers')[0].children[2].children[0].click();
        $("#history_select_all_ts_chosen").mousedown();
        $('.chosen-results li:contains(GARAGE )').mouseup();
        $("#history-load-navigation").click();)";

    setGarageJs.replace("GARAGE", thisGarage);

    ui -> widget -> page() -> runJavaScript(setGarageJs);

    requestJson();
}

void MainWindow::garageSet()
{
    ui -> widget -> page()
        -> runJavaScript("$('#history-load-navigation')[0].classList.length < 4",
                        [this](const QVariant &b)
                        {
                            garageSelected = b.toBool();
                        });

    if(garageSelected)
    {
        timer5 -> stop();
        emit garSet();
    }
}

void MainWindow::setTimeInterval()
{
    for(int i = 0; i < json.size(); ++i)
    {
        QJsonObject timeN = json[i].toObject();
        QString timeNav = timeN["timeNav"].toString();
        timeNav = timeNav.sliced(11);
        timeNav = timeNav.sliced(0, 5);
        QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
        QTime raceTime = QTime::fromString(time, "hh:mm");
        bool indexFined = false;

        for(int j = 0; j < 10; ++j)
        {
            int msecPerMin = 60000;
            int strt = raceTime.msecsSinceStartOfDay();
            strt -= j * msecPerMin;
            QTime startTime = QTime::fromMSecsSinceStartOfDay(strt);

            if(pointTimeNav == startTime)
            {
                pointIndex = i - j;
                indexFined = true;
            }
        }
        if(indexFined)
            break;
    }

    for(int i = 0; i < json.size(); ++i)
    {
        QJsonObject timeN = json[i].toObject();
        QString timeNav = timeN["timeNav"].toString();
        timeNav = timeNav.sliced(11);
        timeNav = timeNav.sliced(0, 5);
        QTime pointTimeNav = QTime::fromString(timeNav, "hh:mm");
        QTime raceTime = QTime::fromString(time, "hh:mm");
        raceTime = raceTime.addSecs(3600);
        bool indexFined = false;

        for(int j = 0; j < 10; ++j)
        {
            int msecPerMin = 60000;
            int strt = raceTime.msecsSinceStartOfDay();
            strt += j * msecPerMin;
            QTime finishTime = QTime::fromMSecsSinceStartOfDay(strt);

            if(pointTimeNav == finishTime)
            {
                pointFinishIndex = i + j;
                indexFined = true;
            }
        }
        if(indexFined)
        {
            timer6 -> start(300);
            break;
        }
    }
}

void MainWindow::chooseTimeInterval()
{
    timer6 -> stop();
    QString chooseTIJs = R"(
    $(`#history-navigation-table tbody [index = STARTINDEX]`).click()
    $(`#history-navigation-table tbody [index = FINISHINDEX]`)[0].dispatchEvent(simulate))";

    chooseTIJs.replace("STARTINDEX", QString::fromStdString(std::to_string(pointIndex)));
    chooseTIJs.replace("FINISHINDEX", QString::fromStdString(std::to_string(pointFinishIndex)));

    ui -> widget -> page() -> runJavaScript(chooseTIJs);

    emit intervalSet();
}

void MainWindow::finishBuildingTrack()
{
    QString finishBTJs = R"(
    function f7() {
      return new Promise(function(resolve){
        if("THISROUTE" != "PREVROUTE")
          {
            var inter = setInterval(function(){
              if($('.history-selected-row').length > 10)
                clearInterval(inter);
                console.log(7);
                $('#free-view-wrap')[0].click();
                resolve();
            }, 350);
          }
        else resolve();
      })
    }

    function f4() {
      return new Promise(function(resolve) {
        setTimeout(function() {
          console.log(4);
          $('#tabs-page-headers')[0].children[0].children[0].click()
          $('#tabs-page1 div:contains(THISROUTE)')[0].click()
          $('#choose-transport-action-tracking-marsh')[0].click()
          $('#map')[0].children[3].children[5].style.display = "none"
          $('#map')[0].children[3].children[6].style.display = "block"
        }, 500);
        resolve();
      })
    }

    function f5() {
      return new Promise(function(resolve){
        setTimeout(function(){
          console.log(5);
          $('#tabs-page-headers')[0].children[2].children[0].click();
        }, 1000);
        resolve();
      })
    }

    function f6() {
      return new Promise(function(resolve){
        setTimeout(function(){
          console.log(6);
          $('#tabs-page3')[0].scrollTo(0,0);
        }, 1000);
        resolve();
      })
    }

    f7().then(function() {
      return f4();
    }).then(function() {
      return f5();
    }).then(function() {
      return f6();
    }).then(function() {
      $("#menu")[0].value = true;
    });)";

    finishBTJs.replace("THISROUTE", thisRoute);
    finishBTJs.replace("PREVROUTE", prevRoute);

    ui -> widget -> page() -> runJavaScript(finishBTJs);

    disconnect(this, &MainWindow::dateSet, this, &MainWindow::setGarage);
    disconnect(this, &MainWindow::garSet, this, &MainWindow::setTimeInterval);
    disconnect(this, &MainWindow::intervalSet, this, &MainWindow::finishBuildingTrack);

    timer1 -> start(250);
}
