#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>

class HTMLDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    struct HTMLText { QString html; };

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

Q_DECLARE_METATYPE(HTMLDelegate::HTMLText);

#endif // HTMLDELEGATE_H
