#include <QtWidgets>

#include "button.h"

Button::Button(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setFixedSize(150,50);
    setText(text);
}

QSize Button::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}
