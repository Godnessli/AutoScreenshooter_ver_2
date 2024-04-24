#include "table.h"
#include "xlnt/xlnt.hpp"
#include <QWidget>
#include <QFileDialog>

using namespace xlnt;

Table::Table(QWidget *parent) : QWidget(parent)
{
}

void Table::getpath()
{
    QWidget *w = new QWidget();
    filepath = QFileDialog::getOpenFileName(w,
                                            "Выберите файл",
                                            "C:/", "Файл Excel(*.xlsx)");
}

bool Table::check(const QString &path)
{
    if (path != QString(""))
        return true;
    else
        return false;
}

QVector<QVector<QString>> Table::readtable()
{
    qDebug() << "Table thread is " << QThread::currentThreadId();
    getpath();
    QVector<QVector<QString>> tab;
    QString check1 = "ОБРЫВ БЛОКА ГЛОНАСС",
            check2 = "ПРОБКИ",
            check3 = "РЕЙС ВЫПОЛНЕН ПРАВИЛЬНО";
    if (check(filepath))
    {
        workbook wb;
        wb.load(filepath.toStdString());
        auto ws = wb.active_sheet();
        for(auto row : ws.rows(false))
        {
            QVector<QString> ROW;
            for(auto cell : row)
            {
                int num = cell.column_index();
                if(num != 7 && num != 10)
                {
                    if(num != 4 && num != 5 && num != 1)
                    {
                        std::string c = cell.to_string();
                        ROW.push_back(QString::fromStdString(c));
                    }
                    else if(num == 1)
                    {
                        std::string date_str = cell.to_string();
                        int day_count;

                        if(cell.is_date())
                        {
                            try
                            {
                                day_count = stod(date_str);
                                QDate date;
                                date.setDate(1900, 1, 1);
                                date = date.addDays(day_count);
                                ROW.push_back(date.toString("dd.MM.yyyy"));
                            }
                            catch(std::invalid_argument)
                            {
                                ROW.push_back(QString::fromStdString(date_str));
                            }
                        }
                        else
                            ROW.push_back(QString::fromStdString(date_str));
                    }
                    else
                    {
                        const int msecperDay = 86400000;
                        double day_count;
                        std::string time_str = cell.to_string();
                        if(cell.is_date())
                        {
                            try
                            {
                                day_count = stod(time_str);
                                day_count *= msecperDay;
                                QTime time = QTime::fromMSecsSinceStartOfDay(day_count);
                                ROW.push_back(time.toString("hh:mm"));
                            }
                            catch(std::invalid_argument)
                            {
                                ROW.push_back(QString::fromStdString(time_str));
                            }
                        }
                        else
                            ROW.push_back(QString::fromStdString(time_str));
                    }
                }
            }

            if(ROW[6] == check1 || ROW[6] == check2 || ROW[6] == check3)
            {
                tab.append(ROW);
            }
        }
    }
    return tab;
}
