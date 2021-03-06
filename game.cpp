#include "game.h"
#include <iostream>
#include <QTimer>
#include <QList>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <typeinfo>
#include <QMediaPlayer>
#include <QMediaPlaylist>

/*
 * Game.cpp is the class that implements the orb classes and controls game variables
 * and the game loop. It uses QGraphicsScene and QGraphicsView to hold the orb Items
 * and display them to the user. A single QTimer controls the gameloop, which calls
 * all the necessary methods such as move(), grow(), etc. which are necessary for
 * the movement and composition of the scene. The player size grows/shrinks depending
 * on how low/high they are to prevent the player from getting to large when they go down
 * and eat bigger orbs. The player is also prevented from moving too high or low if they are
 * not the right size.
 *
 * The win screen is also handled once the player has won. This is the most important class!
 * It is called from the main.cpp and started from the user pressing the "play" button on the start
 * screen.
 *
 * Music is played using a QMediaPlayer and an attached playlist.
 *
 */

Game::Game()
{
    // Get the specific geometry of the user's screen to get the window size
    QRect rec = QApplication::desktop()->screenGeometry(); // get screen geometry for width and height
    WINDOW_HEIGHT = rec.height();
    WINDOW_WIDTH = rec.width();

    // Total size of the game environment (on and off screen)
    SCENE_WIDTH = 3000;
    SCENE_HEIGHT = 20000;

    // Initialize the scene, which holds QGraphicsItemsf

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, SCENE_WIDTH, SCENE_HEIGHT);

    // Initialize list of AIOrbs so we can manipulate all of them
    aiList = new QList<AIOrb *>();

    // Initialize list of FeederOrbs so we can manipulate THEM too
    feederList = new QList<FeederOrb *>();

    // Scale
    scale = 1;
    lastScale = 1;

    // Initialize the player, add them to the scene, and allow them to be focused on
    player = new PlayerOrb(); // the player
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable); // Focus allows the object to take in keypresses
    player->setFocus();

    // Initialize the view and set it to display the scene
    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    // Set the background images (which are dependent on your own directory in which they are saved)
    scene->setBackgroundBrush(QPixmap("C:/Users/Luke/Documents/QtProjects/OrbalDilemma/resources/BG1.png"));
    scene->setForegroundBrush(QPixmap("C:/Users/Luke/Documents/QtProjects/OrbalDilemma/resources/BG.png"));

    won = false;

    // Single timer that calls gameLoop which controls the movements of the other objects
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));

    // Music - set the playlist
    playlist = new QMediaPlaylist;
    // Add the song to the playlist
    playlist->addMedia(QUrl("qrc:/music/resources/Orbal_Dilemma.wav"));
    // Create a QMediaPlayer
    music = new QMediaPlayer();
    // Set it to the playlist with the song
    music->setPlaylist(playlist);
    // Start the song on a loop
    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    music->play();

    //Sound for when an orb is eaten
    bloop = new QMediaPlayer();
    bloop->setMedia(QUrl("qrc:/music/resources/bloop.wav"));


}

void Game::show()
{
    view->showFullScreen();

    // Timer called every 20 milliseconds
    timer->start(20);
}

// Spawn the AI
void Game::spawnAI()
{
    int max = 25;
    if (aiList->size() < max)
    {
        //Spawn Orbs Below
        if (player->y() > 0 && player->y() < SCENE_HEIGHT-WINDOW_HEIGHT)
        {
            qreal y = (qrand() % 1500) + (player->y()+WINDOW_HEIGHT);
            qreal yR = qrand() % 100 + 100*(int)(y / 4000);
            if (yR < 20) yR = 20 + qrand() % 20;
            AIOrb* newAI = new AIOrb(yR, qrand() % SCENE_WIDTH, y);
            aiList->append(newAI);
            scene->addItem(newAI);
        }

        //Spawn Orbs Above
        if (player->y() < SCENE_HEIGHT && player->y() > WINDOW_HEIGHT + 500)
        {
            qreal y = (player->y()-WINDOW_HEIGHT) - (qrand() % 1500);
            if (y > 500)
            {
                qreal yR = qrand() % 100 + 100*(int)(y / 4000);
                if (yR < 20) yR = 20 + qrand() % 20;
                AIOrb* newAI2 = new AIOrb(yR, qrand() % SCENE_WIDTH, y);
                aiList->append(newAI2);
                scene->addItem(newAI2);
            }
        }
    }
}

// Properly removes an orb from the scene and list and deletes it
void Game::deleteAI(Orb * orb)
{
    scene->removeItem((QGraphicsItem*) orb);
    if (typeid(*(orb)) == typeid(AIOrb))
        aiList->removeAt(aiList->indexOf((AIOrb*) orb));
    else if (typeid(*(orb)) == typeid(FeederOrb))
        feederList->removeAt(feederList->indexOf((FeederOrb*)orb));
    delete orb;
}

// Loop called every 20 ms
void Game::gameLoop()
{
    player->move();

    // Check player collisions
    collidePlayer();

    // Display the correct image for the player
    player->correctImage();

    // Re-set the focus to the player to make sure it can still move
    player->setFocus();

    // Center the view on the player so that it follows the player around
    view->centerOn(player);

    // Spawn feeders
    spawnFeeders();

    // Move AI if in view and check their collisions
    moveCollideOrbs();

    // Don't do these things if you won
    if (!won)
    {
        // Cull dem bad orbs
        cullBadOrbs();

        // Spawn AIOrbs if applicable
        spawnAI();

        // Make sure all of the AI grow/shrink regardless of whether they are in the view
        for (int i = 0; i < aiList->size(); i++)
            aiList->at(i)->grow();

        // If an AI gets too small, delete it
        for (int i = 0; i < aiList->size(); i++)
        {
            if (aiList->at(i)->getRadius() < 10)
                deleteAI(aiList->at(i));
        }

        // Make the scale of everything change at height intervals
        int divisor = SCENE_HEIGHT / 5;
        if (lastScale != scale)
        {
            changeScale();
            lastScale = scale;
        }
        scale = ((int) (player->y()/divisor)) + 1;
    }

    if (player->y()+5 >= SCENE_HEIGHT - player->getRadius()*2 && !won)
    {
        won = true;
        win();
    }
}

// Changes the size of the player and all AI based on scale
void Game::changeScale()
{
    // If the player is moving up a level or 'scale'
    if (scale < lastScale)
    {
        player->growBy( (player->getActualRadius() / pow(2, scale) ));

        for (int i = 0; i < aiList->size(); i++)
        {
            AIOrb * current = aiList->at(i);
            current->growBy(current->getRadius());
        }
    }
    // Otherwise the player is moving down a level
    else
    {
        player->shrinkBy( (player->getActualRadius() / pow(2, scale-1)));

        for (int i = 0; i < aiList->size(); i++)
        {
            AIOrb * current = aiList->at(i);
            current->shrinkBy(current->getRadius()/2);
        }
    }
}

// Returns the value of scale
int Game::getScale()
{
    return scale;
}

void Game::moveCollideOrbs()
{
    // Get a list of all the items seen in the view
    itemViewList = view->items(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);

    // For every item seen in the view
    for (int i = 0; i < itemViewList.size(); i++)
    {
        // Cast each item in the list as an Orb
        Orb * currentOrb = (Orb*)itemViewList[i];

        // If the item is an AIOrb
        if (typeid(*(currentOrb)) == typeid(AIOrb))
        {
            // Cast it as an AIOrb, and allow it to move
            AIOrb * aiOrb = (AIOrb*)currentOrb;
            aiOrb->move();

            // Get a list of collisions with the current AIOrb
            QList<QGraphicsItem*> collisions = aiOrb->collidingItems();
            qreal thisRadius = aiOrb->getRadius();

            // For every item the current AIOrb is colliding with
            for (int i = 0; i < collisions.size(); i++)
            {
                // Cast it as an Orb
                Orb * collided = (Orb*)collisions[i];
                qreal oradius = collided->getRadius();

                // If the current AIOrb is bigger than the other object (and the other object is an AIOrb)
                if (thisRadius > oradius && typeid(*(collided)) == typeid(AIOrb))
                {
                    // Remove the item from the scene and aiList and delete it
                    itemViewList.removeAt(itemViewList.indexOf(collisions[i]));
                    deleteAI((AIOrb*)collided);

                    // Then increase the size of the current AIOrb
                    if (aiOrb->getRadius() < player->getRadius() + 20)
                    {
                        qreal radiusDiff = (sqrt((double) (thisRadius*thisRadius + oradius*oradius))) - thisRadius;
                        aiOrb->growBy(radiusDiff/4);
                        aiOrb->setActualRadius(aiOrb->getActualRadius() + radiusDiff/4);
                    }

                    //Play a bloop
                    if (bloop->state() == QMediaPlayer::PlayingState)
                    {
                        bloop->setPosition(0);
                    }
                    else if (bloop->state() == QMediaPlayer::StoppedState)
                    {
                        bloop->play();
                    }

                }
                else if (thisRadius >= oradius && typeid(*(collided)) == typeid(FeederOrb))
                {
                    itemViewList.removeAt(itemViewList.indexOf(collisions[i]));
                    deleteAI(collided);

                    // Add the area of the eaten orb to the AI Orb but don't let the player grow bigger than 300 from eating
                    if (thisRadius < 75)
                    {
                        qreal radiusDiff = sqrt( (double) (thisRadius*thisRadius + oradius*oradius) ) - thisRadius;
                        aiOrb->growBy(radiusDiff/4);
                        aiOrb->setActualRadius(aiOrb->getActualRadius() + radiusDiff/4);
                    }

                    //Play a bloop
                    if (bloop->state() == QMediaPlayer::PlayingState)
                    {
                        bloop->setPosition(0);
                    }
                    else if (bloop->state() == QMediaPlayer::StoppedState)
                    {
                        bloop->play();
                    }
                }
            }
        }

        // If the Orb is a FeederOrb
        else if (typeid(*(currentOrb)) == typeid(FeederOrb))
        {
            // Cast it as a feeder orb
            FeederOrb * feeder = (FeederOrb*)currentOrb;
            feeder->move();
        }
    }
}

void Game::collidePlayer()
{
    // Check collisions with the player
    QList<QGraphicsItem *> collisions = player->collidingItems();

    // For every item the player is colliding with
    for (int i = 0; i < collisions.size(); i++)
    {
        // Cast the item as an Orb
        Orb * current = (Orb*)collisions[i]; // cast the colliding objects as Orb references so their member functions can be accessed
        qreal aiRadius = current->getRadius();
        qreal pRadius = player->getRadius();

        // If the player is bigger than the other (AI)
        if (pRadius >= aiRadius && typeid(*(current)) == typeid(AIOrb))
        {
            // Remove the item from the scene and aiList and delete it
            deleteAI(current);

            // Add one to orbs eaten
            orbsEaten++;

            // Add the area of the eaten orb to the player but don't let the player grow bigger than 300 from eating
            if (player->getRadius() < 300)
            {
                qreal radiusDiff = sqrt( (double) (pRadius*pRadius + aiRadius*aiRadius) ) - pRadius;
                player->setActualRadius(player->getActualRadius() + radiusDiff/4);
                player->growBy(radiusDiff/4);
            }

            //Play a bloop sound
            if (bloop->state() == QMediaPlayer::PlayingState)
            {
                bloop->setPosition(0);
            }
            else if (bloop->state() == QMediaPlayer::StoppedState)
            {
                bloop->play();
            }
        }

        //If the player is smaller than the other (gets eaten)
        else if (pRadius < aiRadius && typeid(*(current)) == typeid(AIOrb))
        {
            // Delete all the AI
            for (int j = 0; j < aiList->size();)
            {
                deleteAI(aiList->at(j));
            }

            scale -= 1;
            if (scale < 1)
            {
                scale = 1;
            }

            // Set the player to a base size and move them up a level
            lastScale = scale;
            player->setPos(1500,(scale - 1)*4000 + 2000);
            player->setRadius(40);
            player->setActualRadius(40*pow(2,scale - 1));

            // Add one to deaths
            deaths++;
        }

        // If the player is bigger than the other feeder orb
        else if (pRadius >= aiRadius && typeid(*(current)) == typeid(FeederOrb))
        {
            // Delete the feeder orb
            deleteAI(current);

            // Add the area of the eaten orb to the player but don't let the player grow bigger than 300 from eating
            if (player->getRadius() < 75)
            {
                qreal radiusDiff = sqrt( (double) (pRadius*pRadius + aiRadius*aiRadius) ) - pRadius;
                player->setActualRadius(player->getActualRadius() + radiusDiff);
                player->growBy(radiusDiff);
            }

            //Play a bloop
            if (bloop->state() == QMediaPlayer::PlayingState)
            {
                bloop->setPosition(0);
            }
            else if (bloop->state() == QMediaPlayer::StoppedState)
            {
                bloop->play();
            }
        }
    }
}

// Spawn the feeder orbs
void Game::spawnFeeders()
{
    // Max of 25 feeder orbs on screen at a time
    int max = 25;
    int heightRange;

    // Still spawn feeder orbs after having won, but in the whole screen
    if (won)
    {
        heightRange = SCENE_HEIGHT;
    }
    else
    {
        heightRange = 500;
    }

    if (feederList->size() < max)
    {
        FeederOrb * newFeeder = new FeederOrb((qrand() % 15)+5, qrand() % SCENE_WIDTH, qrand() % heightRange);
        feederList->append(newFeeder);
        scene->addItem(newFeeder);
    }

    // The player can see the range, so spawn some orbs to the left and right where the player can't see
    // If the size of the feeder list is a thid of max
    else if (feederList->size() < max / 3 && !won)
    {
        //Prevent orbs from spawning on the screen with the player
        if (player->x() > WINDOW_WIDTH/2 - player->getRadius() + 100)
        {
            FeederOrb * newFeeder = new FeederOrb((qrand() % 15)+5, qrand() % (int)(player->x() + player->getRadius() - WINDOW_WIDTH / 2 - 40), qrand() % 500);
            feederList->append(newFeeder);
            scene->addItem(newFeeder);
        }
        if (player->x() < 3000 - WINDOW_WIDTH/2 - player->getRadius() - 100)
        {
            int rightScreenBoundary = player->x() + player->getRadius() + WINDOW_WIDTH/2 + 40;
            FeederOrb * newFeeder = new FeederOrb((qrand() % 15)+5, rightScreenBoundary + qrand() % (3000 - rightScreenBoundary), qrand() % 500);
            feederList->append(newFeeder);
            scene->addItem(newFeeder);
        }
    }
}

// Get rid of AI orbs that are too high or low
void Game::cullBadOrbs()
{
    for (int i = 0; i < aiList->size(); i++)
    {
        // If a AI is too far above the player, delete it
        if (aiList->at(i)->y() > player->y() + 2000)
        {
            deleteAI(aiList->at(i));
        }
        // Or too low below the player
        else if (aiList->at(i)->y() < player->y() - 2000)
        {
            deleteAI(aiList->at(i));
        }
    }
}

// If the player reachs the bottom
void Game::win()
{
    // Delete all the AI
    for (int i = 0; i < aiList->size();)
        deleteAI(aiList->at(i));
    delete aiList;

    // Delete all the feeder orbs
    for (int i = 0; i < feederList->size();)
        deleteAI(feederList->at(i));

    // Limit the screen size
    scene->setSceneRect(0,0, WINDOW_WIDTH-5, WINDOW_HEIGHT-5);
    SCENE_WIDTH = WINDOW_WIDTH;
    SCENE_HEIGHT = WINDOW_HEIGHT;

    // Credits HTML/text
    QGraphicsTextItem * credits = new QGraphicsTextItem();
    credits->setPos(WINDOW_WIDTH/5, WINDOW_HEIGHT/8);
    credits->setTextWidth(3*WINDOW_WIDTH/5);
    credits->setHtml("<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:72px;\"><strong>YOU WIN!!!!</strong></span></span></p>"
                     "<p>&nbsp;</p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:48px;\">Programmed by:</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Timon Angerhofer</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Luke Bickell</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Jeremy Johnston</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Jon Kyle</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Rebecca Wedow</span></span></p>"
                     "<p>&nbsp;</p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:48px;\">Music by:</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Jon Kyle</span></span></p>"
                     "<p>&nbsp;</p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:48px;\">Art by:</span></span></p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:36px;\">Rebecca Wedow</span></span></p>"
                     "<p>&nbsp;</p>"
                     "<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:48px;\">Thanks for playing!!</span></span></p>");
    scene->addItem(credits);

    // Statistic counters
    QGraphicsTextItem * counters = new QGraphicsTextItem();
    counters->setPos(3*WINDOW_WIDTH/5, WINDOW_HEIGHT/8);
    counters->setTextWidth(2*WINDOW_WIDTH/5);
    counters->setHtml("<p><span style=\"font-family:georgia,serif;\"><span style=\"font-size:48px;\">Orbs Eaten: " +
                      QString::number(orbsEaten) +
                      "</span></span></p>"
                      "<p><span style=\"font-size:48px;\"><span style=\"font-family:georgia,serif;\">Deaths: " +
                      QString::number(deaths) +
                      "</span></span></p>");
    scene->addItem(counters);

}
