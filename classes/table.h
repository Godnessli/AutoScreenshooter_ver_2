#ifndef TABLE_H
#define TABLE_H
#include <QString>

class Table
{
public:
    Table();

    QString filepath;
    bool run_already;

    void getpath();
    bool check(const QString &path);
    QVector<QVector<QString>> readtable();

};

#endif // TABLE_H
