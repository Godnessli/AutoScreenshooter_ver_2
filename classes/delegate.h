#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QVector<int> gRows() const
    {
        return good_rows;
    }

    QVector<int> qRows() const
    {
        return question_rows;
    }

    QVector<int> bRows() const
    {
        return bad_rows;
    }

    void setGoodRows(const QVector<int> &rows)
    {
        good_rows = rows;
    }

    void setQuestionRows(const QVector<int> &rows)
    {
        question_rows = rows;
    }

    void setBadRows(const QVector<int> &rows)
    {
        bad_rows = rows;
    }

protected:
    void initStyleOption(QStyleOptionViewItem *option,
                         const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        if(good_rows.contains(index.row()))
        {
            option -> backgroundBrush = QBrush(QColor("#3cb371"));
        }

        if(question_rows.contains(index.row()))
        {
            option -> backgroundBrush = QBrush(QColor("#FF7F50"));
        }

        if(bad_rows.contains(index.row()))
        {
            option -> backgroundBrush = QBrush(QColor("#CD5C5C"));
        }
    }

private:
    QVector<int> good_rows;
    QVector<int> question_rows;
    QVector<int> bad_rows;
};

#endif // DELEGATE_H
