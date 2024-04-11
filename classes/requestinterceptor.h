#ifndef REQUESTINTERCEPTOR_H
#define REQUESTINTERCEPTOR_H

#include <QtWebEngineCore>

class RequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

public:
    RequestInterceptor(QObject *parent = Q_NULLPTR);

    void interceptRequest(QWebEngineUrlRequestInfo &info);
};

#endif // REQUESTINTERCEPTOR_H
