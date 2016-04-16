#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include <QGraphicsView>
#include <QList>
#include <QObject>
#include <QGraphicsScene>


class startScreen : public QObject
{
    Q_OBJECT
public:
    startScreen();
    void show();
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
private:
    QGraphicsScene * scene;
    QGraphicsView * view;

    int SCENE_WIDTH;
    int SCENE_HEIGHT;
};

#endif // STARTSCREEN_H
