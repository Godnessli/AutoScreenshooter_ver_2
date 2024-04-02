#include "web.h"
#include "ui_web.h"
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineCertificateError>

Web::Web(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Web)
{
    QWebEngineView *view = new QWebEngineView(this);
    QWebEnginePage *page = new QWebEnginePage();

    page -> load(QUrl("https://webnavlo.nta.group/WNavSystemB"));

    QObject::connect(page, &QWebEnginePage::certificateError,
                     [](QWebEngineCertificateError e) { e.acceptCertificate(); });

    view -> setPage(page);
    view -> resize(1109, 511);
    view -> show();

    ui -> setupUi(this);
}

Web::~Web()
{
    delete ui;
}
