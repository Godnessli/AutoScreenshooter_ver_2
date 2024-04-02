#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include <QSqlQueryModel>

class Table
{
    Q_OBJECT

public:
    Table(QObject *parent = nullptr);

    QString filepath;
    bool run_already;

    void getpath();
    bool check(const QString &path);
    QVector<QVector<QString>> readtable();

};

#endif // TABLE_H
