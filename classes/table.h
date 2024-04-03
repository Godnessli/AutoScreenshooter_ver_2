#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>
#include <QString>
#include <QSqlQueryModel>

class Table : public QWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr);

    QString filepath;
    bool run_already;

    void getpath();
    bool check(const QString &path);
    QVector<QVector<QString>> readtable();
};

#endif // TABLE_H
