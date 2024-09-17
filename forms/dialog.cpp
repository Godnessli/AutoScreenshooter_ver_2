#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui -> acceptBtn, &QPushButton::clicked, this, &Dialog::accept);
    connect(ui -> cancelBtn, &QPushButton::clicked, this, &Dialog::cancel);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::accept()
{
    if(ui -> timeEnd -> time() > ui -> timeStart -> time())
    {
        settingsFile -> beginGroup("TimeBuffer");
        settingsFile -> setValue("TimeStart", QTime(ui -> timeStart -> time()).toString("hh:mm"));
        settingsFile -> setValue("TimeEnd", QTime(ui -> timeEnd -> time()).toString("hh:mm"));
        settingsFile -> endGroup();

        Dialog::close();
        emit accepted();
    }
    else
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Неккоректное время",
                                                                   tr("Время окончания должно быть больше\nвремени начала"),
                                                                   QMessageBox::Ok | QMessageBox::Ok);
}

void Dialog::cancel()
{
    Dialog::close();
}
