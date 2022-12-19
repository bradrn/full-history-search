#include "htmldelegate.h"

#include <QPainter>
#include <QTextDocument>

void HTMLDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<HTMLText>())
    {
        QStyleOptionViewItem option_ = option;
        initStyleOption(&option_, index);

        QString html = qvariant_cast<HTMLText>(index.data()).html;

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        painter->save();

        QTextDocument doc;
        doc.setHtml(html);

        painter->translate(option_.rect.left(), option_.rect.top());
        doc.drawContents(painter, QRect(0, 0, option_.rect.width(), option_.rect.height()));

        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize HTMLDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<HTMLText>())
    {
        QStyleOptionViewItem option_ = option;
        initStyleOption(&option_, index);

        QString html = qvariant_cast<HTMLText>(index.data()).html;

        QTextDocument doc;
        doc.setHtml(html);
        return QSize(doc.idealWidth(), doc.size().height());
    }
    else
        return QStyledItemDelegate::sizeHint(option, index);
}
