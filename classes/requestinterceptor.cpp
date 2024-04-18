#include "requestinterceptor.h"

RequestInterceptor::RequestInterceptor(QObject *parent)
    :QWebEngineUrlRequestInterceptor(parent)
{}

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    QString rsrct = "";

    switch(info.resourceType()){
    case 13:rsrct = "ResourceTypeXhr, // a XMLHttpRequest";
        break;
    default : rsrct = "Other or Unknown type";
        break;
    }


    //if(info.requestUrl() == QUrl("https://webnavlo.nta.group/WNavSystemB/Map/GetHistoryNavigation"))
      //  qDebug() << Q_FUNC_INFO << ":\n"
        //         << "WebUrlRequestInterceptor::interceptRequest    " << info.resourceType()
          //       << "\r\n  " << info.httpHeaders();

}
