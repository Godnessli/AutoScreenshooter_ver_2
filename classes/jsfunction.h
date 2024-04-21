#ifndef JSFUNCTION_H
#define JSFUNCTION_H

#include "ui_mainwindow.h"
#include "delegate.h"
#include "automate.h"

class jsFunction : public QObject
{
    Q_OBJECT

    Ui::MainWindow *ui;
    Automate a;
    Delegate *m_delegate = new Delegate;

    int numRow = 0;
    QString date,
        route,
        garage,
        nextGarage,
        problem,
        time,
        name;

public:
    void tableNavigate()
    {
        if (a.wasRun)
            numRow = a.numRow;
        else
            a.numRow = numRow;

        QVector<int> rws = m_delegate -> rows();

        numRow = ui -> tableWidget_Item -> currentRow();
        rws.push_back(numRow);

        ++numRow;

        date = ui -> tableWidget_Item -> item(numRow, 0) -> text();
        time = ui -> tableWidget_Item -> item(numRow, 3) -> text();
        garage = ui -> tableWidget_Item -> item(numRow, 5) -> text();
        route = ui -> tableWidget_Item -> item(numRow, 1) -> text();
        nextGarage = ui -> tableWidget_Item -> item(numRow + 1, 5) -> text();

        m_delegate -> setRows(rws);
        ui -> tableWidget_Item -> update();

        ui -> tableWidget_Item -> selectRow(numRow);
        if(numRow!= ui -> tableWidget_Item -> rowCount())
            ui -> tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(numRow, 0));
        else {
            ui ->tableWidget_Item -> scrollToItem(
                ui -> tableWidget_Item -> item(
                    ui ->tableWidget_Item->rowCount(), 0));
            if (numRow == ui -> tableWidget_Item -> rowCount())
            {
                QString check = "";
                numRow = 0;
                for(int i = 0; i < ui -> tableWidget_Item -> rowCount(); ++i)
                {
                    QString data = ui -> tableWidget_Item -> item(i, 7) -> text();
                    if(data == check)
                    {
                        numRow = i;
                        ui -> tableWidget_Item -> selectRow(numRow);
                        ui -> tableWidget_Item -> scrollToItem(
                            ui -> tableWidget_Item -> item(numRow, 0));
                        ui -> tableWidget_Item -> item(numRow, 0);
                        break;
                    }
                }
            }
        }

        qDebug() << "Hello from JS!!!";
        emit a.navigated();
    }
};
#endif // JSFUNCTION_H
