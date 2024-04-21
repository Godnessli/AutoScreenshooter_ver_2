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
    if (numRow == rCount - 1)
    {
        emit finished();
    }
    else
    {
        qDebug() << "Row: " << numRow << "     Thread: " << QThread::currentThreadId();

        pixmap.save(initialPath);
        ++numRow;
        emit screencreate();
    }
}
