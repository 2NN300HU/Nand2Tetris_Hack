#ifndef COMPUTER_H
#define COMPUTER_H

#include <Windows.h>

#include <cwchar>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "hardware.h"

class Computer {
   private:
    CPU cpu;
    ROM32k instructionMemory;
    Memory dataMemory;
    bool reset;

   public:
    void setInput(const bool reset);
    void finishClock();

    bool****** getScreen() {
        return this->dataMemory.getScreen();
    }

    void keyinput(const bool* in) {
        this->dataMemory.keyInput(in);
    }

    void writeProgram(const std::string fileName);
};

class Monitor {
   private:
    bool* currentScreen;
    void gotoXY(const int x, const int y);
    void printPixel(const int pos, const bool pixel);
    void smallFont();
    void cursorOff();

   public:
    Monitor();

    void printScreen(bool****** newScreen);
    ~Monitor() {
        delete[] currentScreen;
    }
};

#endif