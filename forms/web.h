#ifndef WEB_H
#define WEB_H

#include <QWidget>
#include <QWebEngineCertificateError>

namespace Ui {
class Web;
}

class Web : public QWidget
{
    Q_OBJECT

public:
    explicit Web(QWidget *parent = nullptr);
    ~Web();

private:
    Ui::Web *ui;

};

#endif // WEB_H
