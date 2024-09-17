#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSettings>
#include <QCoreApplication>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    void accept();
    void cancel();

    QSettings *settingsFile = new QSettings(QString(QCoreApplication::applicationDirPath() +
                                                    "/" + "cache" + "/" + "settings.ini"), QSettings::IniFormat);
    Ui::Dialog *ui;

signals:
    void accepted();
};

#endif // DIALOG_H
