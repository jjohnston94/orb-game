#include "startscreen.h"
#include <iostream>
#include <QList>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <typeinfo>

startScreen::startScreen()
{
    // Get the specific geometry of the user's screen to get the window size
    QRect rec = QApplication::desktop()->screenGeometry(); // get screen geometry for width and height
    WINDOW_HEIGHT = rec.height();
    WINDOW_WIDTH = rec.width();

    // Total size of the game environment (on and off screen)
    SCENE_WIDTH = 1920;
    SCENE_HEIGHT = 1080;

    // Initialize the scene, which holds QGraphicsItems
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, SCENE_WIDTH, SCENE_HEIGHT);

    // Initialize the view and set it to display the scene
    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    //  A nice gradient background
    scene->setBackgroundBrush(QPixmap(":/images/resources/orbLogo1.png"));
}

void startScreen::show()
{
    view->showFullScreen();
}
