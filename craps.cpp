#include <iostream>
#include <stdio.h>
//#include <QApplication>
//#include <QWidget>
//#include <QGridLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QPixmap>

#include "die.h"
#include "craps.h"
#include "ui_CrapsMainWindow.h"
#include <sstream>

CrapsMainWindow :: CrapsMainWindow(QMainWindow *parent) {
// Build a GUI window with two dice.

    currentBankValue = 10000;
    setupUi(this);

    QObject::connect(rollButton, SIGNAL(clicked()), this, SLOT(rollButtonClickedHandler()));
}

void CrapsMainWindow::printStringRep() {
    // String representation for Craps.
    char buffer[25];
    int length =  sprintf(buffer, "Die1: %i\nDie2: %i\n", die1.getValue(), die2.getValue());
    printf("%s", buffer);
}

void CrapsMainWindow::updateUI() {
//    printf("Inside updateUI()\n");
    char outputString[12];
    std::string die1ImageName = ":/dieImages/" + std::to_string(die1.getValue());
    std::string die2ImageName = ":/dieImages/" + std::to_string(die2.getValue());
    die1UI->setPixmap(QPixmap(QString::fromStdString(die1ImageName)));
    die2UI->setPixmap(QPixmap(QString::fromStdString(die2ImageName)));

    currentBankValueUI->setText(QString::fromStdString(std::to_string(currentBankValue)));
    winsCountUI->setText(QString::fromStdString(std::to_string(winsCount)));
    lossesCountUI->setText(QString::fromStdString(std::to_string(lossesCount)));
    statusUI->setText(QString::fromStdString(status));
    rollingForUI->setText(QString::fromStdString(rollingFor));

}

void CrapsMainWindow::rollButtonClickedHandler() {
//void Craps::rollButtonClickedHandler() {
    printf("Roll button clicked\n");

    if (turn == 1) {
        rollingFor = "";
        status = "";
        updateUI();
        checkBankValue();
    }
    else if (turn == 2) {
        status = "";
        updateUI();
        rollDice();
        winCheck();
    }
}

void CrapsMainWindow::checkBankValue() {
    bool betValid = false;

    do {
        if (currentBankValue - currentBetUI->value() < 0) {
            status = "You have insufficient funds in your bank to bet that amount!";
            betValid = false;
        }
        else {
            betValid = true;
            status = ("You bet $" + std::to_string(currentBetUI->value()));
            currentBankValue -= currentBetUI->value();
            rollDice();
            winCheck();
        }
    }while(betValid == false);
}

void CrapsMainWindow::rollDice() {
    die1.roll();
    die2.roll();
    printStringRep();
    updateUI();
}

void CrapsMainWindow::winCheck() {
    if (turn == 1) {
        rollValue = die1.getValue() + die2.getValue();

        if (rollValue == 2 || rollValue == 3 || rollValue == 12) {
            status = "You rolled a " + std::to_string(rollValue) + " which means you lost on the first roll!";
            lossesCount += 1;
            updateUI();
        } else if (rollValue == 7 || rollValue == 11) {
            status = "You rolled a " + std::to_string(rollValue) + " which means you won on the first roll!";
            currentBankValue += currentBetUI->value();
            winsCount += 1;
            updateUI();
        } else if (rollValue == 4 || rollValue == 5 || rollValue == 6 || rollValue == 8 || rollValue == 9 ||
                   rollValue == 10) {
            status = "You rolled a " + std::to_string(rollValue) + " which means you get to roll again.";
            previousRoll = rollValue;
            rollingFor = "Rolling for a " + std::to_string(previousRoll) + "...";
            turn++;
            updateUI();
        }
    }
    else if (turn == 2) {
        rollValue = die1.getValue() + die2.getValue();

        if (rollValue == previousRoll) {
            rollingFor = "";
            status = "You rolled the same number twice (" + std::to_string(rollValue) + "), which means you won!";
            turn = 1;
            winsCount += 1;

            if (rollValue == 4)
                currentBankValue += (currentBetUI->value() * 2);
            else if (rollValue == 5)
                currentBankValue += currentBetUI->value() + (currentBetUI->value() / 2);
            else if (rollValue == 6)
                currentBankValue += (static_cast<float>((currentBetUI->value())) * 1.2);
            else if (rollValue == 8)
                currentBankValue += (static_cast<float>((currentBetUI->value())) * 1.2);
            else if (rollValue == 9)
                currentBankValue += (static_cast<float>((currentBetUI->value())) * 1.5);
            else if (rollValue == 10)
                currentBankValue += (static_cast<float>((currentBetUI->value())) * 2.0);

            updateUI();
        }
        else {
            rollingFor = "";
            status = "You rolled a " + std::to_string(rollValue) + " which means you lost.";
            turn = 1;
            lossesCount += 1;

            updateUI();
        }
    }
}