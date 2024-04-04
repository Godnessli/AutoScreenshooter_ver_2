#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QVector<int> rows() const
    {
        return m_rows;
    }

    void setRows(const QVector<int> &rows)
    {
        m_rows = rows;
    }

protected:
    void initStyleOption(QStyleOptionViewItem *option,
                         const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        if(m_rows.contains(index.row()))
        {
            option -> backgroundBrush = QBrush(QColor("#3cb371"));
        }
    }

private:
    QVector<int> m_rows;
};

#endif // DELEGATE_H
