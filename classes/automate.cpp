#include "automate.h"
#include <QtWidgets>

Automate::Automate(QObject *parent)
    : QObject{parent}
{}

bool Automate::running() const
{
    return m_running;
}

void Automate::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged();
}

void Automate::screenshot()
{
    while (i < ui -> tableWidget_Item -> rowCount())
    {
        qDebug() << "this thread " << QThread::currentThreadId();
        ++i;
    }
    emit finished();
}
