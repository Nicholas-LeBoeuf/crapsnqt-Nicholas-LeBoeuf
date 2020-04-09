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

bool isInteger(const std::string & s);
void displayResults(float currentBank, float currentBet);

CrapsMainWindow :: CrapsMainWindow(QMainWindow *parent) {
    // Build a GUI window with two dice.

    setupUi(this);

    Die die1, die2;
    bool firstRoll = true;
    int winsCount = 0;
    int lossesCount = 0;
    float currentBankValue;
    int currentBet = 10;
    int previousRoll = 0;
    const float payouts[] = {0.0, 0.0, 1.0, 1.0, 2.0, 1.5, 1.2, 1.0, 1.2, 1.5, 2.0, 1.0, 1.0};

    currentBankValue = 10000;
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

    snprintf(outputString, sizeof(outputString), "%.2f", currentBankValue);
    currentBankValueUI->setText(QString::fromStdString(std::string(outputString)));
}

float CrapsMainWindow::processBet(float currentBank) {
    float attemptedBet;
    bool  betAccepted = false;

    std::string userInput = "";
    while (!betAccepted){
        if(isInteger(userInput)) {
            attemptedBet = stof(userInput);
            if (attemptedBet <= currentBank) {
                betAccepted = true;
                return attemptedBet;
            } else {
                betAccepted = false;
            }
        } else
            return 0.0;
    }
}

std::tuple<bool, float>  CrapsMainWindow::playFirstRoll(int rollValue, float currentBank, float currentBet){
    std::cout << rollValue << "\n";
    switch (rollValue) {
        case 7:
        case 11: {
            currentBank = processWin(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        case 2:
        case 3:
        case 12: {
            currentBank = processLoss(rollValue, 1, currentBank, currentBet);
            return std::make_tuple(true, currentBank);
        }
        default: {
            return std::make_tuple(false, currentBank);
        }
    }
};

std::tuple<bool, float>  CrapsMainWindow::playSecondRoll(int rollValue, int previousRoll,  float currentBank, float currentBet){
    if (rollValue == previousRoll) {
        return std::make_tuple(true, processWin(rollValue, 2, currentBank, currentBet));
    } else {
        return std::make_tuple(true, processLoss(rollValue, 2, currentBank, currentBet));
    }
};

float CrapsMainWindow::processWin(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You won!" << "\n";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, true);
}

float CrapsMainWindow::processLoss(int rollValue, int rollNumber, float currentBank, float currentBet) {
    std::cout << "You lost." << "\n";
    return calculateCurrentBank(rollValue, rollNumber, currentBank, currentBet, false);
};

float CrapsMainWindow::calculateCurrentBank(int rollValue, int rollNumber, float currentBank, float currentBet, bool wonBet) {
    if (rollNumber == 1) {
        if(wonBet)
            return currentBank + currentBet;
        else
            return currentBank - currentBet;
    }
    else {
        if(wonBet)
            return currentBank + currentBet * payouts[rollValue];
        else
            return currentBank - currentBet * payouts[rollValue];
    }
};

// Player asked for another roll of the dice.
void CrapsMainWindow::rollButtonClickedHandler() {
//void Craps::rollButtonClickedHandler() {
    bool rollCompleted = false;
    float localBank = currentBankValue;
    int rollValue;

    printf("Roll button clicked\n");
    rollValue = die1.roll() + die2.roll();
    if(firstRoll) {
        currentBet = processBet(currentBankValue);
        std::tie(rollCompleted, localBank) = playFirstRoll(rollValue, currentBankValue, currentBet);
        if (rollCompleted) {
            firstRoll = 0;
            rollCompleted = false;
        }
        else {
            previousRoll = rollValue;
            firstRoll = 2;
            rollCompleted = false;
        }
    }
    else {
        std::tie( rollCompleted, localBank) = playSecondRoll(rollValue, previousRoll, currentBankValue, currentBet);
        if (rollCompleted) {
            previousRoll = rollValue;
            firstRoll = 0;
            rollCompleted = false;
        }
    }
    printStringRep();
    updateUI();
}


inline bool isInteger(const std::string & s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}