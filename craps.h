//
// Created by Arana Fireheart on 2/2/20.
//

#ifndef CRAPSSTARTER_CRAPS_H
#define CRAPSSTARTER_CRAPS_H

#include "ui_CrapsMainWindow.h"
#include "die.h"
#include <QMainWindow>

class CrapsMainWindow : public QMainWindow, private Ui::CrapsMainWindow {
Q_OBJECT

public:
    CrapsMainWindow(QMainWindow *parent = nullptr);
    void printStringRep();
    void updateUI();

private:
    Die die1, die2;
    bool firstRoll = true;
    int winsCount;
    float currentBankValue;
    int currentBet;
    int previousRoll;
    float payouts[];

    std::tuple<bool, float> playFirstRoll(int rollValue, float currentBank, float currentBet);
    std::tuple<bool, float> playSecondRoll(int rollValue, int previousRoll, float currentBank, float currentBet);
    float processWin(int rollValue, int rollNumber, float currentBank, float currentBet);
    float processLoss(int rollValue, int rollNumber, float currentBank, float currentBet);
    float calculateCurrentBank(int rollValue, int rollNumber, float currentBank, float currentBet, bool wonBet);
    float processBet(float currentBank);

public Q_SLOTS:
    void rollButtonClickedHandler();

};

#endif //CRAPSSTARTER_CRAPS_H
//
// Created by Arana Fireheart on 1/30/20.
//

#ifndef CRAPSSTARTER_DIE_H
#define CRAPSSTARTER_DIE_H

//
// Created by Arana Fireheart on 1/30/20.
//
#include <string>

class Die {
protected:
    int numberOfSides;
    int startingValue;
    int increment;
    std::string color;
    std::string name;
    int value;
public:
    Die(int openingNumberOfSides = 6, int openingStartingValue = 1, int openingIncrement = 1,
        std::string openingColor = "White", std::string openingName = "Bones") ;
    std::string stringRep();
    int roll();
    void setValue(int newValue);
    int getValue();
    void setNumberOfSides(int newNumberOfSides);
    int getNumberOfSides();
    void setStartingValue(int newStartingValue);
    int getStartingValue();
    void setIncrement(int newIncrement);
    int getIncrement();
    void setColor(std::string newColor);
    std::string getColor();
    void setName(std::string newName);
    std::string getName();
};

struct BadDieValueException : public std::exception {
    const char * what() const throw() {
        return "Can't set die value; value out of range.";
    }
};

#endif //CRAPSSTARTER_DIE_H
