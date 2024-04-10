#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <QObject>
#include <ui_mainwindow.h>

class Automate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

    bool m_running;

public:
    explicit Automate(QObject *parent = nullptr);

    bool running() const;

signals:
    void runningChanged();
    void finished();

public slots:
    void setRunning(bool running);
    void screenshot();

private:
    int i = 0;
    Ui::MainWindow *ui;
};

#endif // AUTOMATE_H
