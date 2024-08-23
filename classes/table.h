#ifndef TABLE_H
#define TABLE_H

#include <QtWidgets>
#include <QString>
#include <QSqlQueryModel>
#include "xlnt/xlnt.hpp"

class Table : public QWidget
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr);

    QString filepath;
    QMap<unsigned, unsigned> mapOfTable;
    QVector<QVector<QString>> tab;
    bool run_already;

    void getpath();
    bool check(const QString &path);
    void readtable();
    QComboBox *models = new QComboBox(dialog);
    void getTableModel();
private:
    QDialog *dialog = new QDialog();

};

#endif // TABLE_H
