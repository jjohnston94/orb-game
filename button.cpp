#include <QtWidgets>

#include "button.h"
/*
 * The idea and concept for this Button class as an extension of
 * a QToolButton were borrowed from a QT calculator example.
 * This is a very simple implementation. sizeHint is currently
 * a bit unnecessary, but allows for future startscreen additions
 * very easily
 */

Button::Button(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    // The buttons are set to a constant size
    setFixedSize(150,50);

    // Set the text to what the parameter is
    setText(text);
}

// Provides a hint for the size that allows the spacing of multiple buttons to work out better in a layout
QSize Button::sizeHint() const
{
    // Get the basic size hint
    QSize size = QToolButton::sizeHint();

    // Add 20 the height and set the width to the max
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());

    // Return the calculated size
    return size;
}

































































































































































































































// You found the secret portion of the code! Congratulations!
struct Cake
{
    int candles;
    QString cakeType;
    QString tag = "The cake is a lie";
    int calories;
    int hpRestore;
    int atoms;
    int elements;
    QList<QString*> ingredients;
};
