#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "button.h"
#include <QToolButton>

class StartScreen : public QWidget
{
    Q_OBJECT
public:
    StartScreen();
private slots:
    void play();
    void instructions();
    void report();
private:
    Button *createButton(const QString &text, const char *member);
    QWidget * instructionsScreen;
    QWidget * reportScreen;

};

#endif // STARTSCREEN_H
