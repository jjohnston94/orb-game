#include <QtWidgets>
#include "startscreen.h"
#include "game.h"
#include <QString>

/*
 * The startscreen is the window that pops up first
 * in main.cpp. It is a very simple window, but it allows
 * the user to view instructions before starting the game itself
 * There is plenty of potential for things to be added to the start
 * screen in the future if needed.
 */

// Allow the startscreen to have access to the game in main so that it can start it
extern Game * game;

StartScreen::StartScreen()
{
    // A decent size methinks
    setFixedSize(375,200);

    // Create the buttons that will be on the widget
    Button *playButton = createButton(tr("Play"), SLOT(play()));
    Button *instructionsButton = createButton(tr("Instructions"), SLOT(instructions()));
    Button *reportButton = createButton(tr("Report"), SLOT(report()));

    // Simple gridlayout
    QGridLayout *mainLayout = new QGridLayout;

    // Play button on top
    mainLayout->addWidget(playButton, 0, 0);

    // Instructions on bottom
    mainLayout->addWidget(instructionsButton, 1, 0);

    // Report at bottom really
    mainLayout->addWidget(reportButton,2,0);

    // Set the Layout of the widget
    setLayout(mainLayout);

    // Set the title of the window
    setWindowTitle(tr("Orbal Dilemma"));

    // Set the window hints to only show title and closebutton
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::WindowTitleHint);
    setWindowFlags(Qt::WindowCloseButtonHint);

    // Initialize the instructions screen
    instructionsScreen = new QWidget();

    // Initialize the report screen
    reportScreen = new QWidget();

}

// When the play button is clicked
void StartScreen::play()
{
    // Initialize the game
    game = new Game();
    // Show it
    game->show();
    // Close the startscreen
    this->close();
}

// When the instrucitons button is clicked
void StartScreen::instructions()
{
    // A very nice size methinks
    instructionsScreen->setFixedSize(425,550);

    // Set the window hints to only have a close button
    instructionsScreen->setWindowFlags(Qt::CustomizeWindowHint);
    instructionsScreen->setWindowFlags(Qt::WindowCloseButtonHint);

    // Make a QLabel for the instructional text
    QLabel * instructionText = new QLabel(instructionsScreen);
    const char* text =  "You, the Player, move by using either "
                        "the WASD keys or the arrows keys.\n\n"
                        "You gain size by eating Food (at the top), "
                        ""
                        "or eating other orbs smaller \nthan you. "
                        "This allows you to move down to lower areas "
                        "of the game \nenvironment.\n\n"
                        "Getting eaten will force you to move \n"
                        "back up a level, so dodge well!\n\n"
                        "Press 'Esc' to exit fullscreen and "
                        ""
                        "'F' to re-enter fullscreen.\n\n\n"
                        "A long time ago, in a galaxy far far\n"
                        "away, there lived a small moon, \n"
                        "named Ozzeran. He lived in solitude,\n"
                        "and was very content with his solitude.\n\n"
                        "One day (or night), the stars nearest\n"
                        "to him began growing larger, or so it\n"
                        "seemed. Ozzeran began to grow afraid, \n"
                        ""
                        "fearing that they would burst and \n"
                        "destroy him along with them.\n\n"
                        "Ozzeran decided that he would have to\n"
                        "take his fate into his own hands\n"
                        "(as if he had hands)!\n\n"
                        "He looked far and wide for a solution,\n"
                        "and realized that the only way he\n"
                        ""
                        "could save himself was by consuming\n"
                        "the stars whole. He was not big enough,\n"
                        "so he set out to consume other moons \n"
                        "until he could grow big enough to save\n"
                        "his own life.\n\n"
                        "Selfish bastard. "
                        "";

    // Set the text of the qlabel
    instructionText->setText(text);

    // Show the instructions screen
    instructionsScreen->show();
}

void StartScreen::report()
{
    // A very nice size methinks
    reportScreen->setFixedSize(400,950);

    // Set the window hints to only have a close button
    reportScreen->setWindowFlags(Qt::CustomizeWindowHint);
    reportScreen->setWindowFlags(Qt::WindowCloseButtonHint);

    // Make a QLabel for the instructional text
    QLabel * reportText = new QLabel(reportScreen);
    const char* text =  "COP3503 Term Project Group 25\n"
                        "Team Leader:\n"
                        "Timon Angerhofer\n"
                        "\n"
                        "Members:\n"
                        "Luke Bickell\n"
                        "Jeremy Johnston\n"
                        "Jonathan Kyle\n"
                        "Rebecca Wedow\n"
                        "\n"
                        "Professor:\n"
                        "Dr. Liu\n"
                        "\n"
                        "Orbal Dilemma – Project Report:\n\n"
                        ""
                        "\tOrbal Dilemma is an interactive game where\n"
                        "the player is a movable orb that must compete\n"
                        "with other AI orbs for survival. The player’s\n"
                        "orb moves with the ‘WASD’ keys or the arrow keys,\n"
                        "and advances by eating orbs smaller than it,\n"
                        "which will increase the player orb’s size. When\n"
                        "the player orb reaches the bottom of the screen,\n"
                        "the player wins! The game is split up into five\n"
                        "levels, and the player can only advance into the\n"
                        "next level below once it reaches a certain size.\n"
                        "If the player orb is eaten by a larger orb, the\n"
                        "player is reset to a level above, and its size\n"
                        "is decreased. In order to assist the orb, there\n"
                        "are small feeder orbs at the top of the screen\n"
                        "which can assist the player in increasing size\n"
                        "(but only a certain amount).\n\n"
                        ""
                        "\tOur group had a positive experience working\n"
                        "on this project and spent a lot of effort. We\n"
                        "wrote an original song and created our own\n"
                        "artwork for the game. We all learned how to\n"
                        "work on code as a team, and gained valuable\n"
                        "experience with GitHub. No one on our team had\n"
                        "ever done a programming assignment with a team,\n"
                        "so this was a new and important experience. We\n"
                        "had to coordinate and keep a strict schedule so\n"
                        "we could use classes that other members had\n"
                        "written when we needed them. We also had to deal\n"
                        "with a team member dropping the class, and had\n"
                        "to shift our assignment accordingly. We are very\n"
                        "proud of our finished project.\n\n"
                        ""
                        "\tAlthough our project is complete, there are\n"
                        "a few features that we were considering that we\n"
                        "could implement in the future. One feature would\n"
                        "be that of different difficulties, where the\n"
                        "user could select at the beginning of the game\n"
                        "what difficulty they would like, thus changing\n"
                        "things like player speed, growth rate of the\n"
                        "player, etc. Another feature would be that of an\n"
                        "urchin orb. This orb would not move or grow in\n"
                        "size, but it would destroy any orbs that come\n"
                        "into contact with it, including the player.\n";

    // Set the text of the qlabel
    reportText->setText(text);

    // Show the instructions screen
    reportScreen->show();
}


// Method for creating a button and connecting it to the right method (borrowed from a QT calculator example)
Button *StartScreen::createButton(const QString &text, const char *member)
{
    // Initialize a button
    Button *button = new Button(text);

    // Connect the button to a method
    connect(button, SIGNAL(clicked()), this, member);

    // Return the button
    return button;
}
