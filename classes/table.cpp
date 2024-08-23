#include "table.h"
#include <QWidget>
#include <string.h>>
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

void Table::getTableModel()
{
    QLabel *text = new QLabel("Выберете модель таблицы, которую будете выбирать.");
    QPushButton *accept = new QPushButton("Принять");
    QPushButton *cancel = new QPushButton("Выйти");
    connect(accept, &QPushButton::clicked, this, &Table::readtable);
    connect(cancel, &QPushButton::clicked, dialog, &QDialog::close);
    QGridLayout *layout = new QGridLayout(dialog);
    layout -> addWidget(text, 0, 0, 1, 2);
    text -> setAlignment(Qt::AlignCenter);
    QFont txtfont = text -> font();
    txtfont.setPointSize(14);
    layout -> addWidget(models, 1, 0, 1, 2);
    layout -> addWidget(accept, 2, 0);
    layout -> addWidget(cancel, 2, 1);
    models -> addItem("Axapta");
    models -> setItemData(0, true);
    models -> addItem("Ручная");
    models -> setItemData(1, false);
    dialog -> setModal(true);

    dialog -> show();
}

bool Table::check(const QString &path)
{
    if (path != QString(""))
        return true;
    else
        return false;
}

void Table::readtable()
{
    dialog -> close();
    tab.clear();
    getpath();

    /*if(models -> currentData().toBool())
    {
        xlnt::workbook wb;
        wb.load(filepath.toStdString());
        xlnt::worksheet ws = wb.active_sheet();
        unsigned numOfRow = 0;
        for(auto row : ws.rows(false))
        {
            QVector<QString> ROW(9);
            for(auto cell : row)
            {
                switch(cell.column_index())
                {
                case 2:
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
                            ROW[0] = date.toString("dd.MM.yyyy");
                        }
                        catch(std::invalid_argument)
                        {
                            ROW[0] = QString::fromStdString(date_str);
                        }
                    }
                    else
                        ROW[0] = QString::fromStdString(date_str);

                    break;
                }
                case 3:
                {
                    ROW[5] = QString::fromStdString(cell.to_string());
                    break;
                }
                case 4:
                {
                    ROW[1] = QString::fromStdString(cell.to_string()).sliced(0, 3);
                    break;
                }
                case 6:
                {
                    const int msecperDay = 86400000;
                    double day_count;
                    std::string time_str = cell.to_string();
                    if(cell.is_date())
                    {
                        try
                        {
                            day_count = stod("0" + QString::fromStdString(time_str).sliced(5).toStdString());
                            day_count *= msecperDay;
                            QTime time = QTime::fromMSecsSinceStartOfDay(day_count + 10000);
                            ROW[3] = time.toString("hh:mm");
                        }
                        catch(std::invalid_argument)
                        {
                            ROW[3] = QString::fromStdString(time_str).sliced(12);
                        }
                    }
                    else
                        ROW[3] = QString::fromStdString(time_str).sliced(12);

                    break;
                }
                case 8:
                    ROW[2] = QString::fromStdString(cell.to_string());
                    break;
                default:
                    ROW[8] = QString::number(cell.row());
                }
            }

            tab.append(ROW);
        }
    }
    else
    {*/
        if (check(filepath))
        {
            xlnt::workbook wb;
            wb.load(filepath.toStdString());
            xlnt::worksheet ws = wb.active_sheet();
            unsigned numOfRow = 0;
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
                                    QTime time = QTime::fromMSecsSinceStartOfDay(day_count + 10000);
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

                if(ROW[6] == QString("ОБРЫВ БЛОКА ГЛОНАСС") || ROW[6] == QString("ПРОБКИ") ||
                    ROW[6] == QString("РЕЙС ВЫПОЛНЕН ПРАВИЛЬНО") || ROW[6].toLower() == QString("ракетная опасность") ||
                    ROW[6] == QString("РО"))
                {
                    ROW.push_back(QString::number(numOfRow));
                    tab.append(ROW);
                }
                numOfRow++;
                      }
        }
    //}
}
