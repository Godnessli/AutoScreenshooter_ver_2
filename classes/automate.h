#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <QObject>
#include "delegate.h"
#include <ui_mainwindow.h>

class Automate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

public:
    explicit Automate(QObject *parent = nullptr);

    bool running() const;

    bool wasRun = false;

    int rCount,
        numRow;

    QString initialPath;

    QPixmap pixmap;

signals:
    void runningChanged();
    void screencreate();
    void navigated();
    void finished();
    void wait();

public slots:
    void setRunning(bool running);
    void screenshot();

private:
    bool m_running;

    Delegate *m_delegate = new Delegate;
    Ui::MainWindow *ui;
};

#endif // AUTOMATE_H
