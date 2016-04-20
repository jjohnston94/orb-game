/*
COP3503 Term Project Group 25
Team Leader:
Timon Angerhofer
Members:
Luke Bickell
Jeremy Johnston
Jonathan Kyle
Rebecca Wedow
Professor:
Dr. Liu
Orbal Dilemma – Project Report:
        Orbal Dilemma is an interactive game where
    the player is a movable orb that must compete
    with other AI orbs for survival. The player’s
    orb moves with the ‘WASD’ keys or the arrow keys,
    and advances by eating orbs smaller than it,
    which will increase the player orb’s size. When
    the player orb reaches the bottom of the screen,
    the player wins! The game is split up into five
    levels, and the player can only advance into the
    next level below once it reaches a certain size.
    If the player orb is eaten by a larger orb, the
    player is reset to a level above, and its size
    is decreased. In order to assist the orb, there
    are small feeder orbs at the top of the screen
    which can assist the player in increasing size
    (but only a certain amount).
        Our group had a positive experience working
    on this project and spent a lot of effort. We
    wrote an original song and created our own
    artwork for the game. We all learned how to
    work on code as a team, and gained valuable
    experience with GitHub. No one on our team had
    ever done a programming assignment with a team,
    so this was a new and important experience. We
    had to coordinate and keep a strict schedule so
    we could use classes that other members had
    written when we needed them. We also had to deal
    with a team member dropping the class, and had
    to shift our assignment accordingly. We are very
    proud of our finished project.
        Although our project is complete, there are
    a few features that we were considering that we
    could implement in the future. One feature would
    be that of different difficulties, where the
    user could select at the beginning of the game
    what difficulty they would like, thus changing
    things like player speed, growth rate of the
    player, etc. Another feature would be that of an
    urchin orb. This orb would not move or grow in
    size, but it would destroy any orbs that come
    into contact with it, including the player.
*/

#include <QApplication>
#include "game.h"
#include "startscreen.h"
#include <iostream>\

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // necessary for any qt program

    StartScreen start;
    start.show();

    return a.exec();
}
