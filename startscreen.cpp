
#include <QtWidgets>

#include "button.h"
#include "startscreen.h"
#include "game.h"
#include <QString>

extern Game * game;

StartScreen::StartScreen()
{
    setFixedSize(375,150);

    Button *playButton = createButton(tr("Play"), SLOT(play()));
    Button *instructionsButton = createButton(tr("Instructions"), SLOT(instructions()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(playButton, 0, 0);
    mainLayout->addWidget(instructionsButton, 1, 0);

    setLayout(mainLayout);
    setWindowTitle(tr("Orbal Dilemma"));
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::WindowTitleHint);
    setWindowFlags(Qt::WindowCloseButtonHint);

    instructionsScreen = new QWidget();

}

void StartScreen::play()
{
    game = new Game();
    game->show();
    this->close();
}

void StartScreen::instructions()
{
    instructionsScreen->setFixedSize(425,400);
    instructionsScreen->setWindowFlags(Qt::CustomizeWindowHint);
    instructionsScreen->setWindowFlags(Qt::WindowCloseButtonHint);
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

    instructionText->setText(test);
    instructionsScreen->show();
}

Button *StartScreen::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
