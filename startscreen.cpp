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
    setFixedSize(375,150);

    // Create the buttons that will be on the widget
    Button *playButton = createButton(tr("Play"), SLOT(play()));
    Button *instructionsButton = createButton(tr("Instructions"), SLOT(instructions()));

    // Simple gridlayout
    QGridLayout *mainLayout = new QGridLayout;

    // Play button on top
    mainLayout->addWidget(playButton, 0, 0);

    // Instructions on bottom
    mainLayout->addWidget(instructionsButton, 1, 0);

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
    instructionsScreen->setFixedSize(425,200);

    // Set the window hints to only have a close button
    instructionsScreen->setWindowFlags(Qt::CustomizeWindowHint);
    instructionsScreen->setWindowFlags(Qt::WindowCloseButtonHint);

    // Make a QLabel for the instructional text
    QLabel * instructionText = new QLabel(instructionsScreen);
    const char* test =  "You, the Player, move by using either "
                        "the WASD keys or the arrows keys.\n\n"
                        "You gain size by eating Food (at the top), "
                        "or eating other orbs smaller \nthan you. "
                        "This allows you to move down to lower areas "
                        "of the game \nenvironment.\n\n"
                        "Getting eaten will force you to move \n"
                        "back up a level, so dodge well!\n\n"
                        "Press 'Esc' to exit fullscreen and "
                        "'F' to re-enter fullscreen.";

    // Set the text of the qlabel
    instructionText->setText(test);

    // Show the instructions screen
    instructionsScreen->show();
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
